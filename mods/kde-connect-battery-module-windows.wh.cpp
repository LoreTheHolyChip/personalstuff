// ==WindhawkMod==
// @id              kde-connect-battery-module-windows
// @name            KDE Connect Battery Module
// @description     Adds battery reception to KDE Connect on Windows for KDE Connect Integration
// @version         0.1.0
// @author          LTHC
// @include         kdeconnectd.exe
// @architecture    x86-64
// @compilerOptions -ladvapi32 -lversion
// ==/WindhawkMod==

// ==WindhawkModReadme==
/*
# KDE Connect Battery for Windows
Receives the phone's existing KDE Connect battery packets, without ADB.
Adds the incoming battery capability to KDE Connect identity packets.
Records only device ID, battery percentage, charging state and last update.
Designed for the installed Qt 6.11.1 x64 ABI; refuses other Qt versions.
This adapter supplies the Windhawk taskbar integration. It does not install
a native KDE plugin or add a battery display to KDE's own app/tray.
Reconnection is required after enabling/disabling to refresh capabilities.
*/
// ==/WindhawkModReadme==

#include <windows.h>
#include <string>
#include <cstring>
#include <atomic>
#include <vector>
#include "kde-integration-json.hpp"

using Json = nlohmann::json;

struct QtArray { void* data; void* ptr; long long size; };
static_assert(sizeof(QtArray) == 24);
using SerializeFn = QtArray* (*)(const void*, QtArray*);
using ReceiveFn = void (*)(void*, const void*);
using IdFn = QtArray* (*)(const void*, QtArray*);
using ReplaceFn = void* (*)(QtArray*, long long, long long, const char*, long long);
using DestroyFn = void (*)(QtArray*);

static SerializeFn originalSerialize = nullptr;
static ReceiveFn originalReceive = nullptr;
static IdFn getId = nullptr;
static ReplaceFn replaceBytes = nullptr;
static DestroyFn destroyBytes = nullptr, destroyString = nullptr;
static std::atomic<unsigned> identities{0}, updates{0}, packets{0};

static bool TypeIs(const void* packet, const wchar_t* expected) {
    QtArray text{};
    SIZE_T read = 0;
    if (!ReadProcessMemory(GetCurrentProcess(), packet, &text, sizeof(text), &read) ||
        read != sizeof(text)) return false;
    size_t length = wcslen(expected);
    if (text.size != static_cast<long long>(length) || length >= 64 || !text.ptr) return false;
    wchar_t buffer[64]{};
    return ReadProcessMemory(GetCurrentProcess(), text.ptr, buffer, length * 2, &read) &&
           read == length * 2 && !memcmp(buffer, expected, length * 2);
}

static bool PatchIdentity(std::string& payload) {
    auto json = Json::parse(payload, nullptr, false);
    if (json.is_discarded() || !json.is_object() ||
        json.value("type", std::string()) != "kdeconnect.identity") return false;
    auto body = json.find("body");
    if (body == json.end() || !body->is_object()) return false;
    auto list = body->find("incomingCapabilities");
    if (list == body->end() || !list->is_array()) return false;
    for (const auto& value : *list)
        if (value.is_string() && value == "kdeconnect.battery") return false;
    list->push_back("kdeconnect.battery");
    payload = json.dump() + "\n";
    return true;
}

static QtArray* SerializeHook(const void* packet, QtArray* result) {
    auto returned = originalSerialize(packet, result);
    if (!TypeIs(packet, L"kdeconnect.identity") || !result->ptr ||
        result->size <= 0 || result->size > 262144) return returned;
    try {
        std::string payload(static_cast<const char*>(result->ptr),
                            size_t(result->size));
        if (PatchIdentity(payload)) {
            replaceBytes(result, 0, result->size, payload.data(),
                         static_cast<long long>(payload.size()));
            Wh_SetIntValue(L"identitiesPatched", ++identities);
        }
    } catch (...) {
        Wh_Log(L"Identity left unchanged after parsing failure");
    }
    return returned;
}

static bool BatteryValues(const std::string& payload, int& charge, bool& charging) {
    auto json = Json::parse(payload, nullptr, false);
    if (json.is_discarded() || !json.is_object() ||
        json.value("type", std::string()) != "kdeconnect.battery") return false;
    auto body = json.find("body");
    if (body == json.end() || !body->is_object()) return false;
    auto value = body->find("currentCharge"), plugged = body->find("isCharging");
    if (value == body->end() || !value->is_number_integer() ||
        plugged == body->end() || !plugged->is_boolean()) return false;
    long long n = value->get<long long>();
    if (n < 0 || n > 100) return false;
    charge = int(n);
    charging = plugged->get<bool>();
    return true;
}

