// ==WindhawkMod==
// @id              fluent-clock
// @name            Fluent Clock
// @description     Makes the time on the clock smoother
// @description:it Rende più fluido l’orologio della barra delle applicazioni
// @description:it-IT Rende più fluido l’orologio della barra delle applicazioni
// @version         0.1.43
// @author          LTHC
// @include         explorer.exe
// @architecture    x86-64
// @compilerOptions -lole32 -loleaut32 -lruntimeobject -luuid
// ==/WindhawkMod==

// ==WindhawkModReadme==
/*
# Fluent Clock
## Italiano

Anima le cifre dell’orologio senza modificarne il formato. Supporta l’allineamento di Taskbar Clock Customization e offre controlli per movimento, scala, sfocatura e una base acrylic opzionale. Non supporta i formati dell’ora su più righe. Le impostazioni seguono la lingua di Windhawk: scegli Italiano nelle impostazioni di Windhawk se necessario.

## English
Experimental Windows 11 XAML animation mod. Compile and enable as a separate mod.
Observes the existing TimeInnerTextBlock: Taskbar Clock Customization remains responsible for formatting, weather, seconds, metrics, fonts and colors. Only changed characters animate; the date and tooltip are untouched. Multi-line custom time formats are not supported in this first version. The native clock remains the accessible/clickable control.
Respects the Windows animation preference. Windows 10 classic taskbars, third-party replacement taskbars and Microsoft.UI.Xaml/WinUI 3 hosts are not supported. Runtime compatibility must be tested on the target Windows build.
Uses direct control hooks without occupying the exclusive XAML diagnostics connection. Runtime compatibility is experimental. Code: GPL-3.0-or-later. Original reference: m417z, Windows 11 Start Menu Styler, https://github.com/ramensoftware/windhawk-mods/blob/main/mods/windows-11-start-menu-styler.wh.cpp
*/
// ==/WindhawkModReadme==

// ==WindhawkModSettings==
/*
- duration: 320
  $name: Animation duration
  $name:it: "Durata dell’animazione"
  $name:it-IT: "Durata dell’animazione"
  $description: Duration in milliseconds (100-1000).
  $description:it: "Durata in millisecondi (100-1000)."
  $description:it-IT: "Durata in millisecondi (100-1000)."
- blur: true
  $name: Soft glyph blur
  $name:it: "Sfocatura morbida del testo"
  $name:it-IT: "Sfocatura morbida del testo"
  $description: Briefly soften incoming characters using a fading alpha-mask shadow.
  $description:it: "Ammorbidisce brevemente i caratteri in entrata con un’ombra che sfuma."
  $description:it-IT: "Ammorbidisce brevemente i caratteri in entrata con un’ombra che sfuma."
- travel: 12
  $name: Rolling distance
  $name:it: "Distanza di scorrimento"
  $name:it-IT: "Distanza di scorrimento"
  $description: Vertical movement in pixels (0-40).
  $description:it: "Movimento verticale in pixel (0-40)."
  $description:it-IT: "Movimento verticale in pixel (0-40)."
- startScale: 78
  $name: Incoming and outgoing scale
  $name:it: "Dimensione iniziale e finale"
  $name:it-IT: "Dimensione iniziale e finale"
  $description: Percentage of the normal digit size (50-100).
  $description:it: "Percentuale della dimensione normale delle cifre (50-100)."
  $description:it-IT: "Percentuale della dimensione normale delle cifre (50-100)."
- blurRadius: 5
  $name: Soft blur radius
  $name:it: "Intensità della sfocatura"
  $name:it-IT: "Intensità della sfocatura"
  $description: Glyph shadow radius (0-20). Requires Soft glyph blur.
  $description:it: "Raggio dell’ombra dei caratteri (0-20). Richiede la sfocatura morbida del testo."
  $description:it-IT: "Raggio dell’ombra dei caratteri (0-20). Richiede la sfocatura morbida del testo."
- liquidMotion: true
  $name: Liquid motion
  $name:it: "Movimento liquido"
  $name:it-IT: "Movimento liquido"
  $description: Add a gentle elastic stretch and settling movement.
  $description:it: "Aggiunge una lieve deformazione elastica e un movimento di assestamento."
  $description:it-IT: "Aggiunge una lieve deformazione elastica e un movimento di assestamento."
- glassPanel: false
  $name: Acrylic glass base
  $name:it: "Base in vetro acrylic"
  $name:it-IT: "Base in vetro acrylic"
  $description: Optional rounded acrylic base behind the time. Uses Windows backdrop blur, with a solid fallback.
  $description:it: "Base acrylic arrotondata dietro l’ora. Sfoca lo sfondo; se l’effetto non è disponibile, usa un colore pieno."
  $description:it-IT: "Base acrylic arrotondata dietro l’ora. Sfoca lo sfondo; se l’effetto non è disponibile, usa un colore pieno."
- glassOpacity: 15
  $name: Glass tint opacity
  $name:it: "Opacità della tinta del vetro"
  $name:it-IT: "Opacità della tinta del vetro"
  $description: Tint percentage (0-80). Lower values reveal more backdrop.
  $description:it: "Percentuale della tinta (0-80). Valori bassi lasciano vedere maggiormente lo sfondo."
  $description:it-IT: "Percentuale della tinta (0-80). Valori bassi lasciano vedere maggiormente lo sfondo."
- glassRadius: 10
  $name: Glass corner radius
  $name:it: "Arrotondamento del vetro"
  $name:it-IT: "Arrotondamento del vetro"
  $description: Rounded base corners in pixels (0-30).
  $description:it: "Raggio degli angoli della base in pixel (0-30)."
  $description:it-IT: "Raggio degli angoli della base in pixel (0-30)."
- respectAnimations: true
  $name: Respect Windows animation settings
  $name:it: "Rispetta le animazioni di Windows"
  $name:it-IT: "Rispetta le animazioni di Windows"
  $description: Skip animations when Windows animations are disabled.
  $description:it: "Disattiva gli effetti quando le animazioni di Windows sono disabilitate."
  $description:it-IT: "Disattiva gli effetti quando le animazioni di Windows sono disabilitate."
*/
// ==/WindhawkModSettings==
#define FLUENT_CLOCK 1
// Direct control attachment: does not claim the exclusive XAML diagnostics slot.
// Reference: m417z's Windows 11 Start Menu Styler (GPL-3.0).
// This source is distributed under GPL-3.0-or-later.
#include <windows.h>
#include <winstring.h>
#undef GetCurrentTime
#include <ocidl.h>
#include <xamlom.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Windows.UI.h>
#include <winrt/Windows.Data.Json.h>
#include <winrt/Windows.UI.Xaml.Media.Imaging.h>
#include <winrt/Windows.UI.Text.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Windows.UI.Xaml.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <winrt/Windows.UI.Xaml.Documents.h>
#include <winrt/Windows.UI.Xaml.Hosting.h>
#include <winrt/Windows.UI.Xaml.Input.h>
#include <winrt/Windows.UI.Xaml.Media.h>
#include <algorithm>
#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <unordered_set>
namespace x=winrt::Windows::UI::Xaml;
namespace c=x::Controls;
namespace comp=winrt::Windows::UI::Composition;
namespace num=winrt::Windows::Foundation::Numerics;
using winrt::Windows::Foundation::TimeSpan;
std::atomic<bool> stopping=false;
std::atomic<int> pendingCallbacks=0;
void RetainModule() {HMODULE pinned=nullptr;GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS|GET_MODULE_HANDLE_EX_FLAG_PIN,reinterpret_cast<LPCWSTR>(&RetainModule),&pinned); }
int durationMs=320;
bool blurEnabled=true,respectAnimations=true;
float travel=12.f,startScale=.78f,blurRadius=5.f;
bool liquidMotion=true,glassPanel=false;
float glassOpacity=.15f,glassRadius=10.f;
bool enterFromBelow=false,animatePaste=false;
int letterDelay=18,maxAnimatedLetters=24;
HMODULE SelfModule() {
    HMODULE module=nullptr;
    GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS|GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        reinterpret_cast<LPCWSTR>(&SelfModule),&module);
    return module;
}
void LoadOptions() {
    durationMs=std::clamp(Wh_GetIntSetting(L"duration"),100,1000);
    blurEnabled=Wh_GetIntSetting(L"blur")!=0;
    respectAnimations=Wh_GetIntSetting(L"respectAnimations")!=0;
    travel=(float)std::clamp(Wh_GetIntSetting(L"travel"),0,40);
    startScale=std::clamp(Wh_GetIntSetting(L"startScale"),50,100)/100.f;
    blurRadius=(float)std::clamp(Wh_GetIntSetting(L"blurRadius"),0,20);
    liquidMotion=Wh_GetIntSetting(L"liquidMotion")!=0;
#if FLUENT_CLOCK
    glassPanel=Wh_GetIntSetting(L"glassPanel")!=0;
    glassOpacity=std::clamp(Wh_GetIntSetting(L"glassOpacity"),0,80)/100.f;
    glassRadius=(float)std::clamp(Wh_GetIntSetting(L"glassRadius"),0,30);
#else
    enterFromBelow=Wh_GetIntSetting(L"enterFromBelow")!=0;
    letterDelay=std::clamp(Wh_GetIntSetting(L"letterDelay"),0,60);
    animatePaste=Wh_GetIntSetting(L"animatePaste")!=0;
    maxAnimatedLetters=std::clamp(Wh_GetIntSetting(L"maxAnimatedLetters"),1,32);
#endif
}
bool AnimateEnabled() {
    try {return !respectAnimations || winrt::Windows::UI::ViewManagement::UISettings().AnimationsEnabled();}
    catch(...) {return false;}
}
void AnimateVisual(x::UIElement const& element,bool outgoing=false,int delay=0) {
    auto visual=x::Hosting::ElementCompositionPreview::GetElementVisual(element);
    auto compositor=visual.Compositor();
    x::Hosting::ElementCompositionPreview::SetIsTranslationEnabled(element,true);
    auto ease=compositor.CreateCubicBezierEasingFunction({.16f,1.f},{.3f,1.f});
    auto move=compositor.CreateVector3KeyFrameAnimation();
    move.Duration(TimeSpan{durationMs*10000LL});
    move.DelayTime(TimeSpan{delay*10000LL});
    move.InsertKeyFrame(0,num::float3{0,outgoing ? 0.f : (enterFromBelow ? travel : -travel),0});
    if(liquidMotion&&!outgoing) move.InsertKeyFrame(.7f,num::float3{0,travel*.1f,0},ease);
    move.InsertKeyFrame(1,num::float3{0,outgoing ? travel : 0.f,0},ease);
    visual.StartAnimation(L"Translation",move);
    auto scale=compositor.CreateVector3KeyFrameAnimation();scale.Duration(move.Duration());scale.DelayTime(move.DelayTime());
    scale.InsertKeyFrame(0,num::float3{outgoing ? 1.f : startScale,outgoing ? 1.f : startScale,1});
    if(liquidMotion&&!outgoing) scale.InsertKeyFrame(.7f,num::float3{1.025f,1.055f,1},ease);
    scale.InsertKeyFrame(1,num::float3{outgoing ? startScale : 1.f,outgoing ? startScale : 1.f,1},ease);
    visual.CenterPoint({visual.Size().x*.5f,visual.Size().y*.5f,0});
    visual.StartAnimation(L"Scale",scale);
    auto opacity=compositor.CreateScalarKeyFrameAnimation();opacity.Duration(move.Duration());opacity.DelayTime(move.DelayTime());
    opacity.InsertKeyFrame(0,outgoing ? 1.f : 0.f);
    opacity.InsertKeyFrame(1,outgoing ? 0.f : 1.f,ease);
    visual.StartAnimation(L"Opacity",opacity);
}
void ResetVisual(x::UIElement const& element) {
    auto visual=x::Hosting::ElementCompositionPreview::GetElementVisual(element);
    for(auto name:{L"Translation",L"Scale",L"Opacity"}) visual.StopAnimation(name);
    visual.Properties().InsertVector3(L"Translation",{0,0,0});
    visual.Scale({1,1,1});visual.Opacity(1);
}
void ResetSearchText(x::UIElement const& element) {
    // The internal editor only receives an opacity animation. Translation and
    // scale belong to our own glyphs and are unsupported on some editor visuals.
    auto visual=x::Hosting::ElementCompositionPreview::GetElementVisual(element);
    visual.StopAnimation(L"Opacity");
    visual.Opacity(1);
}
// An alpha-mask shadow supplies a short, zero-offset blurred glyph image.
// The sharp text fades in above it. No screen capture or per-frame CPU timer.
void BlurGlyph(c::TextBlock const& glyph,c::Canvas const& cell,int delay=0,float radiusOverride=-1) {
    if(!blurEnabled) return;
    auto solid=glyph.Foreground().try_as<x::Media::SolidColorBrush>();if(!solid) return;
    glyph.Measure({10000,10000});glyph.Arrange({0,0,glyph.DesiredSize().Width,glyph.DesiredSize().Height});
    auto visual=x::Hosting::ElementCompositionPreview::GetElementVisual(cell);
    const float radius=radiusOverride>=0 ? radiusOverride : blurRadius;
    auto compositor=visual.Compositor();auto shadow=compositor.CreateDropShadow();
    shadow.Mask(glyph.GetAlphaMask());shadow.Color(solid.Color());shadow.Offset({0,0,0});shadow.BlurRadius(radius);shadow.Opacity(delay ? 0.f : .7f);
    auto sprite=compositor.CreateSpriteVisual();sprite.Size({glyph.DesiredSize().Width,glyph.DesiredSize().Height});sprite.Shadow(shadow);
    x::Hosting::ElementCompositionPreview::SetElementChildVisual(cell,sprite);
    auto blur=compositor.CreateScalarKeyFrameAnimation();blur.Duration(TimeSpan{durationMs*10000LL});blur.DelayTime(TimeSpan{delay*10000LL});
    blur.InsertKeyFrame(0,radius);blur.InsertKeyFrame(1,0.f);shadow.StartAnimation(L"BlurRadius",blur);
    auto fade=compositor.CreateScalarKeyFrameAnimation();fade.Duration(blur.Duration());fade.DelayTime(blur.DelayTime());
    fade.DelayBehavior(winrt::Windows::UI::Composition::AnimationDelayBehavior::SetInitialValueAfterDelay);
    fade.InsertKeyFrame(0,.7f);fade.InsertKeyFrame(1,0.f);shadow.StartAnimation(L"Opacity",fade);
}

