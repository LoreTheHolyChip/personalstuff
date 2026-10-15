// ==WindhawkMod==
// @id              kde-connect-integration
// @name            KDE Connect Integration
// @description     Phone status in the Windows 11 taskbar with a Fluent notification panel
// @version         0.6.0
// @author          LTHC
// @include         explorer.exe
// @architecture    x86-64
// @compilerOptions -lgdi32 -lshell32 -ladvapi32 -luser32 -ldwmapi -lcomdlg32 -lgdiplus -lole32 -lruntimeobject -lwindowsapp
// ==/WindhawkMod==

// ==WindhawkModReadme==
/*
# KDE Connect Integration

**Requires KDE Connect for Windows to be installed and running, with the phone paired through KDE Connect.**
**The author reports this mod and the companion Battery Module working on their setup.**

Native XAML taskbar button in a reserved left column, with a separate native
notification popup. Taskbar Styler controls the DockLike acrylic islands.
No Python, PowerShell helper, separate app or ADB.

Shows the last battery update received through KDE Connect by the companion
Windhawk battery adapter. Compact expandable phone panel with live KDE Connect
notifications and phone ringing. The native Phone Link panel in Start is not
replaced.

No topmost or layered taskbar overlay is created. Existing left controls are
preserved and restored on unload. Only Explorer owning the primary taskbar is
modified.
*/
// ==/WindhawkModReadme==

#include <windows.h>
#include <shellapi.h>
#include <string>
#include <dwmapi.h>
#include <gdiplus.h>
#include <atomic>

static std::atomic<bool> connected = false;
static HANDLE connectionThread = nullptr, connectionStop = nullptr;

#include "kde-integration-devices.hpp"
#include "kde-integration-dbus.hpp"

static HANDLE threadHandle = nullptr;
static DWORD threadId = 0;
static HANDLE readyEvent = nullptr;
static HWND widget = nullptr, panel = nullptr;
static int charge = -1;
static bool charging = false;
static COLORREF surface = RGB(29, 29, 29),
                foreground = RGB(238, 238, 238),
                muted = RGB(170, 170, 170),
                cardColor = RGB(45, 45, 45),
                hoverColor = RGB(47, 47, 47),
                accent = RGB(120, 205, 240);
static bool darkTheme = true;

static COLORREF Blend(COLORREF a, COLORREF b, int weight) {
    return RGB((GetRValue(a) * (100 - weight) + GetRValue(b) * weight) / 100,
               (GetGValue(a) * (100 - weight) + GetGValue(b) * weight) / 100,
               (GetBValue(a) * (100 - weight) + GetBValue(b) * weight) / 100);
}

static void RefreshTheme() {
    DWORD light = 0, bytes = sizeof(light);
    RegGetValueW(
        HKEY_CURRENT_USER,
        LR"(Software\Microsoft\Windows\CurrentVersion\Themes\Personalize)",
        L"SystemUsesLightTheme", RRF_RT_REG_DWORD, nullptr, &light, &bytes);
    darkTheme = !light;
    DWORD color = 0;
    BOOL opaque = FALSE;
    if (SUCCEEDED(DwmGetColorizationColor(&color, &opaque)))
        accent = RGB((color >> 16) & 255, (color >> 8) & 255, color & 255);
    surface = darkTheme ? RGB(32, 32, 32) : RGB(243, 243, 243);
    foreground = darkTheme ? RGB(245, 245, 245) : RGB(24, 24, 24);
    muted = darkTheme ? RGB(175, 175, 175) : RGB(95, 95, 95);
    cardColor = darkTheme ? RGB(45, 45, 45) : RGB(255, 255, 255);
    hoverColor = Blend(surface, accent, darkTheme ? 25 : 16);
    HIGHCONTRASTW hc{sizeof(hc)};
    if (SystemParametersInfoW(SPI_GETHIGHCONTRAST, sizeof(hc), &hc, 0) &&
        (hc.dwFlags & HCF_HIGHCONTRASTON)) {
        surface = GetSysColor(COLOR_WINDOW);
        foreground = muted = GetSysColor(COLOR_WINDOWTEXT);
        cardColor = surface;
        hoverColor = GetSysColor(COLOR_HIGHLIGHT);
        accent = foreground;
    }
    if (panel) {
        int value = darkTheme;
        DwmSetWindowAttribute(panel, 20, &value, sizeof(value));
        InvalidateRect(panel, nullptr, FALSE);
    }
    if (widget) InvalidateRect(widget, nullptr, FALSE);
}