static void ReceiveHook(void* device, const void* packet) {
    unsigned seen = ++packets;
    if (seen == 1 || seen % 32 == 0) Wh_SetIntValue(L"packetsSeen", seen);
    if (TypeIs(packet, L"kdeconnect.battery")) {
        QtArray bytes{}, id{};
        try {
            originalSerialize(packet, &bytes);
            int charge = 0;
            bool charging = false;
            if (bytes.ptr && bytes.size > 0 && bytes.size < 65536 &&
                BatteryValues(std::string(static_cast<const char*>(bytes.ptr),
                                          size_t(bytes.size)), charge, charging)) {
                getId(device, &id);
                if (id.ptr && id.size > 0 && id.size <= 128) {
                    std::wstring name(static_cast<const wchar_t*>(id.ptr), size_t(id.size));
                    bool valid = true;
                    for (wchar_t ch : name)
                        if (!((ch >= L'0' && ch <= L'9') ||
                              (ch >= L'a' && ch <= L'z') ||
                              (ch >= L'A' && ch <= L'Z') || ch == L'_')) valid = false;
                    if (valid) {
                        HKEY key = nullptr;
                        std::wstring path = L"Software\\KDEConnectIntegration\\Battery\\" + name;
                        if (RegCreateKeyExW(HKEY_CURRENT_USER, path.c_str(), 0, nullptr, 0,
                                            KEY_SET_VALUE, nullptr, &key, nullptr) == ERROR_SUCCESS) {
                            DWORD percent = charge, plugged = charging;
                            FILETIME now{};
                            GetSystemTimeAsFileTime(&now);
                            ULARGE_INTEGER stamp{};
                            stamp.LowPart = now.dwLowDateTime;
                            stamp.HighPart = now.dwHighDateTime;
                            RegSetValueExW(key, L"Charge", 0, REG_DWORD,
                                           (BYTE*)&percent, sizeof(percent));
                            RegSetValueExW(key, L"Charging", 0, REG_DWORD,
                                           (BYTE*)&plugged, sizeof(plugged));
                            RegSetValueExW(key, L"UpdatedFileTime", 0, REG_QWORD,
                                           (BYTE*)&stamp.QuadPart, sizeof(stamp.QuadPart));
                            RegCloseKey(key);
                            Wh_SetIntValue(L"batteryUpdates", ++updates);
                        }
                    }
                }
            }
        } catch (...) {
            Wh_Log(L"Battery packet ignored after validation failure");
        }
        destroyString(&id);
        destroyBytes(&bytes);
    }
    originalReceive(device, packet);
}

static bool SupportedQt(HMODULE qt) {
    wchar_t path[MAX_PATH]{};
    if (!GetModuleFileNameW(qt, path, MAX_PATH)) return false;
    DWORD ignored = 0, size = GetFileVersionInfoSizeW(path, &ignored);
    if (!size) return false;
    std::vector<BYTE> info(size);
    if (!GetFileVersionInfoW(path, 0, size, info.data())) return false;
    VS_FIXEDFILEINFO* version = nullptr;
    UINT count = 0;
    return VerQueryValueW(info.data(), L"\\", (void**)&version, &count) &&
           count >= sizeof(*version) &&
           HIWORD(version->dwFileVersionMS) == 6 &&
           LOWORD(version->dwFileVersionMS) == 11 &&
           HIWORD(version->dwFileVersionLS) == 1;
}

static void* Implementation(void* address) {
    auto p = static_cast<BYTE*>(address);
    for (int i = 0; i < 8; i++) {
        BYTE bytes[5]{};
        SIZE_T read = 0;
        if (!ReadProcessMemory(GetCurrentProcess(), p, bytes, 5, &read) ||
            read != 5 || bytes[0] != 0xE9) break;
        int displacement = 0;
        memcpy(&displacement, bytes + 1, 4);
        p += 5 + displacement;
    }
    return p;
}

BOOL Wh_ModInit() {
    Wh_SetIntValue(L"adapterPID", GetCurrentProcessId());
    Wh_SetIntValue(L"adapterReady", 0);
    Wh_SetIntValue(L"initStage", 1);
    HMODULE core = GetModuleHandleW(L"kdeconnectcore.dll");
    HMODULE qt = GetModuleHandleW(L"Qt6Core.dll");
    if (!core || !qt || !SupportedQt(qt)) {
        Wh_SetIntValue(L"initStage", 2);
        Wh_Log(L"Unsupported or missing Qt runtime; no hooks installed");
        return FALSE;
    }
    getId = (IdFn)GetProcAddress(core, "?id@Device@@QEBA?AVQString@@XZ");
    replaceBytes = (ReplaceFn)GetProcAddress(qt, "?replace@QByteArray@@QEAAAEAV1@_J0PEBD0@Z");
    destroyBytes = (DestroyFn)GetProcAddress(qt, "??1QByteArray@@QEAA@XZ");
    destroyString = (DestroyFn)GetProcAddress(qt, "??1QString@@QEAA@XZ");
    auto serialize = GetProcAddress(core, "?serialize@NetworkPacket@@QEBA?AVQByteArray@@XZ");
    auto receive = GetProcAddress(core, "?privateReceivedPacket@Device@@AEAAXAEBVNetworkPacket@@@Z");
    if (!getId || !replaceBytes || !destroyBytes || !destroyString || !serialize || !receive) {
        Wh_SetIntValue(L"initStage", 3);
        return FALSE;
    }
    if (!Wh_SetFunctionHook(Implementation((void*)serialize),
                            (void*)SerializeHook, (void**)&originalSerialize)) {
        Wh_SetIntValue(L"initStage", 4);
        return FALSE;
    }
    if (!Wh_SetFunctionHook(Implementation((void*)receive),
                            (void*)ReceiveHook, (void**)&originalReceive)) {
        Wh_SetIntValue(L"initStage", 5);
        return FALSE;
    }
    Wh_SetIntValue(L"adapterPID", GetCurrentProcessId());
    Wh_SetIntValue(L"identitiesPatched", 0);
    Wh_SetIntValue(L"batteryUpdates", 0);
    Wh_SetIntValue(L"packetsSeen", 0);
    Wh_SetIntValue(L"initStage", 6);
    Wh_SetIntValue(L"adapterReady", 1);
    return TRUE;
}

void Wh_ModUninit() {
    Wh_SetIntValue(L"adapterReady", 0);
}