// Search content uses WebView2Standalone, as in the official Start Menu Styler.
constexpr winrt::guid IID_FluentWebView{0x59C47E46,0xCC96,0x525F,{0xA1,0x7C,0x2C,0x21,0x3F,0x98,0x84,0x47}};
struct WebView2Standalone_IWebView2 : ::IInspectable {
    virtual int32_t __stdcall get_CoreWebView2(void**) noexcept = 0;
    virtual int32_t __stdcall get_CoreWebView2Controller(void**) noexcept = 0;
    virtual int32_t __stdcall EnsureCoreWebView2Async(void**) noexcept = 0;
    virtual int32_t __stdcall ExecuteScriptAsync(void*, void**) noexcept = 0;
    virtual int32_t __stdcall get_Source(void**) noexcept = 0;
    virtual int32_t __stdcall put_Source(void*) noexcept = 0;
    virtual int32_t __stdcall get_CanGoForward(bool*) noexcept = 0;
    virtual int32_t __stdcall put_CanGoForward(bool) noexcept = 0;
    virtual int32_t __stdcall get_CanGoBack(bool*) noexcept = 0;
    virtual int32_t __stdcall put_CanGoBack(bool) noexcept = 0;
    virtual int32_t __stdcall Reload() noexcept = 0;
    virtual int32_t __stdcall GoForward() noexcept = 0;
    virtual int32_t __stdcall GoBack() noexcept = 0;
    virtual int32_t __stdcall NavigateToString(void*) noexcept = 0;
    virtual int32_t __stdcall Close() noexcept = 0;
    virtual int32_t __stdcall add_NavigationCompleted(void*, winrt::event_token*) noexcept = 0;
    virtual int32_t __stdcall remove_NavigationCompleted(winrt::event_token) noexcept = 0;
    virtual int32_t __stdcall add_WebMessageReceived(void*, winrt::event_token*) noexcept = 0;
    virtual int32_t __stdcall remove_WebMessageReceived(winrt::event_token) noexcept = 0;
    virtual int32_t __stdcall add_NavigationStarting(void*, winrt::event_token*) noexcept = 0;
    virtual int32_t __stdcall remove_NavigationStarting(winrt::event_token) noexcept = 0;
    virtual int32_t __stdcall add_CoreProcessFailed(void*, winrt::event_token*) noexcept = 0;
    virtual int32_t __stdcall remove_CoreProcessFailed(winrt::event_token) noexcept = 0;
    virtual int32_t __stdcall add_CoreWebView2Initialized(void*, winrt::event_token*) noexcept = 0;
    virtual int32_t __stdcall remove_CoreWebView2Initialized(winrt::event_token) noexcept = 0;
};