static std::wstring label = L"KDE Connect";
static bool hover = false;
static const wchar_t* cls = L"KDEConnectIntegration.Taskbar.010";

static std::wstring Env(const wchar_t* name) {
    wchar_t buffer[32768]{};
    GetEnvironmentVariableW(name, buffer, 32768);
    return buffer;
}

static bool panelBatteryIcon = true;
static bool showOpenKDE = false;
static bool settingsMode = false, settingsExpanded = false;

static void LayoutSettings();
static void DrawSettings(HDC dc, RECT r);
static void SettingsClick(HWND w, int x, int y);
static void OpenSettingsPanel();

#include "kde-integration-panel.hpp"
#include "kde-integration-taskbar.hpp"
#include "kde-integration-settings-panel.hpp"

static LRESULT CALLBACK Proc(HWND w, UINT m, WPARAM wp, LPARAM lp) {
    switch (m) {
    case WM_SETTINGCHANGE:
    case WM_THEMECHANGED:
    case WM_DWMCOLORIZATIONCOLORCHANGED:
        RefreshTheme();
        return 0;
    case WM_APP + 41:
        OpenSettingsPanel();
        return 0;
    case WM_APP + 40:
        settingsMode = false;
        OpenPanel();
        return 0;
    case WM_ERASEBKGND:
        return 1;
    case WM_MOUSEMOVE:
        if (!hover) {
            hover = true;
            TRACKMOUSEEVENT t{sizeof(t), TME_LEAVE, w, 0};
            TrackMouseEvent(&t);
            InvalidateRect(w, nullptr, FALSE);
        }
        return 0;
    case WM_MOUSELEAVE:
        hover = false;
        InvalidateRect(w, nullptr, FALSE);
        return 0;
    case WM_LBUTTONUP:
        OpenPanel();
        return 0;
    case WM_KEYUP:
        if (wp == VK_SPACE || wp == VK_RETURN) OpenPanel();
        return 0;
    case WM_TIMER: {
        HWND bar = FindWindowW(L"Shell_TrayWnd", nullptr);
        auto id = DeviceId();
        std::wstring keyPath =
            L"Software\\KDEConnectIntegration\\Battery\\" +
            std::wstring(id.begin(), id.end());
        const wchar_t* key = keyPath.c_str();
        DWORD value = 0, batteryBytes = sizeof(value);
        int nextCharge = -1;
        bool nextCharging = false;
        if (RegGetValueW(HKEY_CURRENT_USER, key, L"Charge",
                         RRF_RT_REG_DWORD, nullptr, &value, &batteryBytes) ==
                ERROR_SUCCESS && value <= 100)
            nextCharge = value;
        batteryBytes = sizeof(value);
        if (RegGetValueW(HKEY_CURRENT_USER, key, L"Charging",
                         RRF_RT_REG_DWORD, nullptr, &value, &batteryBytes) ==
            ERROR_SUCCESS)
            nextCharging = value != 0;
        charge = nextCharge;
        charging = nextCharging;
        std::wstring next =
            DeviceName() + L" · " +
            (!connected
                 ? std::wstring(Tr(L"Disconnesso", L"Disconnected"))
                 : charge < 0
                       ? std::wstring(Tr(L"Batteria —", L"Battery —"))
                       : std::to_wstring(charge) + L"%" +
                             (charging ? Tr(L" · In carica", L" · Charging") : L""));
        NativePhone::RunFromWindowThread(bar, NativePhone::Tick, nullptr);
        if (next != label) {
            label = next;
            SetWindowTextW(w, label.c_str());
            InvalidateRect(w, nullptr, FALSE);
            if (panel) InvalidateRect(panel, nullptr, FALSE);
        }
        return 0;
    }
    case WM_CLOSE:
        DestroyWindow(w);
        return 0;
    case WM_DESTROY:
        KillTimer(w, 1);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(w, m, wp, lp);
}

static DWORD WINAPI Run(void*) {
    HINSTANCE instance = nullptr;
    GetModuleHandleExW(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
            GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        (LPCWSTR)&Proc, &instance);
    MSG initial{};
    PeekMessageW(&initial, nullptr, WM_USER, WM_USER, PM_NOREMOVE);
    SetEvent(readyEvent);

    HWND bar = nullptr;
    for (int attempt = 0; attempt < 240 && !NativePhone::stopping; ++attempt) {
        bar = FindWindowW(L"Shell_TrayWnd", nullptr);
        if (bar) break;
        Sleep(500);
    }
    if (!bar || NativePhone::stopping) return 1;
    DWORD owner = 0;
    GetWindowThreadProcessId(bar, &owner);
    if (owner != GetCurrentProcessId()) return 1;
    Wh_SetIntValue(L"taskbarPID", GetCurrentProcessId());
    if (connectionStop)
        connectionThread = CreateThread(nullptr, 0, CheckConnection, nullptr, 0, nullptr);

    WNDCLASSW wc{};
    wc.lpfnWndProc = Proc;
    wc.hInstance = instance;
    wc.lpszClassName = cls;
    wc.hCursor = LoadCursorW(nullptr, IDC_HAND);
    RegisterClassW(&wc);
    widget = CreateWindowExW(0, cls, L"KDE Connect", 0, 0, 0, 0, 0,
                            HWND_MESSAGE, nullptr, instance, nullptr);
    Gdiplus::GdiplusStartupInput graphicsInput;
    Gdiplus::GdiplusStartup(&graphicsToken, &graphicsInput, nullptr);
    RefreshTheme();

    if (widget) {
        SetTimer(widget, 1, 2000, nullptr);
        SendMessageW(widget, WM_TIMER, 1, 0);
        MSG msg;
        while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    if (widget && IsWindow(widget)) DestroyWindow(widget);
    widget = nullptr;
    if (panel) {
        DestroyWindow(panel);
        panel = nullptr;
    }
    iconCache.clear();
    if (graphicsToken) {
        Gdiplus::GdiplusShutdown(graphicsToken);
        graphicsToken = 0;
    }
    UnregisterClassW(L"KDEConnectIntegration.Panel.011", instance);
    UnregisterClassW(cls, instance);
    return 0;
}

BOOL Wh_ModInit() {
    LoadDevicePreferences();
    NativePhone::stopping = false;
    HWND bar = FindWindowW(L"Shell_TrayWnd", nullptr);
    DWORD owner = 0;
    if (bar) {
        GetWindowThreadProcessId(bar, &owner);
        if (owner != GetCurrentProcessId()) return FALSE;
    }
    if (!NativePhone::Symbols()) return FALSE;
    connectionWake = CreateEventW(nullptr, FALSE, FALSE, nullptr);
    connectionStop = CreateEventW(nullptr, TRUE, FALSE, nullptr);
    readyEvent = CreateEventW(nullptr, TRUE, FALSE, nullptr);
    if (!readyEvent) return FALSE;
    threadHandle = CreateThread(nullptr, 0, Run, nullptr, 0, &threadId);
    if (!threadHandle) {
        CloseHandle(readyEvent);
        readyEvent = nullptr;
        return FALSE;
    }
    WaitForSingleObject(readyEvent, 5000);
    Wh_SetIntValue(L"taskbarPID", GetCurrentProcessId());
    return TRUE;
}

void Wh_ModUninit() {
    NativePhone::stopping = true;
    if (connectionStop) SetEvent(connectionStop);
    if (connectionThread) {
        WaitForSingleObject(connectionThread, INFINITE);
        CloseHandle(connectionThread);
        connectionThread = nullptr;
    }
    if (connectionStop) {
        CloseHandle(connectionStop);
        connectionStop = nullptr;
    }
    if (connectionWake) {
        CloseHandle(connectionWake);
        connectionWake = nullptr;
    }
    HWND bar = FindWindowW(L"Shell_TrayWnd", nullptr);
    if (bar)
        NativePhone::RunFromWindowThread(
            bar, [](void*) { NativePhone::Detach(); }, nullptr);
    if (threadId) PostThreadMessageW(threadId, WM_QUIT, 0, 0);
    if (threadHandle) {
        WaitForSingleObject(threadHandle, INFINITE);
        CloseHandle(threadHandle);
        threadHandle = nullptr;
    }
    if (widget && IsWindow(widget)) DestroyWindow(widget);
    widget = nullptr;
    if (readyEvent) {
        CloseHandle(readyEvent);
        readyEvent = nullptr;
    }
    threadId = 0;
}