const wchar_t* SearchHomeScript=LR"FLUENT((() => {
 const key='__fluentSearchHome';
 if(window[key]) {window[key].apply();return;}
 const marked=new Set();const headingTabIndexes=new Map();
 let collapsed=false;try{collapsed=localStorage.getItem('fluent-search-preview-collapsed')==='1';}catch{}const animated=new WeakMap();const injected=new Set();
 const originals=new Map();
 const set=(el,name,value)=>{if(!originals.has(el))originals.set(el,new Map());const props=originals.get(el);if(!props.has(name))props.set(name,[el.style.getPropertyValue(name),el.style.getPropertyPriority(name)]);if(el.style.getPropertyValue(name)!==value||el.style.getPropertyPriority(name)!=='important')el.style.setProperty(name,value,'important');};
 const mark=(el,name)=>{if(el&&!el.hasAttribute(name)){el.setAttribute(name,'');marked.add(el);}};
 const clear=()=>{for(const [el,props]of originals)for(const [name,[value,priority]]of props)value?el.style.setProperty(name,value,priority):el.style.removeProperty(name);originals.clear();for(const el of marked)for(const a of [...el.attributes])if(a.name.startsWith('data-fluent-home-')||a.name.startsWith('data-fluent-results-')||a.name.startsWith('data-fluent-preview-')||a.name.startsWith('data-fluent-action'))el.removeAttribute(a.name);marked.clear();};
 const style=document.createElement('style');style.id='fluent-search-home-style';
 style.textContent=`
 .fluent-preview-card{order:-20;flex:none;position:relative;width:100%;box-sizing:border-box;padding:12px;border-radius:12px;background:rgba(128,128,128,.12);overflow:hidden;color:inherit}
 .fluent-preview-card[hidden]{display:none!important}
 .fluent-preview-toggle{display:block;margin-left:auto;border:0;border-radius:7px;background:rgba(128,128,128,.12);color:inherit;width:30px;height:28px;font-family:'Segoe Fluent Icons','Segoe MDL2 Assets';cursor:pointer}
 .fluent-preview-body{display:flex;align-items:center;gap:16px;padding:8px 4px 16px}
 .fluent-preview-icon{display:flex;align-items:center;justify-content:center;flex:none;width:76px;height:76px;border-radius:12px;background:rgba(128,128,128,.14)}
 .fluent-preview-icon img,.fluent-preview-icon .icon,.fluent-preview-icon .iconContent{position:static!important;width:52px!important;height:52px!important;object-fit:contain!important;margin:0!important}
 .fluent-preview-name{font-size:18px;font-weight:600;line-height:24px}.fluent-preview-desc{font-size:12px;opacity:.72;margin-top:6px}
 .fluent-preview-actions{display:flex;gap:6px;justify-content:center;flex-wrap:wrap}
 .fluent-preview-actions button{display:flex;flex:1 1 70px;max-width:110px;min-width:0;flex-direction:column;align-items:center;justify-content:center;gap:8px;padding:10px 4px;border:0;border-radius:8px;background:rgba(128,128,128,.14);color:inherit;cursor:pointer;transition:background 140ms,transform 140ms}
 .fluent-preview-actions button:hover{background:rgba(128,128,128,.26)}.fluent-preview-actions button:active{transform:scale(.94)}.fluent-preview-actions button:disabled{opacity:.35;cursor:default}
 .fluent-preview-glyph{font-family:'Segoe Fluent Icons','Segoe MDL2 Assets';font-size:18px;line-height:22px}.fluent-preview-caption{font-size:11px;line-height:14px;text-align:center}
 .fluent-preview-card.is-collapsed{display:flex;gap:8px;align-items:center}.is-collapsed .fluent-preview-toggle{order:2;margin-left:0;flex:none}.is-collapsed .fluent-preview-body,.is-collapsed .fluent-preview-caption{display:none!important}.is-collapsed .fluent-preview-actions{margin-left:auto;justify-content:flex-end;flex-wrap:wrap}.is-collapsed .fluent-preview-actions button{flex:none;width:36px;height:36px;padding:6px}
 .fluent-preview-extra{width:100%;margin-top:10px;box-sizing:border-box}.fluent-preview-extra[hidden]{display:none!important}.fluent-preview-extra summary{padding:10px 12px;border-radius:8px;background:rgba(128,128,128,.1);cursor:pointer;font-size:12px}.fluent-preview-extra-list{display:flex;flex-direction:column;gap:3px;padding-top:6px}.fluent-preview-extra-list button{display:flex;align-items:center;gap:12px;width:100%;min-height:38px;padding:8px 12px;border:0;border-radius:6px;background:transparent;color:inherit;text-align:left;cursor:pointer;transition:background 140ms}.fluent-preview-extra-list button:hover{background:rgba(128,128,128,.16)}.fluent-preview-extra-list .fluent-preview-caption{display:block!important;font-size:12px;text-align:left;overflow-wrap:anywhere}.fluent-preview-extra-list .fluent-preview-glyph{flex:none}.is-collapsed .fluent-preview-extra{display:none!important}.fluent-preview-extra[open] .fluent-preview-extra-list{animation:fluent-extra-in 180ms ease-out}@keyframes fluent-extra-in{from{opacity:0;transform:translateY(-5px)}to{opacity:1;transform:none}}
 .fluent-preview-card .fluent-preview-toggle{position:absolute;top:8px;right:8px;z-index:2;margin:0;opacity:0;pointer-events:none;transition:opacity 140ms ease}.fluent-preview-card:hover .fluent-preview-toggle,.fluent-preview-card:focus-within .fluent-preview-toggle{opacity:1;pointer-events:auto}.fluent-preview-card .fluent-preview-body{padding-top:0}.fluent-preview-card.is-collapsed{padding-right:48px}.fluent-preview-card.is-collapsed .fluent-preview-toggle{top:50%;transform:translateY(-50%)}@media(prefers-reduced-motion:reduce){.fluent-preview-card .fluent-preview-toggle{transition:none}}
 .fluent-preview-card .fluent-preview-toggle{display:flex;align-items:center;justify-content:center;padding:0;line-height:1;box-sizing:border-box}.fluent-preview-card.is-collapsed{padding-left:48px;padding-right:48px;justify-content:center}.fluent-preview-card.is-collapsed .fluent-preview-actions{margin-left:0;margin-right:0;justify-content:center}
 .fluent-preview-card{width:calc(100% - 28px);margin-left:14px;margin-right:14px;padding-left:16px;padding-right:16px}[data-fluent-results-root]>.groupContainer{order:0}[data-fluent-results-selected]{order:-30!important}
 .fluent-preview-card{order:initial;display:block;float:none;clear:both;margin-top:12px;margin-bottom:16px}.fluent-preview-card.is-collapsed{display:flex}
 
 .userProfileMenuIcon{visibility:hidden!important;pointer-events:none!important}
 [data-fluent-results-best-heading]{display:none!important}
 [data-fluent-preview-toggle]{font-family:'Segoe Fluent Icons','Segoe MDL2 Assets'!important;font-size:14px!important}
 [data-fluent-preview-header]{display:flex!important;align-items:center!important;gap:16px!important;padding:16px!important;width:100%!important;box-sizing:border-box!important}
 [data-fluent-preview-header] img,[data-fluent-preview-header] .icon,[data-fluent-preview-header] .iconContent{width:64px!important;height:64px!important;object-fit:contain!important;font-size:48px!important}
 [data-fluent-preview-name]{font-size:18px!important;font-weight:600!important;line-height:24px!important}
 [data-fluent-preview-description]{font-size:12px!important;opacity:.72!important;margin-top:5px!important}
 [data-fluent-results-details]{display:flex!important;flex-wrap:wrap!important;align-items:center!important;gap:6px!important}
 [data-fluent-results-details] *:not([data-fluent-action]):not([data-fluent-action] *):not([data-fluent-actions-parent]):not([data-fluent-preview-toggle]):not([data-fluent-preview-header]):not([data-fluent-preview-header] *){display:none!important}
 [data-fluent-results-details] [data-fluent-actions-parent]{display:contents!important}
 [data-fluent-results-details] [data-fluent-action]{display:inline-flex!important;flex-direction:column!important;align-items:center!important;justify-content:center!important;gap:7px!important;min-width:76px!important;max-width:96px!important;min-height:62px!important;padding:8px 4px!important;box-sizing:border-box!important;position:relative!important}
 [data-fluent-results-details] [data-fluent-action] .menu-item_details{display:none!important}
 [data-fluent-action-label]{font-size:11px!important;line-height:14px!important;text-align:center!important;white-space:normal!important}
 [data-fluent-preview-collapsed] [data-fluent-preview-header],[data-fluent-preview-collapsed] [data-fluent-action-label]{display:none!important}
 [data-fluent-preview-collapsed] [data-fluent-action]{min-height:36px!important;min-width:36px!important}
 [data-fluent-results-more]{display:none!important}
 [data-fluent-results-scopes]{position:fixed!important;bottom:0!important;left:0!important;right:0!important;z-index:30!important;padding:10px 18px!important;box-sizing:border-box!important;background:rgba(28,28,28,.88)!important;backdrop-filter:blur(20px)!important}
 [data-fluent-results-root]{padding-bottom:64px!important;overflow-y:auto!important}
 [data-fluent-results-selected]{order:-30!important;width:100%!important;max-width:none!important}
 [data-fluent-preview-toggle]{margin-left:auto!important;border:0!important;border-radius:8px!important;background:transparent!important;color:inherit!important;padding:6px 10px!important;cursor:pointer!important}
 [data-fluent-preview-collapsed]{display:flex!important;flex-direction:row!important;align-items:center!important;flex-wrap:wrap!important;gap:8px!important;overflow:hidden!important}
 [data-fluent-preview-collapsed] *:not([data-fluent-action]):not([data-fluent-action] *):not([data-fluent-actions-parent]):not([data-fluent-preview-toggle]){display:none!important}
 [data-fluent-preview-collapsed] [data-fluent-actions-parent]{display:contents!important}
 [data-fluent-preview-collapsed] [data-fluent-action]{display:inline-flex!important;width:36px!important;height:36px!important;min-width:0!important;padding:6px!important;align-items:center!important;justify-content:center!important;margin:0!important;box-sizing:border-box!important}
 [data-fluent-preview-collapsed] [data-fluent-action] .menu-item_details,[data-fluent-preview-collapsed] [data-fluent-action] .text,[data-fluent-preview-collapsed] [data-fluent-action] .title,[data-fluent-preview-collapsed] [data-fluent-action] .label{display:none!important}
 [data-fluent-results-details]{order:-20!important;width:100%!important;max-width:none!important;min-width:0!important;min-height:0!important;position:relative!important;inset:auto!important;box-sizing:border-box!important}
 [data-fluent-results-flatten]{display:contents!important}
 [data-fluent-home-quick]{display:none!important}
 [data-fluent-home-root]{display:flex!important;flex-direction:column!important;gap:16px!important}
 [data-fluent-home-flatten]{display:contents!important}
 [data-fluent-home-apps]{order:-1!important;width:100%!important;max-width:none!important;margin:0!important;float:none!important}
 [data-fluent-home-apps] .suggsList{display:grid!important;grid-template-columns:repeat(6,minmax(0,1fr))!important;gap:8px!important;width:100%!important}
 [data-fluent-home-apps] .suggestion{width:auto!important;min-width:0!important;margin:0!important;box-sizing:border-box!important}
 [data-fluent-home-apps] .suggestion .suggContainer{display:flex!important;flex-direction:column!important;align-items:center!important;justify-content:center!important;gap:6px!important}
 [data-fluent-home-apps] .suggestion .icon,[data-fluent-home-apps] .suggestion .iconContent{position:static!important;margin:0 auto!important;float:none!important}
 [data-fluent-home-apps] .suggestion .text{position:static!important;margin:0!important;padding:0!important;width:100%!important;text-align:center!important}
 [data-fluent-home-apps] .suggestion .title{display:block!important;position:static!important;font-size:11px!important;line-height:14px!important;text-align:center!important;margin:0!important;padding:0!important;overflow:hidden!important;text-overflow:ellipsis!important;white-space:nowrap!important;max-width:100%!important}
 [data-fluent-home-recent]{width:100%!important;max-width:none!important;float:none!important}
 [data-fluent-home-recent] .suggsList{display:grid!important;grid-template-columns:minmax(0,1fr) minmax(0,1fr)!important;column-gap:24px!important;row-gap:0!important;position:relative!important;width:100%!important;box-sizing:border-box!important}
 [data-fluent-home-recent] .suggsList::before{content:""!important;position:absolute!important;left:50%!important;top:0!important;bottom:0!important;width:1px!important;transform:translateX(-50%)!important;background:currentColor!important;opacity:.18!important;pointer-events:none!important}
 [data-fluent-home-recent] .suggestion{width:auto!important;min-width:0!important;margin:0!important;box-sizing:border-box!important}
 [data-fluent-home-recent] .suggestion .suggContainer{display:flex!important;flex-direction:row!important;align-items:center!important;gap:10px!important;min-width:0!important}
 [data-fluent-home-recent] .suggestion .title{white-space:nowrap!important;overflow:hidden!important;text-overflow:ellipsis!important}
 `;
 (document.head||document.documentElement).append(style);
 const region=(value)=>document.querySelector('[data-region="'+value+'"]');
 const section=(node)=>node&&(node.closest('.groupContainer')||node);
 const heading=(text)=>[...document.querySelectorAll('.groupTitle,h2,h3,h4,[role="heading"]')].find(n=>text.includes(n.textContent.trim().toLowerCase()));
 let previewCard=null;let chosenResult=null;let lastSelection=null;
 const currentResult=()=>{const active=document.activeElement?.closest('.suggestion');const marked=document.querySelector('.suggestion[aria-selected="true"],.suggestion[aria-current="true"],.suggestion.selected,.suggestion.focused');return active||marked||(chosenResult?.isConnected?chosenResult:null)||document.querySelector('.topResults .suggestion')||document.querySelector('.topResults');};
 const selectionChanged=event=>{const result=event.target.closest?.('.suggestion');if(result&&!result.closest('.fluent-preview-card'))chosenResult=result;requestAnimationFrame(()=>apply());};
 document.addEventListener('click',selectionChanged,true);document.addEventListener('focusin',selectionChanged,true);
 const tabNavigation=event=>{
  if(event.key!=='Tab'||event.altKey||event.metaKey)return;
  const selector=event.ctrlKey?'button,a[href],input,textarea,select,summary,[tabindex]':'.suggestion';
  const items=[...document.querySelectorAll(selector)].filter(node=>{
   if(node.closest('#qfPreviewPane')||node.matches(':disabled,[aria-disabled="true"]')||!node.getClientRects().length)return false;
   const css=getComputedStyle(node);if(css.visibility==='hidden'||css.display==='none')return false;
   if(event.ctrlKey){const saved=headingTabIndexes.get(node);return node.tabIndex>=0||(saved!==undefined&&saved!==null&&Number(saved)>=0);}
   if(node.closest('.groupTitle,[role="heading"],h2,h3,h4'))return false;
   const result=node.closest('.suggestion');if(result&&result!==node)return false;
   if(node.matches('.suggestion')&&node.parentElement.closest('.suggestion'))return false;
   return true;
  });
  if(!items.length)return;
  const active=document.activeElement;let index=items.findIndex(node=>node===active||node.contains(active));
  index=index<0?(event.shiftKey?items.length-1:0):(index+(event.shiftKey?-1:1)+items.length)%items.length;
  event.preventDefault();event.stopImmediatePropagation();const next=items[index];if(next.matches('.suggestion')&&!next.hasAttribute('tabindex')){if(!headingTabIndexes.has(next))headingTabIndexes.set(next,null);next.setAttribute('tabindex','0');}next.focus({preventScroll:true});
 };
 document.addEventListener('keydown',tabNavigation,true);
 const navigationChanged=event=>{if(['ArrowUp','ArrowDown','Home','End'].includes(event.key))requestAnimationFrame(()=>apply());};document.addEventListener('keydown',navigationChanged,true);
 const enhancePreview=(details,common)=>{
  const selected=currentResult();
  if(!previewCard||!previewCard.isConnected){previewCard=document.createElement('section');previewCard.className='fluent-preview-card';previewCard.innerHTML='<button type="button" class="fluent-preview-toggle"></button><div class="fluent-preview-body"><div class="fluent-preview-icon"></div><div><div class="fluent-preview-name"></div><div class="fluent-preview-desc"></div></div></div><div class="fluent-preview-actions"></div><details class="fluent-preview-extra"><summary>Azioni e file recenti</summary><div class="fluent-preview-extra-list"></div></details>';common.append(previewCard);injected.add(previewCard);
   previewCard.querySelector('button').onclick=()=>{const before=previewCard.getBoundingClientRect().height;collapsed=!collapsed;try{localStorage.setItem('fluent-search-preview-collapsed',collapsed?'1':'0');}catch{}updateCollapsed();const after=previewCard.getBoundingClientRect().height;if(!matchMedia('(prefers-reduced-motion: reduce)').matches){previewCard.getAnimations().forEach(a=>a.cancel());previewCard.animate([{height:before+'px'},{height:after+'px'}],{duration:260,easing:'cubic-bezier(.16,1,.3,1)'});if(!collapsed)previewCard.querySelector('.fluent-preview-body').animate([{opacity:0,transform:'translateY(-6px)'},{opacity:1,transform:'none'}],{duration:220});}};
  }
  if(previewCard.parentElement!==common)common.append(previewCard);
  previewCard.hidden=false;
  const name=selected?.querySelector('.primaryText,.title')?.textContent.trim()||selected?.querySelector('.suggDetailsContainer')?.textContent.trim()||'';
  const description=details.querySelector('.description,.appDescription')?.textContent.trim()||'Non ci sta una descrizione per questa app.';
  const title=previewCard.querySelector('.fluent-preview-name'),desc=previewCard.querySelector('.fluent-preview-desc');if(title.textContent!==name)title.textContent=name;if(desc.textContent!==description)desc.textContent=description;
  const icon=selected?.querySelector('img')||[...selected.querySelectorAll('.iconContent,.icon')].find(n=>!n.closest('button,.openPreviewPaneBtn')&&!n.classList.contains('openPreviewPaneBtn'));
  const iconBox=previewCard.querySelector('.fluent-preview-icon');const signature=name+'|'+(icon?.outerHTML||'');if(iconBox.dataset.signature!==signature){iconBox.dataset.signature=signature;iconBox.replaceChildren();if(icon){const clone=icon.cloneNode(true);clone.removeAttribute('id');clone.removeAttribute('style');clone.querySelectorAll('[id]').forEach(n=>n.removeAttribute('id'));iconBox.append(clone);}}
  const candidates=[...details.querySelectorAll('.menu-item,[role="menuitem"],[id^="pp_"]')];
  const actions=candidates.filter(n=>!candidates.some(parent=>parent!==n&&parent.contains(n)));
  const row=previewCard.querySelector('.fluent-preview-actions');
  const labels=actions.map(n=>n.getAttribute('aria-label')||n.querySelector('.menu-item_details')?.textContent.trim()||n.textContent.trim());
  const actionSignature=JSON.stringify(labels);const extras=previewCard.querySelector('.fluent-preview-extra-list');const isPrimary=label=>/^(apri|open)$|amministratore|administrator|altro utente|different user|percorso|file location|barra delle applicazioni|taskbar|start|disinstalla|uninstall/i.test(label);const actionButtons=[];
  if(row.dataset.signature!==actionSignature){row.dataset.signature=actionSignature;row.replaceChildren();extras.replaceChildren();labels.forEach((label,i)=>{let text=label,glyph='\uE8A7';if(/amministratore|administrator/i.test(label)){text='Apri con Admin';glyph='\uE83D';}else if(/altro utente|different user/i.test(label)){text='Altro utente';glyph='\uE77B';}else if(/percorso|file location/i.test(label)){text='Percorso';glyph='\uE8B7';}else if(/barra delle applicazioni|taskbar/i.test(label)){text='Fissa Taskbar';glyph='\uE718';}else if(/start/i.test(label)){text='Fissa Start';glyph='\uE718';}else if(/disinstalla|uninstall/i.test(label)){text='Rimuovi';glyph='\uE74D';}else if(/apri|open/i.test(label))text='Apri';
   const button=document.createElement('button');button.type='button';button.title=label;button.setAttribute('aria-label',label);const symbol=document.createElement('span');symbol.className='fluent-preview-glyph';symbol.textContent=glyph;const caption=document.createElement('span');caption.className='fluent-preview-caption';caption.textContent=text;button.append(symbol,caption);button.dataset.actionIndex=String(i);(isPrimary(label)?row:extras).append(button);});}
  [...row.children,...extras.children].forEach(button=>{const i=Number(button.dataset.actionIndex);button.disabled=actions[i]?.getAttribute('aria-disabled')==='true'||actions[i]?.hasAttribute('disabled');button.onclick=()=>actions[i]?.click();});
  previewCard.querySelector('.fluent-preview-extra').hidden=!extras.children.length;
  updateCollapsed();
 };
 const updateCollapsed=()=>{if(!previewCard)return;previewCard.classList.toggle('is-collapsed',collapsed);const toggle=previewCard.querySelector('.fluent-preview-toggle');const glyph=collapsed?'\uE70D':'\uE70E';if(toggle.textContent!==glyph)toggle.textContent=glyph;toggle.setAttribute('aria-expanded',String(!collapsed));toggle.title=collapsed?'Mostra dettagli':'Nascondi dettagli';};
 const animateResults=()=>{
  if(matchMedia('(prefers-reduced-motion: reduce)').matches)return;
  let index=0;
  for(const node of document.querySelectorAll('.suggestion')){
   if(!node.getClientRects().length||node.closest('[data-fluent-home-apps],[data-fluent-home-recent]'))continue;
   const signature=node.textContent.trim();if(animated.get(node)===signature)continue;animated.set(node,signature);
   node.animate([{opacity:0,transform:'translateY(8px) scale(.985)',filter:'blur(3px)'},{opacity:1,transform:'translateY(0) scale(1)',filter:'blur(0)'}],{duration:220,delay:Math.min(index++*24,160),easing:'cubic-bezier(.16,1,.3,1)',fill:'backwards'});
  }
 };
 const layoutResults=()=>{
  const scopes=document.querySelector('#scopesHeader');if(scopes)mark(scopes,'data-fluent-results-scopes');
  const details=document.querySelector('#qfPreviewPane');
  const selected=currentResult();
  if(previewCard)previewCard.hidden=true;
  if(!details||!selected||!selected.getClientRects().length||document.querySelector('#root.zeroInput19H1'))return;
  const top=selected;
  let common=top.parentElement;while(common&&!common.contains(details))common=common.parentElement;
  if(!common)return;
  mark(common,'data-fluent-results-root');mark(top,'data-fluent-results-selected');set(details,'display','none');enhancePreview(details,common);
  set(common,'display','flex');set(common,'flex-direction','column');set(common,'gap','12px');set(common,'align-items','stretch');
  set(top,'order','-30');set(top,'width','100%');set(top,'max-width','none');set(previewCard,'order','-20');
  for(const child of common.children){if(child===previewCard||child===details||child.contains(top))continue;set(child,'order','0');set(child,'flex-shrink','0');}
  for(const group of common.querySelectorAll('.groupContainer,.suggestion')){if(group===top||group.contains(top)||group.closest('.fluent-preview-card')||details.contains(group))continue;set(group,'order','0');set(group,'flex-shrink','0');}
  for(const [property,value]of Object.entries({'flex-shrink':'0',height:'auto','min-height':'64px','max-height':'none',overflow:'visible',position:'relative',inset:'auto','box-sizing':'border-box'}))set(top,property,value);
  for(const node of top.querySelectorAll('.suggContainer')){set(node,'min-height','60px');set(node,'height','auto');set(node,'position','relative');set(node,'inset','auto');}
  for(let n=top.parentElement;n&&n!==common;n=n.parentElement){mark(n,'data-fluent-results-flatten');set(n,'display','contents');}
  if(lastSelection&&lastSelection!==selected&&!matchMedia('(prefers-reduced-motion: reduce)').matches){previewCard.animate([{opacity:.3,filter:'blur(4px)'},{opacity:1,filter:'blur(0)'}],{duration:240});}lastSelection=selected;

 };
 const profileNode=()=>document.querySelector('.userProfileMenuIcon');
 const moreNode=()=>document.querySelector('#contextMenuButton,#moreOptionsButton,.settingsMenuIcon')||[...document.querySelectorAll('button,[role="button"]')].find(n=>/^(altre opzioni|altro|more options|settings and more|impostazioni e altro|opzioni di ricerca|search options)$/i.test(n.getAttribute('aria-label')||n.getAttribute('title')||''));
 window.__fluentSearchProfile=()=>{profileNode()?.click();};
 window.__fluentSearchMore=()=>{moreNode()?.click();};
 window.__fluentSearchAvatar=()=>{const n=profileNode();if(!n)return '';const img=n.querySelector('img');if(img)return img.currentSrc||img.src;const bg=getComputedStyle(n).backgroundImage;return bg.startsWith('url(')?bg.slice(4,-1).replace(/^['"]|['"]$/g,''):'';};
 const apply=()=>{
  observer.disconnect();
  try{
  clear();
  for(const title of document.querySelectorAll('.groupTitle,h2,h3,h4,[role="heading"]')){if(!headingTabIndexes.has(title))headingTabIndexes.set(title,title.getAttribute('tabindex'));if(title.getAttribute('tabindex')!=='-1')title.setAttribute('tabindex','-1');}
  const best=heading(['corrispondenza migliore','best match']);if(best)mark(best,'data-fluent-results-best-heading');
  const more=moreNode();if(more)mark(more,'data-fluent-results-more');layoutResults();animateResults();
  const quick=section(region('QuickActionList')||heading(['ricerche rapide','quick searches'])?.parentElement);
  if(quick)mark(quick,'data-fluent-home-quick');
  const apps=section(region('TopApps'));
  const recent=section(region('RecentSearches')||region('Recent')||region('History')||heading(['recenti','recent'])?.parentElement);
  if(!apps||!recent||apps===recent||!apps.getClientRects().length)return;
  let common=apps.parentElement;
  while(common&&!common.contains(recent))common=common.parentElement;
  if(!common||common===document.body||common===document.documentElement)return;
  mark(common,'data-fluent-home-root');mark(apps,'data-fluent-home-apps');mark(recent,'data-fluent-home-recent');
  set(common,'display','flex');set(common,'flex-direction','column');set(common,'gap','16px');
  set(apps,'order','-1000');set(apps,'width','100%');set(apps,'max-width','none');
  for(const title of apps.querySelectorAll('.title')){set(title,'font-size','11px');set(title,'line-height','14px');set(title,'text-align','center');}
  set(recent,'display','block');set(recent,'visibility','visible');set(recent,'order','0');set(recent,'width','100%');set(recent,'max-width','none');
  for(const list of recent.querySelectorAll('.suggsList')){set(list,'display','grid');set(list,'grid-template-columns','minmax(0,1fr) minmax(0,1fr)');set(list,'column-gap','24px');set(list,'row-gap','0');set(list,'position','relative');set(list,'width','100%');}
  for(let n=apps.parentElement;n&&n!==common;n=n.parentElement){mark(n,'data-fluent-home-flatten');set(n,'display','contents');}
  for(let n=recent.parentElement;n&&n!==common;n=n.parentElement){mark(n,'data-fluent-home-flatten');set(n,'display','contents');}
  }finally{observer.observe(document.documentElement,{childList:true,subtree:true});}
 };
 let queued=false;
 const observer=new MutationObserver(()=>{if(!queued){queued=true;requestAnimationFrame(()=>{queued=false;apply();});}});
 observer.observe(document.documentElement,{childList:true,subtree:true});
 window[key]={apply,dispose(){observer.disconnect();for(const [title,value]of headingTabIndexes)value===null?title.removeAttribute('tabindex'):title.setAttribute('tabindex',value);headingTabIndexes.clear();document.removeEventListener('click',selectionChanged,true);document.removeEventListener('focusin',selectionChanged,true);document.removeEventListener('keydown',navigationChanged,true);document.removeEventListener('keydown',tabNavigation,true);for(const node of injected)node.remove();clear();style.remove();delete window[key];delete window.__fluentSearchProfile;delete window.__fluentSearchMore;delete window.__fluentSearchAvatar;}};
 apply();return 'Fluent Search home layout installed';
})();
)FLUENT";
const wchar_t* SearchHomeClear=LR"FLUENT(window.__fluentSearchHome?.dispose();)FLUENT";

void InvokeSearchMenu(bool more);
struct Binding:std::enable_shared_from_this<Binding> {
    winrt::com_ptr<WebView2Standalone_IWebView2> layoutWeb;
    c::WebView layoutLegacy{nullptr};
    winrt::event_token layoutNavigation{};
    bool isLayout=false;
    void ApplyHome(bool clear=false) {
        if(!clear&&stopping)return;
        auto script=winrt::hstring(clear ? SearchHomeClear : SearchHomeScript);
        if(layoutWeb) {
            if(!clear) {
                void* ptr=nullptr;winrt::check_hresult(layoutWeb->get_Source(&ptr));
                winrt::Windows::Foundation::Uri uri{ptr,winrt::take_ownership_from_abi};
                if(!uri)return;
                auto url=uri.AbsoluteCanonicalUri();
                if(url!=L"https://www.bing.com/WS/Init"&&url!=L"https://searchapp.bundleassets.example/desktop/2.html")return;
            }
            void* ptr=nullptr;winrt::check_hresult(layoutWeb->ExecuteScriptAsync(winrt::get_abi(script),&ptr));
            winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> op{ptr,winrt::take_ownership_from_abi};
        } else if(layoutLegacy) {
            auto uri=layoutLegacy.Source();if(!uri)return;
            auto url=uri.AbsoluteCanonicalUri();
            if(url!=L"https://www.bing.com/WS/Init"&&url!=L"https://searchapp.bundleassets.example/desktop/2.html")return;
            layoutLegacy.InvokeScriptAsync(L"eval",winrt::single_threaded_vector<winrt::hstring>({script}));
        }
        Wh_SetStringValue(L"homeLayoutStatus",clear ? L"Layout removed" : L"Layout script dispatched");
    }
    x::FrameworkElement target{nullptr};c::TextBlock clock{nullptr};c::TextBox search{nullptr};
    c::RichEditBox richSearch{nullptr};
    c::Grid parent{nullptr};c::Canvas overlay{nullptr};
    x::UIElement searchView{nullptr};
    winrt::event_token textEvent{},layoutEvent{},selectionEvent{};
    winrt::event_token lostFocusEvent{};
    c::Button profileButton{nullptr};c::Grid profileParent{nullptr};
    bool profileVisible=true;
    void EnsureProfile() {
        if(profileButton||(!search&&!richSearch))return;
        auto node=x::Media::VisualTreeHelper::GetParent(target);
        while(node&&!profileParent){auto grid=node.try_as<c::Grid>();if(grid&&grid.ActualWidth()>300)profileParent=grid;node=x::Media::VisualTreeHelper::GetParent(node);}
        if(!profileParent)return;
        profileButton=c::Button();profileButton.Width(28);profileButton.Height(28);
        profileButton.HorizontalAlignment(x::HorizontalAlignment::Right);profileButton.VerticalAlignment(x::VerticalAlignment::Center);
        profileButton.Margin({0,0,10,0});profileButton.Padding({0,0,0,0});profileButton.BorderThickness({0,0,0,0});
        profileButton.Background(x::Media::SolidColorBrush(winrt::Windows::UI::Color{0,0,0,0}));
        c::FontIcon icon;icon.Glyph(L"\xE77B");icon.FontSize(18);profileButton.Content(icon);
        c::MenuFlyout menu;c::MenuFlyoutItem account;account.Text(L"Profilo");
        account.Click([](auto&&,auto&&){InvokeSearchMenu(false);});menu.Items().Append(account);
        c::MenuFlyoutItem more;more.Text(L"Altre opzioni  \x22EF");more.Click([](auto&&,auto&&){InvokeSearchMenu(true);});menu.Items().Append(more);
        c::Grid::SetColumnSpan(profileButton,std::max(1u,profileParent.ColumnDefinitions().Size()));profileButton.Flyout(menu);c::Canvas::SetZIndex(profileButton,100);profileParent.Children().Append(profileButton);
    }
    void RefreshProfile() {
        EnsureProfile();if(!profileButton)return;
        c::TextBlock measure;CopyFont(measure);measure.Text(ReadSearchText());measure.Measure({10000,10000});
        bool visible=measure.DesiredSize().Width<target.ActualWidth()-64;
        if(visible==profileVisible)return;profileVisible=visible;profileButton.IsHitTestVisible(visible);
        auto visual=x::Hosting::ElementCompositionPreview::GetElementVisual(profileButton);
        auto fade=visual.Compositor().CreateScalarKeyFrameAnimation();fade.Duration(TimeSpan{1800000});
        fade.InsertKeyFrame(0,visible?0.f:1.f);fade.InsertKeyFrame(1,visible?1.f:0.f);visual.StartAnimation(L"Opacity",fade);
    }
    int previousCaret=-1;
    int64_t clockToken=0;bool registered=false,dead=false,busy=false,searchQueued=false;
    double originalOpacity=1;
    bool renderTreeReported=false;
    bool searchAnimationActive=false;
    winrt::Windows::Foundation::Point searchAnchor{};
    winrt::Windows::Foundation::Size searchExtent{};
    wchar_t const* searchStage=L"Idle";
    std::wstring previous,layoutKey;
    std::wstring ReadSearchText() {
        if(search) return search.Text().c_str();
        winrt::hstring value;richSearch.Document().GetText(winrt::Windows::UI::Text::TextGetOptions::None,value);
        std::wstring result=value.c_str();
        // A RichEdit document includes its terminal paragraph mark.
        if(!result.empty()&&result.back()==L'\r') result.pop_back();
        return result;
    }
    int CompletionSelectionStart() {
        if(!richSearch) return -1;
        auto selection=richSearch.Document().Selection();
        int start=selection.StartPosition(),end=selection.EndPosition();
        if(start>end) std::swap(start,end);
        const auto length=(int)ReadSearchText().size();
        if(start<=0||start>=end||end!=length) return -1;
        auto before=richSearch.Document().GetRange(start-1,start).CharacterFormat().ForegroundColor();
        auto suffix=richSearch.Document().GetRange(start,start+1).CharacterFormat().ForegroundColor();
        // An inline completion is a differently colored suffix, not a user selection.
        if(before.A==suffix.A&&before.R==suffix.R&&before.G==suffix.G&&before.B==suffix.B) return -1;
        return start;
    }
    int SearchSelectionLength() {
        if(search) return search.SelectionLength();
        if(CompletionSelectionStart()>=0) return 0;
        auto selection=richSearch.Document().Selection();return selection.EndPosition()-selection.StartPosition();
    }
    int SearchCaret() {
        if(search) return search.SelectionStart();
        int completion=CompletionSelectionStart();
        return completion>=0 ? completion : richSearch.Document().Selection().EndPosition();
    }
    void ClearSearchAnimation() {
        searchAnimationActive=false;
        if(searchView) ResetSearchText(searchView);
        if(overlay) {
            auto visual=x::Hosting::ElementCompositionPreview::GetElementVisual(overlay);
            visual.StopAnimation(L"Opacity");visual.Opacity(1);
            overlay.Children().Clear();
        }
    }
    winrt::Windows::Foundation::Rect NativeCharacterRect(int index) {
        if(search) return search.GetRectFromCharacterIndex(index,false);
        winrt::Windows::Foundation::Rect rect;int32_t hit=0;
        richSearch.Document().GetRange(index,index+1).GetRect(
            winrt::Windows::UI::Text::PointOptions::ClientCoordinates,rect,hit);
        return rect;
    }
    void RefreshClock() {
        if(dead||busy||stopping||!clock||!parent) return;
        busy=true;
        try {
            const std::wstring text=clock.Text().c_str();
            auto origin=clock.TransformToVisual(overlay).TransformPoint({0,0});
            auto solid=clock.Foreground().try_as<x::Media::SolidColorBrush>();
            auto color=solid ? solid.Color() : winrt::Windows::UI::Color{};
            auto nativePadding=clock.Padding();
            const auto key=text+L"|"+std::to_wstring(origin.X)+L"|"+std::to_wstring(origin.Y)+L"|"+
                std::to_wstring(clock.ActualWidth())+L"|"+std::to_wstring(clock.ActualHeight())+L"|"+
                std::to_wstring(clock.FontSize())+L"|"+clock.FontFamily().Source().c_str()+L"|"+
                std::to_wstring(clock.FontWeight().Weight)+L"|"+std::to_wstring(clock.CharacterSpacing())+L"|"+
                std::to_wstring((int)clock.Visibility())+L"|"+std::to_wstring((int)clock.TextAlignment())+L"|"+
                std::to_wstring(color.A)+L"|"+std::to_wstring(color.R)+L"|"+std::to_wstring(color.G)+L"|"+std::to_wstring(color.B)+L"|"+
                std::to_wstring((int)clock.FlowDirection())+L"|"+std::to_wstring(nativePadding.Left)+L"|"+std::to_wstring(nativePadding.Right)+L"|"+
                std::to_wstring(nativePadding.Top)+L"|"+std::to_wstring(nativePadding.Bottom)+L"|"+std::to_wstring((int)clock.ActualTheme());
            if(key==layoutKey) {busy=false;return;}layoutKey=key;
            if(text.find(L'\n')!=std::wstring::npos || text.find(L'\r')!=std::wstring::npos) {
                clock.Opacity(originalOpacity);overlay.Children().Clear();busy=false;return;
            }
            overlay.Children().Clear();overlay.Visibility(clock.Visibility());
            overlay.Opacity(originalOpacity);
            auto point=clock.TransformToVisual(overlay).TransformPoint({0,0});
            c::TextBlock measure;CopyFont(measure);measure.Text(text);measure.Measure({10000,10000});
            const float total=measure.DesiredSize().Width;
            // Clock Customization defaults to End, not Right. Resolve logical
            // Start/End against FlowDirection and account for native padding.
            auto padding=clock.Padding();
            point.X+=(float)padding.Left;point.Y+=(float)padding.Top;
            const float available=std::max(0.f,(float)(clock.ActualWidth()-padding.Left-padding.Right));
            auto align=clock.TextAlignment();bool rtl=clock.FlowDirection()==x::FlowDirection::RightToLeft;
            bool right=align==x::TextAlignment::Right || (align==x::TextAlignment::End&&!rtl) || (align==x::TextAlignment::Start&&rtl);
            if(right) point.X+=available-total;
            else if(align==x::TextAlignment::Center) point.X+=(available-total)*.5f;
            if(glassPanel&&!text.empty()) {
                c::Border glass;glass.IsHitTestVisible(false);glass.Width(total+8);glass.Height(std::max(1.f,(float)clock.ActualHeight()-(float)padding.Top-(float)padding.Bottom));
                glass.CornerRadius(x::CornerRadius{glassRadius});
                x::Media::AcrylicBrush material;
                const bool light=clock.ActualTheme()==x::ElementTheme::Light;
                auto tint=light ? winrt::Windows::UI::Color{255,255,255,255} : winrt::Windows::UI::Color{255,28,28,28};
                material.BackgroundSource(x::Media::AcrylicBackgroundSource::Backdrop);
                material.TintColor(tint);material.TintOpacity(glassOpacity);material.FallbackColor(tint);
                glass.Background(material);glass.BorderThickness(x::Thickness{.5});
                glass.BorderBrush(x::Media::SolidColorBrush(winrt::Windows::UI::Color{45,255,255,255}));
                c::Canvas::SetLeft(glass,point.X-4);c::Canvas::SetTop(glass,point.Y);overlay.Children().Append(glass);
            }
            float offset=0;
            const bool animate=previous!=text && !previous.empty() && AnimateEnabled();
            for(size_t i=0;i<text.size();++i) {
                // Preserve UTF-16 surrogate pairs used by custom clock formats.
                size_t length=(text[i]>=0xD800&&text[i]<=0xDBFF&&i+1<text.size()) ? 2 : 1;
                const std::wstring part=text.substr(i,length);
                c::Canvas cell;c::TextBlock current;CopyFont(current);current.Text(part);
                current.Measure({10000,10000});const float width=current.DesiredSize().Width;
                cell.Width(width);cell.Height(std::max(1.0,clock.ActualHeight()));cell.Children().Append(current);
                // Measure prefixes with the same font to preserve kerning and
                // spacing instead of accumulating isolated glyph widths.
                measure.Text(text.substr(0,i));measure.Measure({10000,10000});
                c::Canvas::SetLeft(cell,point.X+measure.DesiredSize().Width);c::Canvas::SetTop(cell,point.Y);
                overlay.Children().Append(cell);
                if(animate && i<previous.size() && text[i]!=previous[i]) {
                    c::TextBlock old;CopyFont(old);old.Text(previous.substr(i,1));
                    cell.Children().InsertAt(0,old);AnimateVisual(old,true);
                    BlurGlyph(current,cell);AnimateVisual(current);
                } else if(animate && i>=previous.size()) {BlurGlyph(current,cell);AnimateVisual(current);}
                offset+=width;i+=length-1;
            }
            previous=text;clock.Opacity(0);
        } catch(...) {Wh_Log(L"Clock animation failed: %08X",winrt::to_hresult());Restore();}
        busy=false;
    }
    void CopyFont(c::TextBlock const& text) {
        if(clock) {
            text.FontFamily(clock.FontFamily());text.FontSize(clock.FontSize());text.FontWeight(clock.FontWeight());
            text.FontStyle(clock.FontStyle());text.FontStretch(clock.FontStretch());text.CharacterSpacing(clock.CharacterSpacing());text.Foreground(clock.Foreground());
        } else {
            auto control=target.as<c::Control>();
            text.FontFamily(control.FontFamily());text.FontSize(control.FontSize());text.FontWeight(control.FontWeight());
            text.FontStyle(control.FontStyle());text.FontStretch(control.FontStretch());text.CharacterSpacing(control.CharacterSpacing());text.Foreground(control.Foreground());
        }
        text.IsHitTestVisible(false);
    }
    x::UIElement FindSearchView(winrt::Windows::Foundation::IInspectable const& object,int depth=0) {
        if(depth>12) return nullptr;
        auto node=object.try_as<x::DependencyObject>();if(!node) return nullptr;
        const std::wstring type=winrt::get_class_name(node).c_str();
        if(type.find(L"TextBoxView")!=std::wstring::npos||type.find(L"RichEditBoxView")!=std::wstring::npos) return node.try_as<x::UIElement>();
        // CortanaRichSearchBox exposes its native editor as a plain FrameworkElement
        // inside the text ScrollViewer, rather than a named TextBoxView.
        if(type==L"Windows.UI.Xaml.Controls.ScrollContentPresenter") {
            auto ancestor=x::Media::VisualTreeHelper::GetParent(node);
            bool textScroller=false;
            for(int i=0;ancestor&&i<8;++i) {
                if(auto scroller=ancestor.try_as<c::ScrollViewer>()) {
                    textScroller=scroller.Name()==L"ContentElement";
                    break;
                }
                ancestor=x::Media::VisualTreeHelper::GetParent(ancestor);
            }
            if(textScroller) for(int i=0;i<x::Media::VisualTreeHelper::GetChildrenCount(node);++i) {
                auto child=x::Media::VisualTreeHelper::GetChild(node,i);
                if(winrt::get_class_name(child)==L"Windows.UI.Xaml.FrameworkElement")
                    if(auto editor=child.try_as<x::UIElement>()) return editor;
            }
        }
        for(int i=0;i<x::Media::VisualTreeHelper::GetChildrenCount(node);++i)
            if(auto found=FindSearchView(x::Media::VisualTreeHelper::GetChild(node,i),depth+1)) return found;
        return nullptr;
    }
    void SearchChanged() {
        if(dead||stopping) return;
        auto status=[](wchar_t const* value){static thread_local std::wstring last;
            if(last!=value) {last=value;Wh_SetStringValue(L"animationStatus",value);}};
        searchStage=L"Read text";
        const std::wstring text=ReadSearchText();const auto old=previous;previous=text;
        const int caret=std::clamp(SearchCaret(),0,(int)text.size());
        const int oldCaret=previousCaret;previousCaret=caret;
        // Search updates its completion without necessarily changing the query.
        // Do not tear down a running letter animation for a redundant event.
        if(text==old&&caret==oldCaret&&searchAnimationActive) {
            if(SearchSelectionLength()>0) ClearSearchAnimation();
            return;
        }
        searchStage=L"Reset native text visual";
        if(text.empty()) {ClearSearchAnimation();status(L"Skipped: empty text");return;}
        if(!AnimateEnabled()) {ClearSearchAnimation();status(L"Skipped: system animations disabled");return;}
        if(SearchSelectionLength()>0) {ClearSearchAnimation();status(L"Skipped: text selection active");return;}
        // Compare typed prefixes only: the completion may already contain the
        // next key's character, although that character has never been typed.
        const size_t oldTyped=std::min(old.size(),(size_t)std::max(0,oldCaret));
        size_t prefix=0;
        while(prefix<oldTyped&&prefix<(size_t)caret&&old[prefix]==text[prefix]) ++prefix;
        const auto added=(size_t)caret>prefix ? (size_t)caret-prefix : 0;
        const bool animateLetters=added>0&&added<=(size_t)maxAnimatedLetters&&(animatePaste||added==1);
        // Leave IME, combining sequences and shaped scripts to the native renderer.
        if(std::any_of(text.begin(),text.end(),[](wchar_t value){return value>=0x300;})) {ClearSearchAnimation();return;}
        if(!searchView) searchView=FindSearchView(target);
        // TextBoxView is an internal UIElement; it need not expose FrameworkElement.
        auto view=searchView;
        if(!view) {
            status(L"Skipped: native text rendering element not found");
            if(renderTreeReported) return;
            renderTreeReported=true;
            std::wstring tree;
            std::vector<x::DependencyObject> nodes{target};
            int visited=0;
            while(!nodes.empty()&&visited++<128&&tree.size()<6000) {
                auto node=nodes.back();nodes.pop_back();
                tree+=winrt::get_class_name(node).c_str();
                if(auto element=node.try_as<x::FrameworkElement>()) {tree+=L"#";tree+=element.Name().c_str();}
                tree+=L"; ";
                for(int i=0;i<x::Media::VisualTreeHelper::GetChildrenCount(node);++i)
                    nodes.push_back(x::Media::VisualTreeHelper::GetChild(node,i));
            }
            Wh_SetStringValue(L"searchRenderTree",tree.c_str());
            return;
        }
        if(!parent) {
            searchStage=L"Create overlay";
            x::DependencyObject node=x::Media::VisualTreeHelper::GetParent(view);
            while(node&&!parent) {parent=node.try_as<c::Grid>();node=x::Media::VisualTreeHelper::GetParent(node);}
            if(!parent) {status(L"Skipped: no parent grid");return;}
            overlay=c::Canvas();overlay.Name(L"FluentSearchOverlay");overlay.IsHitTestVisible(false);parent.Children().Append(overlay);
        }
        searchStage=L"Measure native font";
        c::TextBlock measure;CopyFont(measure);measure.Text(text);measure.Measure({10000,10000});
        const auto size=view.RenderSize();
        if(size.Width<=0||size.Height<=0||measure.DesiredSize().Width>size.Width) {ClearSearchAnimation();status(L"Skipped: text view size unavailable or scrolling");return;} // Preserve native scrolling.
        searchStage=L"Locate text visual";
        const auto origin=view.TransformToVisual(parent).TransformPoint({0,0});
        overlay.Children().Clear();
        auto overlayVisual=x::Hosting::ElementCompositionPreview::GetElementVisual(overlay);
        overlayVisual.StopAnimation(L"Opacity");overlayVisual.Opacity(1);
        const auto fullSize=measure.DesiredSize();
        const float top=origin.Y+std::max(0.f,(size.Height-fullSize.Height)*.5f);
        auto widthAt=[&](size_t index) {
            measure.Text(text.substr(0,index));measure.Measure({10000,10000});
            return measure.DesiredSize().Width;
        };
        const float caretX=widthAt((size_t)caret);
        const float prefixX=widthAt(std::min(prefix,(size_t)caret));
        auto drawSlice=[&](float left,float right,bool animate,bool completion) {
            if(right<=left) return;
            c::TextBlock shaped;CopyFont(shaped);shaped.Text(text);
            if(completion&&richSearch&&caret<(int)text.size()) {
                auto color=richSearch.Document().GetRange(caret,caret+1).CharacterFormat().ForegroundColor();
                shaped.Foreground(x::Media::SolidColorBrush(color));
            }
            shaped.Measure({10000,10000});shaped.Arrange({0,0,fullSize.Width,fullSize.Height});
            x::Media::RectangleGeometry clip;clip.Rect({left,0,right-left,fullSize.Height});shaped.Clip(clip);
            c::Canvas cell;c::Canvas::SetLeft(cell,origin.X);c::Canvas::SetTop(cell,top);
            cell.Children().Append(shaped);overlay.Children().Append(cell);
            if(animate) {
                x::Media::RectangleGeometry blurClip;blurClip.Rect({left,-travel,right-left,fullSize.Height+2*travel});cell.Clip(blurClip);
                searchStage=L"Animate shaped text slice";
                BlurGlyph(shaped,cell);AnimateVisual(shaped);
                auto visual=x::Hosting::ElementCompositionPreview::GetElementVisual(shaped);
                visual.CenterPoint({(left+right)*.5f,fullSize.Height*.5f,0});
            } else if(completion&&text!=old) {
                // A new inline suggestion fades into focus without moving.
                // Consuming the same suggestion with additional keys doesn't restart it.
                x::Media::RectangleGeometry blurClip;blurClip.Rect({left,0,right-left,fullSize.Height});cell.Clip(blurClip);
                searchStage=L"Fade inline completion";
                BlurGlyph(shaped,cell,0,std::min(2.f,blurRadius));
                auto visual=x::Hosting::ElementCompositionPreview::GetElementVisual(shaped);
                auto compositor=visual.Compositor();
                auto fade=compositor.CreateScalarKeyFrameAnimation();
                fade.Duration(TimeSpan{durationMs*10000LL});
                auto ease=compositor.CreateCubicBezierEasingFunction({.16f,1.f},{.3f,1.f});
                fade.InsertKeyFrame(0,0.f);fade.InsertKeyFrame(1,1.f,ease);
                visual.StartAnimation(L"Opacity",fade);
            }
        };
        if(animateLetters) {
            drawSlice(0,prefixX,false,false);
            drawSlice(prefixX,caretX,true,false);
        } else drawSlice(0,caretX,false,false);
        drawSlice(caretX,fullSize.Width,false,true);
        searchStage=L"Replace native text display";
        auto compositor=x::Hosting::ElementCompositionPreview::GetElementVisual(view).Compositor();
        c::Border cursor;cursor.Width(1);cursor.Height(fullSize.Height);
        cursor.Background(target.as<c::Control>().Foreground());
        c::Canvas::SetLeft(cursor,origin.X+caretX);
        c::Canvas::SetTop(cursor,top);
        overlay.Children().Append(cursor);
        auto blink=compositor.CreateScalarKeyFrameAnimation();blink.Duration(TimeSpan{10000000});
        blink.IterationBehavior(winrt::Windows::UI::Composition::AnimationIterationBehavior::Forever);
        auto step=compositor.CreateStepEasingFunction();step.IsFinalStepSingleFrame(true);
        blink.InsertKeyFrame(0,1);blink.InsertKeyFrame(.5f,0,step);blink.InsertKeyFrame(1,1,step);
        x::Hosting::ElementCompositionPreview::GetElementVisual(cursor).StartAnimation(L"Opacity",blink);
        auto native=x::Hosting::ElementCompositionPreview::GetElementVisual(view);
        native.StopAnimation(L"Opacity");native.Opacity(0);
        searchAnchor=origin;searchExtent=size;searchAnimationActive=true;
        status(L"Animation started");
    }
    void Restore() noexcept {
        if(dead) return;dead=true;
        try {
            if(isLayout) {
                if(layoutWeb&&layoutNavigation.value) winrt::check_hresult(layoutWeb->remove_NavigationCompleted(layoutNavigation));
                if(layoutLegacy&&layoutNavigation.value) layoutLegacy.NavigationCompleted(layoutNavigation);
                ApplyHome(true);return;
            }
            if(clock) {
                if(registered) clock.UnregisterPropertyChangedCallback(c::TextBlock::TextProperty(),clockToken);
                if(layoutEvent.value) clock.LayoutUpdated(layoutEvent);
                clock.Opacity(originalOpacity);
                if(parent&&overlay) {uint32_t index;if(parent.Children().IndexOf(overlay,index)) parent.Children().RemoveAt(index);}
            }
            if(search||richSearch) {
                if(profileParent&&profileButton){uint32_t index;if(profileParent.Children().IndexOf(profileButton,index))profileParent.Children().RemoveAt(index);}
                if(lostFocusEvent.value) target.LostFocus(lostFocusEvent);
                if(layoutEvent.value) target.LayoutUpdated(layoutEvent);
                if(parent&&overlay) {uint32_t index;if(parent.Children().IndexOf(overlay,index)) parent.Children().RemoveAt(index);}
                if(search) {if(textEvent.value) search.TextChanged(textEvent);if(selectionEvent.value) search.SelectionChanged(selectionEvent);}
                else {if(textEvent.value) richSearch.TextChanged(textEvent);if(selectionEvent.value) richSearch.SelectionChanged(selectionEvent);}
                if(searchView) ResetSearchText(searchView);
            }
        } catch(...) {}
    }
};
std::mutex bindingsMutex;
std::vector<std::shared_ptr<Binding>> bindings;


void InvokeSearchMenu(bool more) {
    std::vector<std::shared_ptr<Binding>> copy;{std::lock_guard lock(bindingsMutex);copy=bindings;}
    for(auto const& state:copy)if(!state->dead&&state->isLayout&&state->target.Dispatcher().HasThreadAccess())try {
        winrt::hstring script=more?L"window.__fluentSearchMore?.()":L"window.__fluentSearchProfile?.()";
        if(state->layoutWeb){void* ptr=nullptr;winrt::check_hresult(state->layoutWeb->ExecuteScriptAsync(winrt::get_abi(script),&ptr));winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> op{ptr,winrt::take_ownership_from_abi};}
        else if(state->layoutLegacy)state->layoutLegacy.InvokeScriptAsync(L"eval",winrt::single_threaded_vector<winrt::hstring>({script}));
    }catch(...) {}
}

void AttachSearchLayout(x::FrameworkElement const& element) {
#if !FLUENT_CLOCK
    const auto type=winrt::get_class_name(element);
    if(type!=L"WebView2Standalone.Controls.WebView2"&&type!=L"Windows.UI.Xaml.Controls.WebView")return;
    {std::lock_guard lock(bindingsMutex);for(auto const& old:bindings)if(!old->dead&&old->target==element)return;}
    auto state=std::make_shared<Binding>();state->target=element;state->isLayout=true;
    std::weak_ptr<Binding> weak=state;
    if(type==L"WebView2Standalone.Controls.WebView2") {
        winrt::check_hresult(reinterpret_cast<IUnknown*>(winrt::get_abi(element))->QueryInterface(IID_FluentWebView,state->layoutWeb.put_void()));
        winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Foundation::IInspectable,winrt::Windows::Foundation::IInspectable> handler=
            [weak](auto&&,auto&&){if(auto self=weak.lock())try {self->ApplyHome();}catch(...) {Wh_Log(L"Search home navigation: %08X",winrt::to_hresult());}};
        winrt::check_hresult(state->layoutWeb->add_NavigationCompleted(winrt::get_abi(handler),&state->layoutNavigation));
    } else {
        state->layoutLegacy=element.as<c::WebView>();
        state->layoutNavigation=state->layoutLegacy.NavigationCompleted([weak](auto&&,auto&&){if(auto self=weak.lock())try {self->ApplyHome();}catch(...) {}});
    }
    {std::lock_guard lock(bindingsMutex);bindings.push_back(state);}
    try {state->ApplyHome();}catch(...) {Wh_Log(L"Search home initial apply: %08X",winrt::to_hresult());}
#endif
}

bool IsSearchBox(x::FrameworkElement const& box) {
    x::DependencyObject node=box;
    for(int i=0;node&&i<8;++i) {
        auto element=node.try_as<x::FrameworkElement>();
        std::wstring name=element ? element.Name().c_str() : L"";
        name+=winrt::get_class_name(node);std::transform(name.begin(),name.end(),name.begin(),towlower);
        if(name.find(L"search")!=std::wstring::npos) return true;
        node=x::Media::VisualTreeHelper::GetParent(node);
    }
    return false;
}
void Attach(x::FrameworkElement const& element) {
    if(stopping) return;
    auto state=std::make_shared<Binding>();state->target=element;
#if FLUENT_CLOCK
    auto text=element.try_as<c::TextBlock>();if(!text||text.Name()!=L"TimeInnerTextBlock") return;
    state->clock=text;
    x::DependencyObject node=x::Media::VisualTreeHelper::GetParent(text);
    while(node&&!state->parent) {state->parent=node.try_as<c::Grid>();node=x::Media::VisualTreeHelper::GetParent(node);}
    if(!state->parent) return;
#else
    auto box=element.try_as<c::TextBox>();auto rich=element.try_as<c::RichEditBox>();
    if((!box&&!rich)||!IsSearchBox(element)) return;
    state->search=box;state->richSearch=rich;
#endif
    {std::lock_guard lock(bindingsMutex);for(auto const& old:bindings) if(!old->dead&&old->target==element) return;bindings.push_back(state);}
    const std::weak_ptr<Binding> weak=state;
#if FLUENT_CLOCK
    state->originalOpacity=text.Opacity();
    state->overlay=c::Canvas();state->overlay.Name(L"FluentClockOverlay");state->overlay.IsHitTestVisible(false);
    c::Grid::SetColumnSpan(state->overlay,std::max(1u,state->parent.ColumnDefinitions().Size()));
    c::Grid::SetRowSpan(state->overlay,std::max(1u,state->parent.RowDefinitions().Size()));
    state->overlay.HorizontalAlignment(x::HorizontalAlignment::Stretch);
    state->overlay.VerticalAlignment(x::VerticalAlignment::Stretch);
    state->parent.Children().Append(state->overlay);
    state->clockToken=text.RegisterPropertyChangedCallback(c::TextBlock::TextProperty(),[weak](auto&&,auto&&){if(auto self=weak.lock()) self->RefreshClock();});
    state->registered=true;
    state->layoutEvent=text.LayoutUpdated([weak](auto&&,auto&&){if(auto self=weak.lock()) self->RefreshClock();});
    // RefreshClock hides the native text only after a valid overlay is ready.
    // Unsupported multi-line formats must remain visible.
    state->RefreshClock();
#else
    state->previous=state->ReadSearchText();
    auto changed=[weak](auto&&,auto&&){
        try {if(auto self=weak.lock()) {
            if(self->searchQueued||self->dead||stopping) return;
            self->searchQueued=true;++pendingCallbacks;
            try {self->target.Dispatcher().RunAsync(winrt::Windows::UI::Core::CoreDispatcherPriority::Normal,[weak] {
                try {if(auto current=weak.lock()) {current->searchQueued=false;if(!stopping&&!current->dead) current->SearchChanged();}}catch(...) {auto code=winrt::to_hresult();wchar_t message[200];auto current=weak.lock();swprintf_s(message,L"Animation exception: %08X at %s",static_cast<unsigned>(code),current ? current->searchStage : L"unknown");Wh_SetStringValue(L"animationStatus",message);if(current) try {current->ClearSearchAnimation();}catch(...) {}Wh_Log(L"Search animation failed: %08X",code);}
                --pendingCallbacks;
            });}catch(...) {self->searchQueued=false;--pendingCallbacks;}
        }}catch(...) {}
    };
    auto selected=[weak,changed](auto&& sender,auto&& args){try {if(auto self=weak.lock()) {
        if(self->SearchSelectionLength()>0) self->ClearSearchAnimation();
        else changed(sender,args);
    }
    }catch(...) {}};
    if(box) {state->textEvent=box.TextChanged(changed);state->selectionEvent=box.SelectionChanged(selected);}
    else {state->textEvent=rich.TextChanged(changed);state->selectionEvent=rich.SelectionChanged(selected);}
    state->lostFocusEvent=element.LostFocus([weak](auto&&,auto&&){if(auto self=weak.lock()) try {self->ClearSearchAnimation();}catch(...) {}});
    state->layoutEvent=element.LayoutUpdated([weak](auto&&,auto&&){try {
        if(auto self=weak.lock()) if(self->searchAnimationActive&&self->searchView&&self->parent) {
            auto point=self->searchView.TransformToVisual(self->parent).TransformPoint({0,0});
            auto size=self->searchView.RenderSize();
            if(std::abs(point.X-self->searchAnchor.X)>.25f||std::abs(point.Y-self->searchAnchor.Y)>.25f||
                std::abs(size.Width-self->searchExtent.Width)>.25f||std::abs(size.Height-self->searchExtent.Height)>.25f)
                self->ClearSearchAnimation();
        }
    }catch(...) {}});
    Wh_SetStringValue(L"animationStatus",L"Waiting for TextChanged on attached search field");
#endif
    Wh_Log(L"Attached to %s",element.Name().c_str());
    auto status=std::wstring(L"Attached: ")+winrt::get_class_name(element).c_str()+L"#"+element.Name().c_str();
    Wh_SetStringValue(L"attachmentStatus",status.c_str());
}
// Acquire ABI entry points on an existing XAML UI thread. No diagnostics session,
// private symbols, or exclusive visual-tree subscription is needed.
using TextAccess = HRESULT (WINAPI*)(void*, void*);
using NameAccess = HRESULT (WINAPI*)(void*, void**);
TextAccess textAccessOriginal=nullptr;
NameAccess nameAccessOriginal=nullptr;
using TextChangedSubscription=HRESULT (WINAPI*)(void*,void*,winrt::event_token*);
TextChangedSubscription textChangedOriginal=nullptr;
TextChangedSubscription richTextChangedOriginal=nullptr;
std::atomic<bool> directHooksReady=false;
std::mutex discoveryMutex;
std::unordered_set<void*> queuedControls;
thread_local bool insideDiscovery=false;
void QueueControl(x::FrameworkElement const& element) noexcept {
    if(stopping||insideDiscovery||!element) return;
    insideDiscovery=true;
    try {
        bool eligible=false;
#if FLUENT_CLOCK
        eligible=element.try_as<c::TextBlock>() && element.Name()==L"TimeInnerTextBlock";
#else
        if(element.try_as<c::TextBox>()||element.try_as<c::RichEditBox>()) eligible=IsSearchBox(element);
#endif
        if(eligible) {
            bool bound=false;
            {std::lock_guard lock(bindingsMutex);for(auto const& entry:bindings) if(!entry->dead&&entry->target==element) {bound=true;break;}}
            auto identity=winrt::get_abi(element);
            if(!bound) {
                bool inserted;
                {std::lock_guard lock(discoveryMutex);inserted=queuedControls.insert(identity).second;}
                if(inserted) {
                    auto weak=winrt::make_weak(element);
                    ++pendingCallbacks;
                    try {
                        element.Dispatcher().RunAsync(winrt::Windows::UI::Core::CoreDispatcherPriority::Normal,[weak,identity] {
                            bool previous=insideDiscovery;insideDiscovery=true;
                            try {if(!stopping) if(auto control=weak.get()) Attach(control);}catch(...) {Wh_Log(L"Direct attach failed: %08X",winrt::to_hresult());}
                            insideDiscovery=previous;
                            {std::lock_guard lock(discoveryMutex);queuedControls.erase(identity);}
                            --pendingCallbacks;
                        });
                    } catch(...) {
                        {std::lock_guard lock(discoveryMutex);queuedControls.erase(identity);}
                        --pendingCallbacks;
                    }
                }
            }
        }
    } catch(...) {}
    insideDiscovery=false;
}
HRESULT WINAPI TextAccessHook(void* self,void* value) {
    HRESULT result=textAccessOriginal(self,value);
    if(SUCCEEDED(result)&&!stopping&&!insideDiscovery) {
        try {
#if FLUENT_CLOCK
            c::ITextBlock control{nullptr};winrt::copy_from_abi(control,self);
#else
            c::ITextBox control{nullptr};winrt::copy_from_abi(control,self);
#endif
            QueueControl(control.as<x::FrameworkElement>());
        } catch(...) {}
    }
    return result;
}
HRESULT WINAPI TextChangedSubscriptionHook(void* self,void* handler,winrt::event_token* token) {
    const auto result=textChangedOriginal(self,handler,token);
    if(SUCCEEDED(result)&&!stopping&&!insideDiscovery) {
        try {c::ITextBox control{nullptr};winrt::copy_from_abi(control,self);QueueControl(control.as<x::FrameworkElement>());}catch(...) {}
    }
    return result;
}
HRESULT WINAPI RichTextChangedSubscriptionHook(void* self,void* handler,winrt::event_token* token) {
    const auto result=richTextChangedOriginal(self,handler,token);
    if(SUCCEEDED(result)&&!stopping&&!insideDiscovery) {
        try {c::IRichEditBox control{nullptr};winrt::copy_from_abi(control,self);QueueControl(control.as<x::FrameworkElement>());}catch(...) {}
    }
    return result;
}
void QueueSearchContainer(x::FrameworkElement const& element,std::wstring_view name) noexcept {
#if !FLUENT_CLOCK
    if(stopping||insideDiscovery||element.try_as<c::TextBox>()||element.try_as<c::RichEditBox>()) return;
    try {
        auto type=std::wstring(winrt::get_class_name(element).c_str());
        if(type.find(L"RichSearchBox")==std::wstring::npos&&name!=L"SearchTextBox"&&name!=L"SearchBoxControl") return;
        auto identity=winrt::get_abi(element);
        {std::lock_guard lock(discoveryMutex);if(!queuedControls.insert(identity).second) return;}
        auto weak=winrt::make_weak(element);++pendingCallbacks;
        try {element.Dispatcher().RunAsync(winrt::Windows::UI::Core::CoreDispatcherPriority::Normal,[weak,identity] {
            bool before=insideDiscovery;insideDiscovery=true;
            try {if(!stopping) if(auto container=weak.get()) {
                std::vector<x::DependencyObject> nodes{container};int visited=0;
                while(!nodes.empty()&&visited++<128&&!stopping) {
                    auto node=nodes.back();nodes.pop_back();
                    if(auto child=node.try_as<x::FrameworkElement>()) if(child.try_as<c::TextBox>()||child.try_as<c::RichEditBox>()) {
                        // Attach directly while guarded; no recursive name hook.
                        Attach(child);
                    }
                    auto count=x::Media::VisualTreeHelper::GetChildrenCount(node);
                    for(int i=0;i<count;++i) nodes.push_back(x::Media::VisualTreeHelper::GetChild(node,i));
                }
            }}catch(...) {Wh_Log(L"Search container attach failed: %08X",winrt::to_hresult());}
            insideDiscovery=before;
            {std::lock_guard lock(discoveryMutex);queuedControls.erase(identity);}
            --pendingCallbacks;
        });}catch(...) {{std::lock_guard lock(discoveryMutex);queuedControls.erase(identity);}--pendingCallbacks;}
    }catch(...) {}
#endif
}
void DiscoverSearchRoot(x::FrameworkElement const& control) noexcept {
#if !FLUENT_CLOCK
    // Called once by the UI bootstrap, never from a text/name getter.
    if(stopping||insideDiscovery) return;
    try {
        x::UIElement root{nullptr};
        if(auto xamlRoot=control.XamlRoot()) root=xamlRoot.Content();
        else if(auto window=x::Window::Current()) root=window.Content();
        if(!root) return;
        auto weak=winrt::make_weak(root);
        ++pendingCallbacks;
        try {control.Dispatcher().RunAsync(winrt::Windows::UI::Core::CoreDispatcherPriority::Normal,[weak] {
            bool before=insideDiscovery;insideDiscovery=true;
            std::wstring candidates;
            try {if(!stopping) if(auto root=weak.get()) {
                std::vector<x::DependencyObject> nodes{root};int visited=0;
                while(!nodes.empty()&&visited++<4096&&!stopping) {
                    auto node=nodes.back();nodes.pop_back();
                    if(auto element=node.try_as<x::FrameworkElement>()) {
                        std::wstring name=element.Name().c_str();
                        std::transform(name.begin(),name.end(),name.begin(),towlower);
                        if(name.find(L"search")!=std::wstring::npos&&candidates.size()<1800) {
                            candidates+=winrt::get_class_name(element).c_str();candidates+=L"#";candidates+=name;candidates+=L"; ";
                        }
                        if(element.try_as<c::TextBox>()||element.try_as<c::RichEditBox>()) Attach(element);
                        try {AttachSearchLayout(element);}catch(...) {Wh_Log(L"Search layout attachment: %08X",winrt::to_hresult());}
                    }
                    auto count=x::Media::VisualTreeHelper::GetChildrenCount(node);
                    for(int i=0;i<count;++i) nodes.push_back(x::Media::VisualTreeHelper::GetChild(node,i));
                }
            }}catch(...) {Wh_Log(L"Search root discovery failed: %08X",winrt::to_hresult());}
            insideDiscovery=before;
            wchar_t key[80];swprintf_s(key,L"searchCandidates.%u",GetCurrentProcessId());
            Wh_SetStringValue(key,candidates.empty() ? L"No named search controls in this XAML root" : candidates.c_str());
            --pendingCallbacks;
        });}catch(...) {--pendingCallbacks;}
    }catch(...) {}
#endif
}
HRESULT WINAPI NameAccessHook(void* self,void** value) {
    HRESULT result=nameAccessOriginal(self,value);
    if(SUCCEEDED(result)&&value&&*value&&!stopping&&!insideDiscovery) {
        try {
            UINT32 length=0;const auto raw=WindowsGetStringRawBuffer(static_cast<HSTRING>(*value),&length);
#if FLUENT_CLOCK
            if(std::wstring_view(raw,length)!=L"TimeInnerTextBlock") return result;
#else
            if(length>256) return result;
            std::wstring name(raw,length);std::transform(name.begin(),name.end(),name.begin(),towlower);
            if(name.find(L"search")==std::wstring::npos) return result;
#endif
            x::IFrameworkElement control{nullptr};winrt::copy_from_abi(control,self);
            auto element=control.as<x::FrameworkElement>();
            QueueControl(element);
#if !FLUENT_CLOCK
            QueueSearchContainer(element,std::wstring_view(raw,length));
#endif
        } catch(...) {}
    }
    return result;
}
const UINT bootstrapMessage=RegisterWindowMessageW(L"FluentAnimations.DirectAttach.0.1.1");
void DiscoverFocusedSearchControl() noexcept;
void RefreshCompactSearchHome() noexcept {
#if !FLUENT_CLOCK
    try {std::vector<std::shared_ptr<Binding>> copy;{std::lock_guard lock(bindingsMutex);copy=bindings;}
        for(auto const& state:copy)if(!state->dead&&(state->search||state->richSearch)&&state->target.Dispatcher().HasThreadAccess())state->RefreshProfile();
    }catch(...) {}
#endif
}
std::mutex bootstrapMutex;
std::vector<HHOOK> bootstrapHooks;
LRESULT CALLBACK BootstrapCallback(int code,WPARAM wParam,LPARAM lParam) {
    if(code>=0&&!stopping) {
        auto message=reinterpret_cast<CWPSTRUCT*>(lParam);
        if(message->message==bootstrapMessage) {
            if(directHooksReady) {
#if !FLUENT_CLOCK
                DiscoverFocusedSearchControl();
                RefreshCompactSearchHome();
#endif
                return CallNextHookEx(nullptr,code,wParam,lParam);
            }
            try {
                // Construction is deliberately on the host's UI thread.
#if FLUENT_CLOCK
                c::TextBlock sample;
                auto textInterface=sample.as<c::ITextBlock>();
                // IInspectable's six entries, then 21 entries before put_Text.
                auto textEntry=(*reinterpret_cast<void***>(winrt::get_abi(textInterface)))[27];
#else
                c::TextBox sample;
                auto textInterface=sample.as<c::ITextBox>();
                // Observe event subscriptions, not get_Text. The search query
                // read path is left untouched to avoid interfering with queries.
                auto textEntry=(*reinterpret_cast<void***>(winrt::get_abi(textInterface)))[30];
#endif
                auto framework=sample.as<x::IFrameworkElement>();
                auto nameEntry=(*reinterpret_cast<void***>(winrt::get_abi(framework)))[33];
                std::lock_guard lock(bootstrapMutex);
                if(!directHooksReady) {
#if FLUENT_CLOCK
                    bool textOk=Wh_SetFunctionHook(textEntry,reinterpret_cast<void*>(&TextAccessHook),reinterpret_cast<void**>(&textAccessOriginal));
#else
                    bool textOk=Wh_SetFunctionHook(textEntry,reinterpret_cast<void*>(&TextChangedSubscriptionHook),reinterpret_cast<void**>(&textChangedOriginal));
                    c::RichEditBox richSample;auto richInterface=richSample.as<c::IRichEditBox>();
                    auto richEntry=(*reinterpret_cast<void***>(winrt::get_abi(richInterface)))[21];
                    bool richOk=Wh_SetFunctionHook(richEntry,reinterpret_cast<void*>(&RichTextChangedSubscriptionHook),reinterpret_cast<void**>(&richTextChangedOriginal));
                    Wh_Log(L"Rich text subscription hook: %d",richOk);
#endif
                    bool nameOk=Wh_SetFunctionHook(nameEntry,reinterpret_cast<void*>(&NameAccessHook),reinterpret_cast<void**>(&nameAccessOriginal));
                    Wh_ApplyHookOperations();
                    directHooksReady=textOk||nameOk;
                    Wh_Log(L"Direct XAML control hooks: text=%d, name=%d, UI thread=%u",textOk,nameOk,GetCurrentThreadId());
                    Wh_SetStringValue(L"attachmentStatus",L"Hooks ready; waiting for a search/clock control");
                }
                DiscoverSearchRoot(sample);
            } catch(...) {Wh_Log(L"Waiting for a XAML UI thread: %08X",winrt::to_hresult());}
        }
    }
    return CallNextHookEx(nullptr,code,wParam,lParam);
}
HANDLE startupThread=nullptr;
void DiscoverFocusedSearchControl() noexcept {
#if !FLUENT_CLOCK
    if(stopping||insideDiscovery) return;
    static thread_local ULONGLONG lastAttempt=0;
    auto now=GetTickCount64();if(now-lastAttempt<100) return;lastAttempt=now;
    try {
        auto focused=x::Input::FocusManager::GetFocusedElement();
        if(!focused) {
            if(auto window=x::Window::Current()) if(auto content=window.Content()) if(auto root=content.XamlRoot())
                focused=x::Input::FocusManager::GetFocusedElement(root);
        }
        static thread_local std::wstring lastType;
        const std::wstring type=focused ? winrt::get_class_name(focused).c_str() : L"No focused XAML element";
        if(type!=lastType) {
            lastType=type;Wh_SetStringValue(L"focusedControl",type.c_str());
            wchar_t key[80];swprintf_s(key,L"focusedControl.%u",GetCurrentProcessId());Wh_SetStringValue(key,type.c_str());
            Wh_Log(L"Focused search control: %s",type.c_str());
        }
        if(auto element=focused.try_as<x::FrameworkElement>()) {
            if(auto root=element.XamlRoot()) {
                static thread_local winrt::weak_ref<x::XamlRoot> previousRoot;
                if(previousRoot.get()!=root) {previousRoot=winrt::make_weak(root);DiscoverSearchRoot(element);}
            }
        }
        auto node=focused.try_as<x::DependencyObject>();
        // Internal editing views may have focus rather than the public control.
        for(int i=0;node&&i<12;++i) {
            if(auto element=node.try_as<x::FrameworkElement>()) {
                if(element.try_as<c::TextBox>()||element.try_as<c::RichEditBox>()) {QueueControl(element);break;}
                auto name=element.Name();QueueSearchContainer(element,std::wstring_view(name));
            }
            node=x::Media::VisualTreeHelper::GetParent(node);
        }
    }catch(...) {
        static thread_local bool reported=false;
        if(!reported) {reported=true;Wh_SetStringValue(L"focusedControl",L"Focus discovery unavailable on this UI thread");Wh_Log(L"Focus discovery unavailable: %08X",winrt::to_hresult());}
    }
#endif
}
BOOL Wh_ModInit() {
    LoadOptions();
    return TRUE;
}
void Wh_ModAfterInit() {
    startupThread=CreateThread(nullptr,0,[](void*)->DWORD {
        std::unordered_set<DWORD> threads;
        // Hosts may create their XAML windows after Windhawk injects the mod.
        while(!stopping) {
#if FLUENT_CLOCK
            if(directHooksReady) break;
#else
            if(directHooksReady) {
                DWORD foregroundProcess=0;GetWindowThreadProcessId(GetForegroundWindow(),&foregroundProcess);
                if(foregroundProcess!=GetCurrentProcessId()) {
                    for(int i=0;i<5&&!stopping;++i) Sleep(50);
                    continue;
                }
            }
#endif
            std::vector<HWND> windows;
            EnumWindows([](HWND window,LPARAM context)->BOOL {
                DWORD process=0;GetWindowThreadProcessId(window,&process);
                if(process==GetCurrentProcessId()) reinterpret_cast<std::vector<HWND>*>(context)->push_back(window);
                return TRUE;
            },reinterpret_cast<LPARAM>(&windows));
            for(auto window:windows) {
                DWORD thread=GetWindowThreadProcessId(window,nullptr);
                if(threads.insert(thread).second) {
                    auto hook=SetWindowsHookExW(WH_CALLWNDPROC,BootstrapCallback,nullptr,thread);
                    if(hook) {std::lock_guard lock(bootstrapMutex);bootstrapHooks.push_back(hook);}
                    else threads.erase(thread);
                }
                // SendNotifyMessage dispatches asynchronously across threads,
                // unlike PostMessage, which doesn't call WH_CALLWNDPROC hooks.
                SendNotifyMessageW(window,bootstrapMessage,0,0);
            }
            for(int i=0;i<(directHooksReady ? 5 : 20)&&!stopping;++i) Sleep(50);
        }
        return 0;
    },nullptr,0,nullptr);
}
void Wh_ModBeforeUninit() {
    stopping=true;
    if(startupThread) {if(WaitForSingleObject(startupThread,5000)!=WAIT_OBJECT_0) RetainModule();CloseHandle(startupThread);startupThread=nullptr;}
    {std::lock_guard lock(bootstrapMutex);for(auto hook:bootstrapHooks) UnhookWindowsHookEx(hook);bootstrapHooks.clear();}
    const auto pendingDeadline=GetTickCount64()+5000;
    while(pendingCallbacks && GetTickCount64()<pendingDeadline) Sleep(10);
    if(pendingCallbacks) RetainModule();
    std::vector<std::shared_ptr<Binding>> copy;
    {std::lock_guard lock(bindingsMutex);copy=bindings;}
    for(auto const& state:copy) {
        try {
            auto dispatcher=state->target.Dispatcher();
            if(dispatcher.HasThreadAccess()) state->Restore();
            else {
                auto done=std::shared_ptr<void>(CreateEventW(nullptr,TRUE,FALSE,nullptr),[](void* handle){CloseHandle(handle);});
                dispatcher.RunAsync(winrt::Windows::UI::Core::CoreDispatcherPriority::High,[state,done]{state->Restore();SetEvent(done.get());});
                if(WaitForSingleObject(done.get(),5000)!=WAIT_OBJECT_0) RetainModule();
            }
        }catch(...) {}
    }
    {std::lock_guard lock(bindingsMutex);bindings.clear();}
}
void Wh_ModUninit() {}
void Wh_ModSettingsChanged(BOOL* reload) {*reload=TRUE;}


