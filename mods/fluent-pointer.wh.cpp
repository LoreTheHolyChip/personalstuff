// ==WindhawkMod==
// @id              fluent-pointer
// @name            Fluent Pointer
// @description     Pointer movement tilt and spring selection animation, with optional afterimages, The homepage is what i used for the motion blur.
// @version         0.3.53
// @author          LTHC
// @github          https://github.com/LoreTheHolyChip
// @homepage        https://github.com/sidlikesgrapess/CursorMotionBlur
// @include         windhawk.exe
// @include         *
// @exclude         dwm.exe
// @exclude         csrss.exe
// @exclude         winlogon.exe
// @exclude         LogonUI.exe
// @exclude         lsass.exe
// @exclude         services.exe
// @exclude         svchost.exe
// @exclude         fontdrvhost.exe
// @compilerOptions -lgdi32 -lshell32 -lshcore -lwinmm -lole32 -loleaut32 -luuid
// @license         Unlicense
// ==/WindhawkMod==

// ==WindhawkModReadme==
/*
Based on Pointer Afterimage Motion Blur 1.0 by sidlikesgrapess (Unlicense).
Disable the original pointer blur mod before enabling this fork: both replace
cursor visibility while drawing an overlay. Native cursor visibility is restored
when the effect stops or the mod unloads. Rotation can pivot around the tip, glyph center, or bottom-right corner.
The actual click coordinate stays unchanged; center/corner rotation moves the
visible tip around that coordinate. Holding the left mouse button maintains the selected angle; release returns
with a damped spring. Vertical movement has its own tilt intensity. Pressure compression is adjustable
and scales around the real cursor hotspot, then rebounds on release. Movement
stretch follows velocity in both axes and smoothly rebounds when stopped. Its
maximum amount and speed threshold are adjustable independently of tilt.
Visual magnetism reads accessible enabled button bounds without activating controls.
It depends on application accessibility support. Wheel impulses, held drag lag, and
standard resize-cursor axial stretch each have independent toggles and amounts.
Shape changes can start compressed and overshoot gently, using the existing
spring intensity. Clicks add a short visual-only spring nudge towards the last
known accessible button center (or downward outside known buttons). Button bounds
expire after 250 ms; no extra accessibility calls run in the mouse input handler.
Held rotation has independent exclusions for standard resize/move, text and busy
cursors. Exclusion returns the angle smoothly to zero; axial stretch remains.
Idle animation chooses a spin, figure eight, or look-around after a random
8–25 second pause by default. It avoids consecutive repeats when multiple effects
are enabled. Mouse movement, clicks, scrolling and held buttons cancel it; arrow
and hand cursors are the default idle targets. Busy/working cursors can hop with
a separate height and interval. Idle waits use a low-frequency sampler timeout.
The application's CURSOR_SHOWING state is authoritative, including changes
from visible to hidden while the pointer is stationary. Cursor show/hide events
wake the sampler and renderer, and drawing rechecks that state. Empty cursors
remain hidden. Our native suppression flag never overrides application hiding.
The visible replacement is retained while stationary only while the app shows it. A settled
frame is kept without continual redraws. Native cursor suppression requires a
successfully drawn replacement; fallback restores the native cursor before hiding
the overlay, under the same visibility lock as the suppression thread.
No physical mouse coordinates or input events are changed. Optional automatic
sleep (disabled by default) draws a virtual pointer journey and uses accessibility
Invoke on the actual Start, Power and Sleep controls. Inactivity includes keyboard
input. User input, settings changes or a protected desktop cancel the sequence.
Unsupported/custom Start menus abort instead of clicking guessed coordinates.
*/
// ==/WindhawkModReadme==

// ==WindhawkModSettings==
/*
- CursorColor:
  - mode: none
    $name: "Colore del cursore"
    $options:
    - none: Originale
    - accent: Colore di accento Windows
    - custom: Personalizzato
  - custom: '#00BFFF'
    $name: "Colore personalizzato (#RRGGBB)"
  - full: false
    $name: "Colora anche tutto il cursore"
    $description: "Se disattivato, colora solo la rotella azzurra di attesa/caricamento."
  $name: "Colore del cursore"
- TrailColor:
  - mode: rainbow
    $name: "Color"
    $name:it: "Colore"
    $options:
    - none: None
    - rainbow: Rainbow
    - custom: Custom
  - custom: '#00BFFF'
    $name: "Custom color (#RRGGBB)"
    $name:it: "Colore personalizzato (#RRGGBB)"
  - speed: 300
    $name: "Rainbow speed (percent)"
    $name:it: "Velocità del cambio colore arcobaleno (1–2000%)"
  $name: "Color the trail"
  $name:it: "Colora la scia"
- IdleTrail:
  - pinCursor: false
    $name: "Keep cursor at the real mouse position"
    $name:it: "Mantieni il cursore nella posizione reale del mouse"
    $description:it: "Durante Knight anima soltanto la scia. Il cursore resta sul punto in cui clicchi."
  - direction: right
    $name: "Trail direction"
    $name:it: "Direzione della scia"
    $options:
    - right: Destra
    - left: Sinistra
    - up: Alto
    - down: Basso
  - when: always
    $name: "Show Knight trail"
    $name:it: "Quando mostrare la scia Knight"
    $options:
    - always: Sempre, anche durante il ritorno
    - floating: Solo durante la fluttuazione
    - never: Mai
    $description:it: "Indipendente dallo stile della scia normale e dal colore arcobaleno."
  - spacing: 4
    $name: "Distance between copies (pixels, 1–40)"
    $name:it: "Distanza tra le copie (pixel, 1–40)"
    $description:it: "Distanza orizzontale tra due copie consecutive al 100% di scala."
  - copies: 7
    $name: "Cursor copies"
    $name:it: "Numero di copie (1–50)"
  - opacity: 65
    $name: "Opacity (percent)"
    $name:it: "Opacità (percentuale)"
  - wave: 100
    $name: "Wave intensity (percent)"
    $name:it: "Intensità dell’ondeggiamento (percentuale)"
  $name: "Idle trail"
  $name:it: "Scia durante l’idle"
- knightFloat: false
  $name: "Knight idle float"
  $name:it: "Fluttuazione idle Knight"
- knightDelay: 5
  $name: "Float after inactivity (seconds)"
  $name:it: "Fluttua dopo inattività (secondi, 0 = subito)"
- KnightClose:
  - lineWidth: 10
    $name: "Spessore massimo della linea nera (pixel, 1–40)"
  - glowWidth: 4
    $name: "Ampiezza iniziale alone rosso (pixel, 1–30)"
  - glowExpansion: 18
    $name: "Espansione aggiuntiva alone rosso (pixel, 0–80)"
  - lineFinal: 15
    $name: "Spessore residuo della linea prima del boom (percentuale, 0–100)"
    $description: "Valori più bassi fanno assottigliare maggiormente la linea."
  - fallMs: 700
    $name: "Durata della caduta (millisecondi, 200–3000)"
  - rotation: 100
    $name: "Rotazione dei frammenti (percentuale, 0–300)"
    $description: "Zero disattiva la rotazione."
  - wordSize: 100
    $name: "Dimensione SWOON (percentuale, 25–250)"
  - wordRise: 85
    $name: "Salita SWOON durante il fade (pixel, 0–300)"
  - expansionMs: 1200
    $name: "Durata espansione finale dei bordi (millisecondi)"
  - pauseMs: 150
    $name: "Pausa prima del boom (millisecondi)"
  - slashes: 3
    $name: "Numero di slash (1–25)"
  - interval: 200
    $name: "Intervallo tra gli slash (millisecondi)"
    $description: "Solo apparizione degli slash: 200 ms = 5 al secondo; 0 = tutti insieme. Non cambia durata o grandezza dell’espansione finale."
  - swoon: true
    $name: "Mostra SWOON"
    $description: "Si allunga solo in altezza, sale e svanisce."
  - enabled: true
    $name: "Taglio rosso alla chiusura"
    $description: "Con Knight attivo e durata 0, premendo X mostra una barra rossa e una fenditura prima di richiedere la chiusura. Le richieste di salvataggio restano attive."
  $name: "Chiusura Knight"
- KnightMotion:
  - duration: 0
    $name: "Durata della fluttuazione (secondi, 0 = senza limite)"
  - horizontal: 5
    $name: "Horizontal maximum distance (pixels, 0–200)"
    $name:it: "Spostamento massimo destra/sinistra (pixel, 0–200)"
    $description: "Distance from the resting position in each direction. Zero disables this axis."
    $description:it: "Distanza dalla posizione di riposo in ciascuna direzione. Zero disattiva questo asse."
  - vertical: 12
    $name: "Vertical maximum distance (pixels, 0–200)"
    $name:it: "Spostamento massimo su/giù (pixel, 0–200)"
    $description:it: "Distanza dalla posizione di riposo in ciascuna direzione. Zero disattiva questo asse."
  - horizontalSpeed: 100
    $name: "Horizontal speed (percent, 1–1000)"
    $name:it: "Velocità destra/sinistra (percentuale, 1–1000)"
  - verticalSpeed: 100
    $name: "Vertical speed (percent, 1–1000)"
    $name:it: "Velocità su/giù (percentuale, 1–1000)"
  $name: "Knight motion"
  $name:it: "Movimento Knight"
- springBounce: 65
  $name: "Spring bounce (percent)"
  $name:it: "Rimbalzo (percentuale)"
- trailStyle: classic-remake
  $name: "Pointer trail style"
  $name:it: "Stile della scia del puntatore"
  $options:
  - classic-remake: Classic Remake
  - modern: Modern
  - off: Off
- hideWhenFast: true
  $name: "Hide native pointer during fast motion"
  $name:it: "Nascondi il puntatore nativo durante i movimenti veloci"
- alwaysHideCursor: false
  $name: "Always hide cursor"
  $name:it: "Always hide cursor"
  $description: "Keep the native cursor hidden while the replacement is ready. If rendering is unavailable or the desktop changes, the native cursor returns."
  $description:it: "Nasconde il cursore nativo solo quando la copia animata è disponibile. Se manca o cambia il desktop, torna il cursore normale."
- hideSpeed: 100
  $name: "Trail hide threshold"
  $name:it: "Soglia del movimento per la scia"
- pauseInFullscreen: true
  $name: "Pause in fullscreen applications"
  $name:it: "Sospendi nelle applicazioni a schermo intero"
- Typing:
  - trails: true
    $name: "Show trails while typing"
    $name:it: "Mostra le scie durante la scrittura"
    $description: "Uses the selected trail style while following the text caret."
    $description:it: "Usa lo stile di scia selezionato mentre il puntatore segue il testo."
  - originShadow: true
    $name: "Show mouse shadow while typing"
    $name:it: "Mostra l’ombra nella posizione reale del mouse durante la scrittura"
  - hideNativeCaret: true
    $name: "Hide original text caret (supported Windows fields)"
    $name:it: "Nascondi la barretta originale (campi Windows supportati)"
  - enabled: true
    $name: "Follow the text caret while typing"
    $name:it: "Segui il punto di scrittura"
  - returnDuration: 320
    $name: "Return animation duration (milliseconds)"
    $name:it: "Durata del ritorno al mouse (millisecondi)"
  - fadePeriod: 1000
    $name: "Idle pulse duration (milliseconds)"
    $name:it: "Durata della pulsazione in pausa (millisecondi)"
  $name: "Typing"
  $name:it: "Scrittura"
- Dynamics:
  - reverseEnabled: true
    $name: "Rimbalzo quando inverti direzione"
  - reverseAmount: 8
    $name: "Compressione al cambio di direzione (0–20%)"
  - reverseDuration: 220
    $name: "Durata del rimbalzo (80–500 ms)"
  - trailShrink: true
    $name: "Restringi le scie durante la dissolvenza"
  - trailShrinkAmount: 25
    $name: "Riduzione massima delle scie (0–60%)"
  - idleReturn: true
    $name: "Ritorno elastico dall’idle Knight"
  $name: "Dinamica e dissolvenza"
- Movement:
  - tiltEnabled: true
    $name: "Movement tilt"
    $name:it: "Inclinazione durante il movimento"
  - tiltAmount: 23
    $name: "Maximum movement tilt (degrees)"
    $name:it: "Inclinazione massima in movimento (gradi)"
  - rotationPivot: "center"
    $name: "Rotation pivot"
    $name:it: "Fulcro della rotazione"
    $options:
    - hotspot: Cursor tip
    - center: Center
    - bottom-right: Bottom right
  - verticalTilt: 23
    $name: "Up/down movement tilt (degrees)"
    $name:it: "Inclinazione nei movimenti su/giù (gradi)"
  - stretchEnabled: true
    $name: "Stretch in the movement direction"
    $name:it: "Allunga nella direzione del movimento"
  - stretchAmount: 100
    $name: "Maximum movement stretch (percent)"
    $name:it: "Allungamento massimo in movimento (percentuale)"
  - stretchSpeed: 1800
    $name: "Speed for maximum stretch (pixels per second)"
    $name:it: "Velocità per l'allungamento massimo (pixel al secondo)"
  $name: "Movement"
  $name:it: "Movimento"
- Press:
  - selectionEnabled: true
    $name: "Rotate while holding the left button"
    $name:it: "Ruota mentre tieni premuto il tasto sinistro"
  - selectionAngle: -20
    $name: "Held rotation (degrees)"
    $name:it: "Inclinazione mentre tieni premuto (gradi)"
  - skipResizeRotation: true
    $name: "Do not rotate resize cursors while pressing"
    $name:it: "Non ruotare i cursori di ridimensionamento mentre premi"
  - skipTextRotation: false
    $name: "Do not rotate the text cursor while pressing"
    $name:it: "Non ruotare il cursore di testo mentre premi"
  - skipBusyRotation: true
    $name: "Do not rotate busy cursors while pressing"
    $name:it: "Non ruotare i cursori di attesa mentre premi"
  - pressureEnabled: true
    $name: "Compress while pressing"
    $name:it: "Comprimi mentre tieni premuto"
  - pressureAmount: 10
    $name: "Press compression (percent)"
    $name:it: "Compressione durante la pressione (percentuale)"
  - clickNudge: true
    $name: "Spring nudge when clicking"
    $name:it: "Piccolo scatto elastico al clic"
  - clickNudgeAmount: 2
    $name: "Click nudge distance (pixels)"
    $name:it: "Spostamento visivo al clic (pixel)"
  $name: "Click and hold"
  $name:it: "Clic e pressione"
- Shape:
  - shapeTransition: true
    $name: "Smooth cursor shape changes"
    $name:it: "Transizione fluida tra le forme del cursore"
  - shapeTransitionDuration: 100
    $name: "Shape transition duration (milliseconds)"
    $name:it: "Durata del cambio di forma (millisecondi)"
  - shapeElastic: true
    $name: "Elastic cursor shape change"
    $name:it: "Cambio di forma elastico"
  - shapeElasticAmount: 12
    $name: "Shape compression (percent)"
    $name:it: "Compressione al cambio di forma (percentuale)"
  - shapeBounce: true
    $name: "Bounce when the cursor shape changes"
    $name:it: "Inclinazione e rimbalzo quando cambia il cursore"
  $name: "Cursor shape changes"
  $name:it: "Cambio di forma"
- Scroll:
  - wheelEnabled: true
    $name: "Scroll wheel impulse"
    $name:it: "Impulso della rotellina"
  - wheelAmount: 4
    $name: "Scroll impulse (pixels)"
    $name:it: "Ampiezza dell'impulso di scorrimento (pixel)"
  - wheelElastic: true
    $name: "Elastic scroll compression"
    $name:it: "Compressione elastica durante lo scroll"
  - wheelElasticAmount: 10
    $name: "Scroll compression (percent)"
    $name:it: "Compressione dello scroll (percentuale)"
  $name: "Scroll"
  $name:it: "Scroll"
- CloseHover:
  - closeMouseEffect: false
    $name: "Effetto del cursore sulla X"
    $description: "Trasformazione del cursore e luce sul pulsante X. Separato dai tagli Knight."
  - enabled: true
    $name: "White pointer and expansion on Close"
    $name:it: "Cursore bianco sulla X ed espansione alla pressione"
  - ripples: true
    $name: "Soft transparency circles"
    $name:it: "Cerchi sfumati di trasparenza"
  - rippleDepth: 60
    $name: "Circle transparency strength (percent)"
    $name:it: "Intensità della trasparenza dei cerchi (percentuale)"
  - rays: false
    $name: "Light streaks on press"
    $name:it: "Linee luminose alla pressione"
  - pulseStrength: 100
    $name: "Flash and darkening intensity (percent)"
    $name:it: "Intensità del lampo e dello scurimento (percentuale)"
  - titleDrag: true
    $name: "Expand while dragging the title bar"
    $name:it: "Espandi durante il trascinamento dalla barra del titolo"
  - opacity: 22
    $name: "Highlight opacity (percent)"
    $name:it: "Opacità dell’effetto (percentuale)"
  - duration: 350
    $name: "Transition duration (milliseconds)"
    $name:it: "Durata della transizione (millisecondi)"
  $name: "Close button hover"
  $name:it: "Effetti sui pulsanti finestra e sul titolo"
- Context:
  - magnetEnabled: true
    $name: "Visual magnetism on buttons"
    $name:it: "Magnetismo visivo sui pulsanti"
  - magnetStretch: 10
    $name: "Magnetic deformation (percent)"
    $name:it: "Deformazione magnetica (percentuale)"
  - magnetAmount: 8
    $name: "Maximum magnetic tilt (degrees)"
    $name:it: "Inclinazione magnetica massima (gradi)"
  - dragEnabled: true
    $name: "Drag resistance"
    $name:it: "Resistenza durante il trascinamento"
  - dragAmount: 4
    $name: "Maximum visual drag lag (pixels)"
    $name:it: "Ritardo visivo massimo durante il trascinamento (pixel)"
  - resizeEnabled: true
    $name: "Stretch along resize edges"
    $name:it: "Allungamento lungo i bordi di ridimensionamento"
  - resizeAmount: 12
    $name: "Resize stretch (percent)"
    $name:it: "Allungamento durante il ridimensionamento (percentuale)"
  $name: "Buttons, dragging and resizing"
  $name:it: "Pulsanti, trascinamento e ridimensionamento"
- Idle:
  - idleEnabled: true
    $name: "Random idle animations"
    $name:it: "Animazioni casuali quando il mouse è fermo"
  - idleMinSeconds: 8
    $name: "Minimum idle interval (seconds)"
    $name:it: "Intervallo minimo di inattività (secondi)"
  - idleMaxSeconds: 25
    $name: "Maximum idle interval (seconds)"
    $name:it: "Intervallo massimo di inattività (secondi)"
  - idleDuration: 1600
    $name: "Idle animation duration (milliseconds)"
    $name:it: "Durata dell'animazione idle (millisecondi)"
  - idleAmount: 8
    $name: "Idle movement distance (pixels)"
    $name:it: "Ampiezza dei movimenti idle (pixel)"
  - idleSpin: true
    $name: "Idle spin"
    $name:it: "Giro su sé stesso"
  - idleFigure8: true
    $name: "Idle figure eight"
    $name:it: "Movimento a forma di otto"
  - idleLook: true
    $name: "Idle look around"
    $name:it: "Guardarsi intorno"
  - idleArrowOnly: true
    $name: "Idle animations only for arrow and hand cursors"
    $name:it: "Animazioni idle solo per freccia e manina"
  $name: "Idle animations"
  $name:it: "Animazioni di inattività"
- Busy:
  - busyHop: true
    $name: "Hop while busy or working"
    $name:it: "Saltella durante l'attesa o il lavoro in corso"
  - busyHopAmount: 4
    $name: "Busy hop height (pixels)"
    $name:it: "Altezza del saltello di attesa (pixel)"
  - busyHopDuration: 800
    $name: "Busy hop interval (milliseconds)"
    $name:it: "Intervallo dei saltelli di attesa (millisecondi)"
  - busyHopElastic: true
    $name: "Elastic busy hop"
    $name:it: "Saltello di attesa elastico"
  - busyHopElasticAmount: 12
    $name: "Busy hop stretch and squash (percent)"
    $name:it: "Allungamento e compressione del saltello (percentuale)"
  $name: "Busy and working"
  $name:it: "Attesa e lavoro in corso"
- ClassicTrail:
  - classicCopies: 25
    $name: "Classic Remake copies"
    $name:it: "Copie di Classic Remake"
  - classicDuration: 250
    $name: "Classic Remake length (milliseconds)"
    $name:it: "Lunghezza di Classic Remake (millisecondi)"
  - classicOpacity: 75
    $name: "Classic Remake opacity (percent)"
    $name:it: "Opacità di Classic Remake (percentuale)"
  $name: "Classic Remake"
  $name:it: "Classic Remake"
- ModernTrail:
  - opacity: 100
    $name: "Trail opacity (0 disables afterimages)"
    $name:it: "Opacità della scia (0 disattiva le copie)"
  - trailMs: 65
    $name: "Trail duration (milliseconds)"
    $name:it: "Durata della scia (millisecondi)"
  $name: "Modern"
  $name:it: "Modern"
*/
// ==/WindhawkModSettings==

// How it works. Three threads:
//   sampler  reads the cursor position on every raw mouse report and decides when the real cursor should be hidden
//   hider    uses MagShowSystemCursor without replacing system cursor images
//   render   works out the fading copies along the recent path, blends them by hand into one bitmap and shows it in a
//            click-through layered window; it owns that window, which also receives the raw mouse input

#include <windows.h>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <atomic>
#include <uiautomation.h>
#include <mmsystem.h>
#include <shellapi.h>
#include <sddl.h>
#include <shellscalingapi.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define HOLD_MS        10      // how long after the last fast moment the cursor may come back
#define SHOW_WINDOW_MS 12      // the speed that brings the cursor back is measured over just this many ms
// ~px of travel one copy covers at cursor size 32 (keeps the opacity independent of copy density)
#define COVER_PX       10.0
#define MAX_SAMPLES    512
#define MAX_COPIES     100     // the most copies spread along the trail in one frame
#define COPY_BUF       (MAX_SAMPLES + MAX_COPIES)
#define MAX_SPRITES    16      // cursor pictures kept before the cache is emptied
#define WM_APP_PAUSED  (WM_APP + 1)

struct Sample { int x, y; LONGLONG t; };
struct Sprite { int w, h, hx, hy, id; UINT32* px; };   // cursor picture at its own size: premultiplied BGRA, hotspot
struct Copy { int x, y; float a; };                   // one copy: top-left relative to the frame, opacity 0-1

// settings (written by WhTool_ModSettingsChanged, read by the threads)
static std::atomic<bool> g_tiltEnabled{true},g_selectionEnabled{true},g_pointerReady{false},g_animating{false};
static std::atomic<bool> g_shapeBounce{true},g_pressureEnabled{true},g_shapeTransition{true},g_transitionActive{false};
static std::atomic<bool> g_skipResizeRotation{true},g_skipTextRotation{false},g_skipBusyRotation{true};
static constexpr bool g_alwaysVisible=true;
static std::atomic<bool> g_alwaysHideCursor{false};
static std::atomic<bool> g_protectedDesktop{false};
static bool PointerDesktopIsActive(){
 HDESK input=OpenInputDesktop(0,FALSE,DESKTOP_READOBJECTS);
 if(!input)return false; // Secure desktop access is normally denied.
 wchar_t inputName[256]{},ownName[256]{};DWORD needed=0;
 bool ok=GetUserObjectInformationW(input,UOI_NAME,inputName,sizeof(inputName),&needed)&&
         GetUserObjectInformationW(GetThreadDesktop(GetCurrentThreadId()),UOI_NAME,ownName,sizeof(ownName),&needed);
 CloseDesktop(input);
 return ok&&wcscmp(inputName,ownName)==0;
}
static std::atomic<bool> g_overlayReady{false};
static SRWLOCK g_nativeVisibilityLock=SRWLOCK_INIT;
static std::atomic<bool> g_idleEnabled{true},g_idleSpin{true},g_idleFigure8{true},g_idleLook{true},g_idleArrowOnly{true},g_idleActive{false},g_idleReset{true};
static std::atomic<int> g_idleMinSeconds{8},g_idleMaxSeconds{25},g_idleDuration{1600},g_idleAmount{8},g_idleKind{0};
static std::atomic<LONGLONG> g_idleStart{0};static std::atomic<unsigned> g_idleInputGeneration{0};
static std::atomic<bool> g_busyHop{true},g_busyActive{false};static std::atomic<int> g_busyHopAmount{4},g_busyHopDuration{800};
static std::atomic<bool> g_busyHopElastic{true};static std::atomic<int> g_busyHopElasticAmount{12};
static std::atomic<int> g_transitionDuration{100};
static std::atomic<bool> g_shapeElastic{true},g_clickNudge{true};
static std::atomic<int> g_shapeElasticAmount{12},g_clickNudgeAmount{2};
static std::atomic<double> g_clickNudgeX{0},g_clickNudgeY{1};
static SRWLOCK g_buttonLock=SRWLOCK_INIT;
static RECT g_buttonBounds{};static LONGLONG g_buttonBoundsTime=-1000;
static std::atomic<int> g_pressureAmount{8},g_rotationPivot{1};
static std::atomic<bool> g_stretchEnabled{true};
static std::atomic<bool> g_reverseElastic{true},g_trailShrink{true},g_idleReturnElastic{true};
static std::atomic<int> g_reverseAmount{8},g_reverseDuration{220},g_trailShrinkAmount{25};
static std::atomic<bool> g_magnetEnabled{true},g_wheelEnabled{true},g_dragEnabled{true},g_resizeEnabled{true};
static std::atomic<int> g_magnetAmount{8},g_wheelAmount{4},g_dragAmount{4},g_resizeAmount{12},g_resizeAxis{0};
static std::atomic<bool> g_closeEnabled{true},g_closeVisual{false},g_titleDragWhite{true};
static std::atomic<int> g_closeOpacity{75},g_closeDuration{140};
static SRWLOCK g_closeLock=SRWLOCK_INIT;static RECT g_closeTarget{};static LONGLONG g_closeTime=0;
static std::atomic<bool> g_titleHoverWhite{false},g_pressRays{true};
static std::atomic<int> g_pulseStrength{100};
static std::atomic<bool> g_softRipples{true};
static std::atomic<int> g_rippleDepth{60};
static double g_rippleAge=0;
static double g_hoverWhiteMix=0,g_whiteFlash=0,g_darkPulse=0,g_rayPulse=0;
static std::atomic<HWND> g_whiteFollowWindow{nullptr};
static HWND g_closeSource=nullptr;
static std::atomic<bool> g_closeMouseEffect{false};static int g_knightFloor=0;static RECT g_knightSnapshotRect{};static std::vector<UINT32> g_knightSnapshot;static std::vector<WORD> g_knightPieces;static bool g_knightWindowHidden=false;
static std::atomic<int> g_knightSlashCount{3},g_knightSlashInterval{200},g_knightExpansionMs{1200},g_knightPauseMs{150};static std::atomic<bool> g_knightSwoon{true};static int g_knightRunCount=3,g_knightRunInterval=200,g_knightRunExpansion=1200,g_knightRunPause=150;
static double g_pieceCenterX[352]{},g_pieceCenterY[352]{};
static std::atomic<int> g_knightLineWidth{10},g_knightGlowWidth{4},g_knightGlowExpansion{18},g_knightLineFinal{15},g_knightFallMs{700},g_knightRotation{100},g_knightWordSize{100},g_knightWordRise{85};
static int KnightFallDuration(){return g_knightFallMs.load();}
static int KnightCutsEnd(){return (g_knightRunCount-1)*g_knightRunInterval+200;}
static int KnightFallStart(){return KnightCutsEnd()+g_knightRunExpansion+g_knightRunPause;}
static double KnightCutSigned(int cut,double x,double y,double w,double h){
 struct Cuts{double a[25],b[25],cx[25],cy[25];Cuts(){double aa[6]={1,.28,1,-.55,1,.70},bb[6]={-.30,1,.65,1,-.75,1},xx[6]={.5,.5,.36,.65,.66,.25},yy[6]={0,.32,0,.55,0,.65};for(int i=0;i<25;++i){double ax=i<6?aa[i]:cos(i*2.399963),by=i<6?bb[i]:sin(i*2.399963),length=hypot(ax,by);a[i]=ax/length;b[i]=by/length;cx[i]=(i<6?xx[i]:.5+.30*sin(i*1.7))*a[i];cy[i]=(i<6?yy[i]:.5+.30*cos(i*2.3))*b[i];}}};static const Cuts cuts;
 return x*cuts.a[cut]+y*cuts.b[cut]-w*cuts.cx[cut]-h*cuts.cy[cut];
}
static UINT32 KnightPieceAt(double x,double y,double w,double h){UINT32 piece=0;for(int i=0;i<g_knightRunCount;++i)if(KnightCutSigned(i,x,y,w,h)>0)piece|=1u<<i;return piece;}

static bool CaptureKnightWindow(HWND window){
 RECT r{};if(!GetWindowRect(window,&r))return false;int w=r.right-r.left,h=r.bottom-r.top;if(w<=0||h<=0||size_t(w)*h>12000000)return false;
 HDC screen=GetDC(nullptr),dc=CreateCompatibleDC(screen);BITMAPINFO info{};info.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);info.bmiHeader.biWidth=w;info.bmiHeader.biHeight=-h;info.bmiHeader.biPlanes=1;info.bmiHeader.biBitCount=32;void* bits=nullptr;
 HBITMAP bitmap=CreateDIBSection(screen,&info,DIB_RGB_COLORS,&bits,nullptr,0);bool ok=false;
 if(bitmap&&dc&&bits){auto old=SelectObject(dc,bitmap);if(BitBlt(dc,0,0,w,h,screen,r.left,r.top,SRCCOPY)){g_knightSnapshot.assign((UINT32*)bits,(UINT32*)bits+size_t(w)*h);g_knightPieces.resize(size_t(w)*h);double counts[352]{};std::unordered_map<UINT32,WORD> regions;std::fill_n(g_pieceCenterX,352,0);std::fill_n(g_pieceCenterY,352,0);for(int y=0;y<h;++y)for(int x=0;x<w;++x){size_t i=size_t(y)*w+x;g_knightSnapshot[i]|=0xFF000000;UINT32 mask=KnightPieceAt(x,y,w,h);auto found=regions.find(mask);WORD piece;if(found==regions.end()){piece=WORD(regions.size());if(piece>=352){SelectObject(dc,old);DeleteObject(bitmap);DeleteDC(dc);ReleaseDC(nullptr,screen);g_knightSnapshot.clear();g_knightPieces.clear();return false;}regions.emplace(mask,piece);}else piece=found->second;g_knightPieces[i]=piece;g_pieceCenterX[piece]+=x;g_pieceCenterY[piece]+=y;counts[piece]++;}for(int i=0;i<352;++i)if(counts[i]){g_pieceCenterX[i]/=counts[i];g_pieceCenterY[i]/=counts[i];}g_knightSnapshotRect=r;MONITORINFO monitor{sizeof(monitor)};g_knightFloor=GetMonitorInfoW(MonitorFromRect(&r,MONITOR_DEFAULTTONEAREST),&monitor)?monitor.rcMonitor.bottom:GetSystemMetrics(SM_CYSCREEN);ok=true;}SelectObject(dc,old);}
 if(bitmap)DeleteObject(bitmap);if(dc)DeleteDC(dc);if(screen)ReleaseDC(nullptr,screen);return ok;
}
static HWND g_knightCloseWindow=nullptr;static LONGLONG g_knightCloseStart=0;static bool g_knightCloseDraw=false;static std::atomic<bool> g_knightCloseEnabled{true};
static bool g_closeButtonDraw=false;static int g_captionTargetType=1,g_captionDrawType=1;
static RECT g_closeDrawRect{};static double g_closeMix=0,g_closeRadiusPhase=0;static POINT g_closeMouse{};
static std::atomic<double> g_magnetX{0},g_magnetY{0};
static std::atomic<int> g_magnetStretch{10};
static std::atomic<double> g_magnetAngle{0};static std::atomic<LONGLONG> g_wheelTime{0};static std::atomic<int> g_wheelDirection{0};
static std::atomic<bool> g_wheelElastic{true},g_wheelHorizontal{false};static std::atomic<int> g_wheelElasticAmount{10};
static std::atomic<int> g_stretchAmount{10},g_stretchSpeed{1800};
static bool g_angleAnimating=false; // render thread only
static std::atomic<LONGLONG> g_clickPulse{-1000},g_shapePulse{0};
static std::atomic<int> g_tiltAmount{18},g_verticalTilt{14},g_selectionAngle{-90},g_springBounce{65};
static std::atomic<int> g_opacity, g_trailMs, g_hideWhenFast, g_hideSpeedCm, g_pauseInFullscreen;
static std::atomic<int> g_trailStyle{1},g_classicCopies{7},g_classicDuration{120},g_classicOpacity{65};

// shared between the threads (guarded by g_gate)
static CRITICAL_SECTION g_gate;
static Sample g_hist[MAX_SAMPLES];
static int g_histN;
static HCURSOR g_curHandle;
static bool g_curVisible, g_hideWanted;
static int g_monCursor = 32;          // cursor width on the monitor the cursor is on
static std::atomic<int> g_monGap{7};     // ms between frames: half a refresh of the monitor the cursor is on
static std::atomic<bool> g_blankActive;  // the system cursors are currently swapped for an invisible one
static std::atomic<bool> g_drawFailed;   // the overlay could not draw: then never hide the real cursor
static std::atomic<bool> g_monStale, g_spritesStale;   // display or cursor settings changed
static std::atomic<bool> g_recheck;      // settings or the foreground window changed: check for fullscreen apps now
static std::atomic<bool> g_paused;       // a fullscreen app is in front
// the window in front runs with more rights: Windows sends us no raw input then
static std::atomic<bool> g_adminFront;
static std::atomic<bool> g_stop;
static std::atomic<HWND> g_wnd;          // the overlay window (owned by the render thread)

static HANDLE g_moved, g_mouseWake, g_hideChanged;   // auto-reset events
static HANDLE g_ready;                               // set when the render thread has set up (or failed to)
static HANDLE g_threads[4];
static std::atomic<bool> g_typingHideNative{true},g_typingOriginShadow{true},g_typingTrails{true};
static std::atomic<bool> g_typingEnabled{true},g_typingRequested{false},g_typingAttached{false},g_typingVisual{false};
static std::atomic<int> g_typingReturnDuration{320},g_typingFadePeriod{1000};
static std::atomic<LONGLONG> g_typingKeyTime{-100000};
static std::atomic<unsigned> g_typingGeneration{0};
static SRWLOCK g_typingLock=SRWLOCK_INIT;
static POINT g_typingTarget{};
static std::atomic<int> g_trailColorMode{1},g_trailColorSpeed{100},g_idleTrailLength{96},g_idleTrailSpacing{4},g_idleTrailCopies{7},g_idleTrailOpacity{65},g_idleTrailWave{100},g_idleTrailWhen{0};
static std::atomic<bool> g_idleTrailPinned{false};static std::atomic<int> g_idleTrailDirection{0};
static std::atomic<UINT32> g_trailCustomColor{0x00BFFF},g_cursorColor{0x00BFFF};
static std::atomic<int> g_cursorColorMode{0};static std::atomic<bool> g_cursorColorFull{false};static bool g_drawBusyCursor=false;
static std::atomic<bool> g_rainbowTrail{false},g_knightEnabled{false},g_knightActive{false},g_knightVisual{false};
static std::atomic<int> g_knightDuration{0},g_knightDelay{5},g_knightAmount{12},g_knightHorizontal{5},g_knightVertical{12},g_knightHorizontalSpeed{100},g_knightVerticalSpeed{100};
static std::atomic<LONGLONG> g_knightStart{0};

static LARGE_INTEGER g_freq, g_start;

static LONGLONG Now() {
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    return (t.QuadPart - g_start.QuadPart) * 1000 / g_freq.QuadPart;
}

// CURSOR_SHOWING reflects the application's ShowCursor display count.
// MagShowSystemCursor does not own that count: never use our suppression flag
// as permission to draw an application-hidden cursor.
static bool ApplicationShowsCursor(const CURSORINFO& info){
 return info.hCursor&&(info.flags&CURSOR_SHOWING)&&!(info.flags&CURSOR_SUPPRESSED);
}

static double Dist(Sample a, Sample b) {
    double dx = b.x - a.x, dy = b.y - a.y;
    return sqrt(dx * dx + dy * dy);
}

static int Clamp(int v, int lo, int hi) { return v < lo ? lo : v > hi ? hi : v; }

static void LoadSettings() {
    const wchar_t* colorMode=Wh_GetStringSetting(L"TrailColor.mode");
    g_trailColorMode=colorMode&&*colorMode ? (_wcsicmp(colorMode,L"custom")==0?2:_wcsicmp(colorMode,L"rainbow")==0?1:0) : (Wh_GetIntSetting(L"rainbowTrail")?1:0);
    Wh_FreeStringSetting(colorMode);
    const wchar_t* customColor=Wh_GetStringSetting(L"TrailColor.custom");
    if(customColor){const wchar_t* value=customColor[0]==L'#'?customColor+1:customColor;wchar_t* end=nullptr;unsigned long parsed=wcstoul(value,&end,16);if(wcslen(value)==6&&end&&!*end&&parsed<=0xFFFFFF)g_trailCustomColor=UINT32(parsed);}
    Wh_FreeStringSetting(customColor);
    g_trailColorSpeed=Clamp(Wh_GetIntSetting(L"TrailColor.speed"),1,2000);
    g_idleTrailLength=Clamp(Wh_GetIntSetting(L"IdleTrail.length"),0,400);
    g_idleTrailCopies=Clamp(Wh_GetIntSetting(L"IdleTrail.copies"),1,50);
    g_idleTrailSpacing=Clamp(Wh_GetIntSetting(L"IdleTrail.spacing"),1,40);
    g_idleTrailOpacity=Clamp(Wh_GetIntSetting(L"IdleTrail.opacity"),0,100);
    g_idleTrailWave=Clamp(Wh_GetIntSetting(L"IdleTrail.wave"),0,300);
    g_idleTrailPinned=Wh_GetIntSetting(L"IdleTrail.pinCursor")!=0;
    const wchar_t* direction=Wh_GetStringSetting(L"IdleTrail.direction");
    g_idleTrailDirection=direction&&_wcsicmp(direction,L"left")==0?1:direction&&_wcsicmp(direction,L"up")==0?2:direction&&_wcsicmp(direction,L"down")==0?3:0;
    Wh_FreeStringSetting(direction);

    const wchar_t* idleWhen=Wh_GetStringSetting(L"IdleTrail.when");
    g_idleTrailWhen=idleWhen&&_wcsicmp(idleWhen,L"never")==0?2:idleWhen&&_wcsicmp(idleWhen,L"floating")==0?1:0;
    Wh_FreeStringSetting(idleWhen);
    const wchar_t* cursorMode=Wh_GetStringSetting(L"CursorColor.mode");
    g_cursorColorMode=cursorMode&&_wcsicmp(cursorMode,L"accent")==0?1:cursorMode&&_wcsicmp(cursorMode,L"custom")==0?2:0;Wh_FreeStringSetting(cursorMode);
    g_cursorColorFull=Wh_GetIntSetting(L"CursorColor.full")!=0;
    const wchar_t* cursorHex=Wh_GetStringSetting(L"CursorColor.custom");
    if(cursorHex){const wchar_t* v=cursorHex[0]==L'#'?cursorHex+1:cursorHex;wchar_t* end=nullptr;unsigned long c=wcstoul(v,&end,16);if(wcslen(v)==6&&end&&!*end)g_cursorColor=UINT32(c)&0xFFFFFF;Wh_FreeStringSetting(cursorHex);}
    if(g_cursorColorMode==1){DWORD accent=0,bytes=sizeof(accent);if(RegGetValueW(HKEY_CURRENT_USER,L"Software\\Microsoft\\Windows\\DWM",L"ColorizationColor",RRF_RT_REG_DWORD,nullptr,&accent,&bytes)==ERROR_SUCCESS)g_cursorColor=accent&0xFFFFFF;}
    g_knightCloseEnabled=Wh_GetIntSetting(L"KnightClose.enabled")!=0;
    g_knightLineWidth=Clamp(Wh_GetIntSetting(L"KnightClose.lineWidth"),1,40);g_knightGlowWidth=Clamp(Wh_GetIntSetting(L"KnightClose.glowWidth"),1,30);g_knightGlowExpansion=Clamp(Wh_GetIntSetting(L"KnightClose.glowExpansion"),0,80);g_knightLineFinal=Clamp(Wh_GetIntSetting(L"KnightClose.lineFinal"),0,100);
    g_knightFallMs=Clamp(Wh_GetIntSetting(L"KnightClose.fallMs"),200,3000);g_knightRotation=Clamp(Wh_GetIntSetting(L"KnightClose.rotation"),0,300);g_knightWordSize=Clamp(Wh_GetIntSetting(L"KnightClose.wordSize"),25,250);g_knightWordRise=Clamp(Wh_GetIntSetting(L"KnightClose.wordRise"),0,300);

    g_knightExpansionMs=Clamp(Wh_GetIntSetting(L"KnightClose.expansionMs"),100,4000);g_knightPauseMs=Clamp(Wh_GetIntSetting(L"KnightClose.pauseMs"),0,1000);
    g_knightSlashCount=Clamp(Wh_GetIntSetting(L"KnightClose.slashes"),1,25);g_knightSlashInterval=Clamp(Wh_GetIntSetting(L"KnightClose.interval"),0,1000);g_knightSwoon=Wh_GetIntSetting(L"KnightClose.swoon")!=0;
    g_knightEnabled=Wh_GetIntSetting(L"knightFloat")!=0;
    g_knightDelay=Clamp(Wh_GetIntSetting(L"knightDelay"),0,300);
    g_knightDuration=Clamp(Wh_GetIntSetting(L"KnightMotion.duration"),0,3600);
    g_knightAmount=Clamp(Wh_GetIntSetting(L"knightAmount"),1,40);
    g_knightHorizontal=Clamp(Wh_GetIntSetting(L"KnightMotion.horizontal"),0,200);
    g_knightVertical=Clamp(Wh_GetIntSetting(L"KnightMotion.vertical"),0,200);
    g_knightHorizontalSpeed=Clamp(Wh_GetIntSetting(L"KnightMotion.horizontalSpeed"),1,1000);
    g_knightVerticalSpeed=Clamp(Wh_GetIntSetting(L"KnightMotion.verticalSpeed"),1,1000);

    g_typingEnabled=Wh_GetIntSetting(L"Typing.enabled")!=0;
    g_typingHideNative=Wh_GetIntSetting(L"Typing.hideNativeCaret")!=0;
    g_typingOriginShadow=Wh_GetIntSetting(L"Typing.originShadow")!=0;
    g_typingTrails=Wh_GetIntSetting(L"Typing.trails")!=0;
    g_typingReturnDuration=Clamp(Wh_GetIntSetting(L"Typing.returnDuration"),100,1000);
    g_typingFadePeriod=Clamp(Wh_GetIntSetting(L"Typing.fadePeriod"),400,3000);

    g_alwaysHideCursor=Wh_GetIntSetting(L"alwaysHideCursor")!=0;
    g_idleEnabled=Wh_GetIntSetting(L"Idle.idleEnabled")!=0;
    int idleLo=Clamp(Wh_GetIntSetting(L"Idle.idleMinSeconds"),1,300),idleHi=Clamp(Wh_GetIntSetting(L"Idle.idleMaxSeconds"),1,300);
    g_idleMinSeconds=std::min(idleLo,idleHi);g_idleMaxSeconds=std::max(idleLo,idleHi);
    g_idleDuration=Clamp(Wh_GetIntSetting(L"Idle.idleDuration"),600,5000);g_idleAmount=Clamp(Wh_GetIntSetting(L"Idle.idleAmount"),0,24);
    g_idleSpin=Wh_GetIntSetting(L"Idle.idleSpin")!=0;g_idleFigure8=Wh_GetIntSetting(L"Idle.idleFigure8")!=0;g_idleLook=Wh_GetIntSetting(L"Idle.idleLook")!=0;g_idleArrowOnly=Wh_GetIntSetting(L"Idle.idleArrowOnly")!=0;
    g_busyHop=Wh_GetIntSetting(L"Busy.busyHop")!=0;g_busyHopAmount=Clamp(Wh_GetIntSetting(L"Busy.busyHopAmount"),0,16);g_busyHopDuration=Clamp(Wh_GetIntSetting(L"Busy.busyHopDuration"),300,2500);
    g_busyHopElastic=Wh_GetIntSetting(L"Busy.busyHopElastic")!=0;g_busyHopElasticAmount=Clamp(Wh_GetIntSetting(L"Busy.busyHopElasticAmount"),0,30);
    g_idleActive=false;g_idleReset=true;
    g_skipResizeRotation=Wh_GetIntSetting(L"Press.skipResizeRotation")!=0;
    g_skipTextRotation=Wh_GetIntSetting(L"Press.skipTextRotation")!=0;
    g_skipBusyRotation=Wh_GetIntSetting(L"Press.skipBusyRotation")!=0;
    g_shapeElastic=Wh_GetIntSetting(L"Shape.shapeElastic")!=0;g_shapeElasticAmount=Clamp(Wh_GetIntSetting(L"Shape.shapeElasticAmount"),0,25);
    g_clickNudge=Wh_GetIntSetting(L"Press.clickNudge")!=0;g_clickNudgeAmount=Clamp(Wh_GetIntSetting(L"Press.clickNudgeAmount"),0,6);
    g_softRipples=Wh_GetIntSetting(L"CloseHover.ripples")!=0;g_rippleDepth=Clamp(Wh_GetIntSetting(L"CloseHover.rippleDepth"),0,100);
    g_pressRays=Wh_GetIntSetting(L"CloseHover.rays")!=0;g_pulseStrength=Clamp(Wh_GetIntSetting(L"CloseHover.pulseStrength"),0,200);
    g_titleDragWhite=Wh_GetIntSetting(L"CloseHover.titleDrag")!=0;
    g_closeEnabled=Wh_GetIntSetting(L"CloseHover.enabled")!=0;
    g_closeMouseEffect=Wh_GetIntSetting(L"CloseHover.closeMouseEffect")!=0;
    g_closeOpacity=Clamp(Wh_GetIntSetting(L"CloseHover.opacity"),10,100);
    g_closeDuration=Clamp(Wh_GetIntSetting(L"CloseHover.duration"),40,500);
    g_magnetEnabled=Wh_GetIntSetting(L"Context.magnetEnabled")!=0;g_magnetAmount=Clamp(Wh_GetIntSetting(L"Context.magnetAmount"),0,20);g_magnetStretch=Clamp(Wh_GetIntSetting(L"Context.magnetStretch"),0,25);
    g_wheelEnabled=Wh_GetIntSetting(L"Scroll.wheelEnabled")!=0;g_wheelAmount=Clamp(Wh_GetIntSetting(L"Scroll.wheelAmount"),0,12);
    g_wheelElastic=Wh_GetIntSetting(L"Scroll.wheelElastic")!=0;g_wheelElasticAmount=Clamp(Wh_GetIntSetting(L"Scroll.wheelElasticAmount"),0,30);
    g_dragEnabled=Wh_GetIntSetting(L"Context.dragEnabled")!=0;g_dragAmount=Clamp(Wh_GetIntSetting(L"Context.dragAmount"),0,10);
    g_resizeEnabled=Wh_GetIntSetting(L"Context.resizeEnabled")!=0;g_resizeAmount=Clamp(Wh_GetIntSetting(L"Context.resizeAmount"),0,30);
    PCWSTR pivot=Wh_GetStringSetting(L"Movement.rotationPivot");
    g_rotationPivot=pivot&&wcscmp(pivot,L"bottom-right")==0?2:pivot&&wcscmp(pivot,L"hotspot")==0?0:1;
    if(pivot)Wh_FreeStringSetting(pivot);
    g_tiltEnabled=Wh_GetIntSetting(L"Movement.tiltEnabled")!=0;
    g_selectionEnabled=Wh_GetIntSetting(L"Press.selectionEnabled")!=0;
    g_shapeBounce=Wh_GetIntSetting(L"Shape.shapeBounce")!=0;
    g_shapeTransition=Wh_GetIntSetting(L"Shape.shapeTransition")!=0;g_transitionDuration=Clamp(Wh_GetIntSetting(L"Shape.shapeTransitionDuration"),20,300);
    g_pressureEnabled=Wh_GetIntSetting(L"Press.pressureEnabled")!=0;
    g_reverseElastic=Wh_GetIntSetting(L"Dynamics.reverseEnabled")!=0;
    g_reverseAmount=Clamp(Wh_GetIntSetting(L"Dynamics.reverseAmount"),0,20);
    g_reverseDuration=Clamp(Wh_GetIntSetting(L"Dynamics.reverseDuration"),80,500);
    g_trailShrink=Wh_GetIntSetting(L"Dynamics.trailShrink")!=0;
    g_trailShrinkAmount=Clamp(Wh_GetIntSetting(L"Dynamics.trailShrinkAmount"),0,60);
    g_idleReturnElastic=Wh_GetIntSetting(L"Dynamics.idleReturn")!=0;
    g_stretchEnabled=Wh_GetIntSetting(L"Movement.stretchEnabled")!=0;
    g_stretchAmount=Clamp(Wh_GetIntSetting(L"Movement.stretchAmount"),0,35);
    g_stretchSpeed=Clamp(Wh_GetIntSetting(L"Movement.stretchSpeed"),300,6000);
    g_pressureAmount=Clamp(Wh_GetIntSetting(L"Press.pressureAmount"),0,25);
    g_tiltAmount=Clamp(Wh_GetIntSetting(L"Movement.tiltAmount"),0,45);
    g_verticalTilt=Clamp(Wh_GetIntSetting(L"Movement.verticalTilt"),0,45);
    g_selectionAngle=Clamp(Wh_GetIntSetting(L"Press.selectionAngle"),-180,180);
    g_springBounce=Clamp(Wh_GetIntSetting(L"springBounce"),0,100);
    g_opacity = Clamp(Wh_GetIntSetting(L"ModernTrail.opacity"), 0, 100);
    auto trailStyle=Wh_GetStringSetting(L"trailStyle");
    g_trailStyle=wcscmp(trailStyle,L"classic-remake")==0?1:wcscmp(trailStyle,L"modern")==0?2:0;
    Wh_FreeStringSetting(trailStyle);
    g_classicCopies=Clamp(Wh_GetIntSetting(L"ClassicTrail.classicCopies"),2,20);
    g_classicDuration=Clamp(Wh_GetIntSetting(L"ClassicTrail.classicDuration"),40,250);
    g_classicOpacity=Clamp(Wh_GetIntSetting(L"ClassicTrail.classicOpacity"),0,100);
    g_trailMs = Clamp(Wh_GetIntSetting(L"ModernTrail.trailMs"), 10, 150);
    if(g_trailStyle==1)g_trailMs=g_classicDuration.load();
    g_hideWhenFast = Wh_GetIntSetting(L"hideWhenFast") != 0;
    g_hideSpeedCm = Clamp(Wh_GetIntSetting(L"hideSpeed"), 20, 300);
    g_pauseInFullscreen = Wh_GetIntSetting(L"pauseInFullscreen") != 0;
}

// A monitor's refresh rate.
static int RefreshRate(HMONITOR mon) {
    MONITORINFOEXW mi;
    mi.cbSize = sizeof(mi);
    DEVMODEW dm = {};
    dm.dmSize = sizeof(dm);
    if (GetMonitorInfoW(mon, &mi) && EnumDisplaySettingsW(mi.szDevice, ENUM_CURRENT_SETTINGS, &dm) &&
        dm.dmDisplayFrequency > 1)
        return dm.dmDisplayFrequency;
    return 60;
}

// A monitor's physical pixel density, from the size Windows reports for it (its DPI setting if that looks wrong).
static double PixelsPerCm(HMONITOR mon, UINT dpi) {
    double fallback = dpi / 2.54 < 20 ? 20 : dpi / 2.54;
    MONITORINFOEXW mi;
    mi.cbSize = sizeof(mi);
    if (!GetMonitorInfoW(mon, &mi)) return fallback;
    HDC dc = CreateDCW(L"DISPLAY", mi.szDevice, nullptr, nullptr);
    if (!dc) return fallback;
    int mm = GetDeviceCaps(dc, HORZSIZE), px = GetDeviceCaps(dc, HORZRES);
    DeleteDC(dc);
    return mm >= 150 && mm <= 2500 && px >= 320 ? px / (mm / 10.0) : fallback;
}

static bool FullscreenAppRunning() {
    QUERY_USER_NOTIFICATION_STATE s;
    return SHQueryUserNotificationState(&s) == S_OK &&
           (s == QUNS_BUSY || s == QUNS_RUNNING_D3D_FULL_SCREEN || s == QUNS_PRESENTATION_MODE);
}

// Width of the arrow as Windows loaded it: at the system DPI, and with the Accessibility pointer size applied.
static int ArrowSize() {
    int size = 0;
    ICONINFO ii;
    if (GetIconInfo(LoadCursorW(nullptr, IDC_ARROW), &ii)) {
        BITMAP bm;
        if (GetObjectW(ii.hbmColor ? ii.hbmColor : ii.hbmMask, sizeof(bm), &bm)) size = bm.bmWidth;
        if (ii.hbmColor) DeleteObject(ii.hbmColor);
        DeleteObject(ii.hbmMask);
    }
    return size > 0 ? size : GetSystemMetrics(SM_CXCURSOR);
}

using MagBool=BOOL(WINAPI*)();using MagVisible=BOOL(WINAPI*)(BOOL);
static HMODULE g_magModule=nullptr;static MagBool g_magInit=nullptr,g_magUninit=nullptr;static MagVisible g_magShow=nullptr;
static bool LoadMagnification(){
 if(g_magShow)return true;
 g_magModule=LoadLibraryW(L"Magnification.dll");if(!g_magModule)return false;
 g_magInit=reinterpret_cast<MagBool>(GetProcAddress(g_magModule,"MagInitialize"));
 g_magUninit=reinterpret_cast<MagBool>(GetProcAddress(g_magModule,"MagUninitialize"));
 g_magShow=reinterpret_cast<MagVisible>(GetProcAddress(g_magModule,"MagShowSystemCursor"));
 return g_magInit&&g_magUninit&&g_magShow;
}
static void ShowNativeCursor(){AcquireSRWLockExclusive(&g_nativeVisibilityLock);if(g_magShow)g_magShow(TRUE);ReleaseSRWLockExclusive(&g_nativeVisibilityLock);}
static void ReloadCursors() { SystemParametersInfoW(SPI_SETCURSORS, 0, nullptr, 0); }

// Whether our invisible cursor is in place, also kept in the mod's storage: if the process is ever killed while hiding,
// the next start knows to put the user's cursors back (and otherwise leaves the cursor scheme alone). That is a storage
// write per hide and per restore, only while hiding is switched on: a deliberate trade for being able to recover.
// The flag is per user, as the storage is shared by everyone signed in on the machine.
static WCHAR g_hiddenKey[256] = L"cursorsHidden";

static void SetBlank(bool on) {
    g_blankActive = on;
    if(g_moved)SetEvent(g_moved);
    Wh_SetIntValue(g_hiddenKey, on);
}

// ---- sampler ----

// The sampler owns the schedule; no input is synthesized and no busy wait is
// used while waiting for an idle deadline. A new random interval follows every
// physical mouse action and every completed animation.
struct IdleSchedule{
 unsigned random=1;LONGLONG next=-1,start=0;int kind=0,lastKind=0;bool active=false;
 unsigned Random(){random^=random<<13;random^=random>>17;random^=random<<5;return random;}
 LONGLONG Delay(int lo,int hi){return lo*1000+Random()%unsigned((hi-lo)*1000+1);}
 void Reset(LONGLONG now,int lo,int hi){active=false;kind=0;next=now+Delay(lo,hi);}
 void Step(LONGLONG now,bool activity,bool allowed,int mask,int lo,int hi,int duration){
  if(activity||!allowed||!mask){Reset(now,lo,hi);return;}
  if(next<0){Reset(now,lo,hi);return;}
  if(active){if(now-start>=duration){Reset(now,lo,hi);}return;}
  if(now<next)return;
  int choices[3],count=0;for(int i=1;i<=3;i++)if(mask&(1<<(i-1)))choices[count++]=i;
  if(count>1){int kept=0;for(int i=0;i<count;i++)if(choices[i]!=lastKind)choices[kept++]=choices[i];count=kept;}
  kind=choices[Random()%count];lastKind=kind;start=now;active=true;
 }
};
static bool IsBusyCursor(HCURSOR cursor){return cursor&&(cursor==LoadCursorW(nullptr,IDC_WAIT)||cursor==LoadCursorW(nullptr,IDC_APPSTARTING));}
static void UpdateIdle(LONGLONG now,bool moved,bool visible,HCURSOR cursor){
 static IdleSchedule schedule{unsigned(GetTickCount64()^GetCurrentProcessId())|1u};
 static unsigned seenInput=0;static HCURSOR previous=nullptr;
 unsigned generation=g_idleInputGeneration.load();
 bool held=(GetAsyncKeyState(VK_LBUTTON)&0x8000)||(GetAsyncKeyState(VK_RBUTTON)&0x8000)||(GetAsyncKeyState(VK_MBUTTON)&0x8000);
 bool allowed=g_idleEnabled&&visible&&!g_paused&&!held&&!IsBusyCursor(cursor);
 if(g_idleArrowOnly&&cursor!=LoadCursorW(nullptr,IDC_ARROW)&&cursor!=LoadCursorW(nullptr,IDC_HAND))allowed=false;
 bool activity=moved||generation!=seenInput||cursor!=previous||g_idleReset.exchange(false);
 seenInput=generation;previous=cursor;
 int mask=(g_idleSpin?1:0)|(g_idleFigure8?2:0)|(g_idleLook?4:0);
 bool was=g_idleActive.load();
 schedule.Step(now,activity,allowed,mask,g_idleMinSeconds,g_idleMaxSeconds,g_idleDuration);
 g_idleKind=schedule.kind;g_idleStart=schedule.start;g_idleActive=schedule.active;
 bool busy=g_busyHop&&visible&&!g_paused&&IsBusyCursor(cursor);
 bool wasBusy=g_busyActive.exchange(busy);
 if(schedule.active||was!=schedule.active||wasBusy!=busy)SetEvent(g_moved);
}
struct IdlePose{double angle=0,x=0,y=0,scale=1;};
static IdlePose IdlePoseAt(int kind,double t,double amount){
 t=std::clamp(t,0.0,1.0);constexpr double pi=3.141592653589793;
 double envelope=sin(pi*t);envelope*=envelope;IdlePose pose;
 if(kind==1){double ease=t*t*(3-2*t);pose.angle=360*ease;pose.scale=1+.025*envelope;}
 else if(kind==2){pose.x=amount*sin(2*pi*t)*envelope;pose.y=amount*.6*sin(4*pi*t)*envelope;pose.angle=8*sin(2*pi*t)*envelope;}
 else if(kind==3){pose.angle=22*sin(2*pi*t)*envelope;pose.x=amount*.45*sin(2*pi*t)*envelope;pose.y=-amount*.2*envelope;}
 return pose;
}
static double BusyHopAt(double t,double amount){
 t-=floor(t);double wave=sin(3.141592653589793*t);return -amount*wave*wave;
}
// Continuous scale and velocity at takeoff, landing and loop boundaries.
// Reciprocal horizontal scaling preserves area rather than inflating the cursor.
static double BusyHopScaleAt(double t,double amount){
 t-=floor(t);double wave=sin(3.141592653589793*t);wave*=wave;
 double landing=pow(cos(3.141592653589793*t),16);
 return 1+std::clamp(amount,0.0,.3)*(wave-landing);
}

static DWORD WINAPI SampleThread(void*) {
    HMONITOR lastMon = nullptr;
    int arrow = 32;
    double hidePxPerCm = 38;   // turns the hide speed (cm/s on the main monitor) into px/s on the monitor in use
    LONGLONG lastMove = 0, lastFast = 0, lastFullscreenCheck = -1000;
    bool paused = false, timerHigh = false;
    while (!g_stop) {
        if (g_recheck.exchange(false)) lastFullscreenCheck = -1000;
        LONGLONG checkedAt = Now();
        if (checkedAt - lastFullscreenCheck >= 500) {
            lastFullscreenCheck = checkedAt;
            bool was = paused;
            paused = g_pauseInFullscreen && FullscreenAppRunning();
            g_paused = paused;
            if (paused != was) {
                Wh_Log(L"Paused for a fullscreen app: %d", paused);
                // stop listening to the mouse during the game
                if (g_wnd) PostMessageW(g_wnd, WM_APP_PAUSED, paused, 0);
            }
        }
        if (paused||g_protectedDesktop) {
            UpdateIdle(Now(),true,false,nullptr);
            EnterCriticalSection(&g_gate);
            g_curVisible = false;
            g_histN = 0;
            if (g_hideWanted) { g_hideWanted = false; SetEvent(g_hideChanged); }
            LeaveCriticalSection(&g_gate);
            if (timerHigh) { timeEndPeriod(1); timerHigh = false; }
            // woken when the foreground window changes; the timeout catches an app leaving fullscreen in place
            WaitForSingleObject(g_mouseWake, 1000);
            continue;
        }

        CURSORINFO ci;
        ci.cbSize = sizeof(ci);
        bool gotCursor=GetCursorInfo(&ci);
        // Respect the application's visibility even when our copy was visible.
        bool showing=gotCursor&&(ApplicationShowsCursor(ci)||g_typingAttached||g_typingVisual);
        if(showing&&!ci.hCursor)ci.hCursor=LoadCursorW(nullptr,IDC_IBEAM);
        // Crossing to another monitor: measure it here, outside the lock, as some of these calls are slow.
        HMONITOR mon = showing ? MonitorFromPoint(ci.ptScreenPos, MONITOR_DEFAULTTONEAREST) : lastMon;
        bool stale = showing && g_monStale.exchange(false);
        bool newMon = showing && (mon != lastMon || stale);
        int monCursor = 32;
        if (newMon) {
            lastMon = mon;
            UINT dpi = 96, mainDpi = 96, dy;
            GetDpiForMonitor(mon, MDT_EFFECTIVE_DPI, &dpi, &dy);
            // While hiding, the arrow is our invisible one: keep a size only if it was measured with hiding off.
            bool blankBefore = g_blankActive;
            int measured = blankBefore ? 0 : ArrowSize();
            if (!blankBefore && !g_blankActive) arrow = measured;
            monCursor = MulDiv(arrow, dpi, GetDpiForSystem());
            int hz = RefreshRate(mon);
            // The hide speed is set in cm/s as measured on the main monitor. Windows moves the pointer further on a
            // monitor with more scaling (twice the pixels at 200%), so scale it by the DPI to hide at the same hand
            // speed on every monitor.
            POINT origin = {0, 0};
            HMONITOR mainMon = MonitorFromPoint(origin, MONITOR_DEFAULTTOPRIMARY);
            GetDpiForMonitor(mainMon, MDT_EFFECTIVE_DPI, &mainDpi, &dy);
            hidePxPerCm = PixelsPerCm(mainMon, mainDpi) * dpi / mainDpi;
            // Half a refresh: frames are not timed to the screen's refreshes, so a whole one would leave the blur
            // fresh on some refreshes and almost a refresh old on others, and it would jitter against the pointer.
            g_monGap = 500 / hz > 3 ? 500 / hz : 3;
        }

        EnterCriticalSection(&g_gate);
        if (!showing) {
            UpdateIdle(Now(),true,false,nullptr);
            g_curVisible = false;
            g_histN = 0;
            if (g_hideWanted) { g_hideWanted = false; SetEvent(g_hideChanged); }
        } else {
            g_curVisible = true;
            {
                if(g_curHandle&&g_curHandle!=ci.hCursor){g_shapePulse=Now();SetEvent(g_moved);}
                g_curHandle=ci.hCursor;
            }
            bool changed = g_histN == 0 || g_hist[g_histN - 1].x != ci.ptScreenPos.x ||
                           g_hist[g_histN - 1].y != ci.ptScreenPos.y;

            if (newMon) {   // drop the trail so it doesn't smear across the gap between monitors
                g_histN = 0;
                g_monCursor = monCursor;
            }

            LONGLONG now = Now();
            if (changed && g_histN && g_hist[g_histN - 1].t == now) {   // several reports within a ms: keep the newest
                g_hist[g_histN - 1].x = ci.ptScreenPos.x;
                g_hist[g_histN - 1].y = ci.ptScreenPos.y;
            } else if (changed || g_histN == 0 || now - g_hist[g_histN - 1].t >= 4) {
                if (g_histN == MAX_SAMPLES) {
                    memmove(g_hist, g_hist + 1, (MAX_SAMPLES - 1) * sizeof(Sample));
                    g_histN--;
                }
                g_hist[g_histN++] = {ci.ptScreenPos.x, ci.ptScreenPos.y, now};
            }
            UpdateIdle(now,changed,true,ci.hCursor);
            if(g_typingAttached||g_typingVisual||g_knightActive||g_knightVisual)g_idleActive=false;
            if (changed) { lastMove = now; SetEvent(g_moved); }   // new mouse position: draw now
            int drop = 0;
            while (g_histN - drop > 1 && now - g_hist[drop].t > g_trailMs) drop++;
            if (drop) {
                memmove(g_hist, g_hist + drop, (g_histN - drop) * sizeof(Sample));
                g_histN -= drop;
            }

            // speed over the trail window -> hide the real cursor when very fast
            LONGLONG dt = g_hist[g_histN - 1].t - g_hist[0].t;
            bool wasHidden = g_hideWanted;
            if (!g_hideWhenFast || g_drawFailed) {
                g_hideWanted = false;
            } else if (dt >= 8) {
                // Hiding looks at the whole trail window (steady). Bringing the cursor back looks at only the last few ms
                // (from sample j on), so it returns as soon as the mouse stops or slows, not a whole trail later.
                int j = g_histN - 1;
                while (j > 0 && now - g_hist[j - 1].t <= SHOW_WINDOW_MS) j--;
                double path = 0, recentPath = 0;
                for (int i = 1; i < g_histN; i++) {
                    double d = Dist(g_hist[i - 1], g_hist[i]);
                    path += d;
                    if (i > j) recentPath += d;
                }
                double speed = path * 1000.0 / dt;
                double hide = g_hideSpeedCm * hidePxPerCm;   // px/s on this monitor, the same hand speed everywhere
                LONGLONG recentDt = now - g_hist[j].t;
                double recent = recentDt >= 4 ? recentPath * 1000.0 / recentDt : speed;
                if (speed > hide) {
                    g_hideWanted = true;
                    lastFast = now;
                } else if (g_hideWanted && recent < hide * 0.5 && now - lastFast > HOLD_MS) {
                    g_hideWanted = false;
                }
            }
            if (g_hideWanted && now - lastMove > HOLD_MS + SHOW_WINDOW_MS) g_hideWanted = false;
            bool clickActive=(g_selectionEnabled||g_pressureEnabled)&&((GetAsyncKeyState(VK_LBUTTON)&0x8000)!=0);
            bool shapeActive=(g_shapeBounce&&now-g_shapePulse.load()<300)||(g_shapeTransition&&now-g_shapePulse.load()<g_transitionDuration+30);
            bool moveActive=(g_tiltEnabled||g_stretchEnabled||g_resizeEnabled||g_dragEnabled)&&now-lastMove<100;
            bool animationActive=g_idleActive.load()||g_busyActive.load()||clickActive||shapeActive||moveActive||g_animating.load()||g_transitionActive.load()||
                (g_magnetEnabled&&fabs(g_magnetAngle.load())>.02)||(g_wheelEnabled&&now-g_wheelTime.load()<350)||(g_clickNudge&&now-g_clickPulse.load()<300);
            if(!g_drawFailed&&g_pointerReady&&animationActive)g_hideWanted=true;
            if(g_alwaysVisible)g_hideWanted=g_pointerReady&&g_overlayReady&&!g_drawFailed;
            // Otherwise retain the optional trail's hide decision above. At rest
            // the native cursor returns and the animated replacement is removed.

            if (g_hideWanted != wasHidden) SetEvent(g_hideChanged);
        }
        bool visible = g_curVisible, hidden = g_hideWanted;
        LeaveCriticalSection(&g_gate);

        if (Now() - lastMove < g_trailMs + 40 || hidden) {   // never sleep while the real cursor is hidden
            if (!timerHigh) { timeBeginPeriod(1); timerHigh = true; }
            // look again on every mouse report; the timeout keeps the path ageing after a stop, and checks every 2 ms
            // while the real cursor is hidden so it comes back quickly
            WaitForSingleObject(g_mouseWake, hidden&&g_animating?2:10);
        } else {
            if (timerHigh) { timeEndPeriod(1); timerHigh = false; }
            // Sleep until the mouse moves; while an app hides the cursor, just look once per report. While a window with more
            // rights is in front no mouse reports arrive, so look every 10 ms instead, and every 100 ms once the mouse has
            // been still for a second.
            DWORD poll = (g_idleEnabled||g_busyHop)?100:INFINITE;
            if (g_adminFront) poll = Now() - lastMove < 1000 ? 10 : 100;
            if (WaitForSingleObject(g_mouseWake, poll) == WAIT_OBJECT_0 && visible) lastMove = Now();
        }
    }
    if (timerHigh) timeEndPeriod(1);
    return 0;
}


static bool CaretRectangle(IUIAutomationTextRange* range,POINT& point,bool rightEdge=false){
 SAFEARRAY* rectangles=nullptr;if(FAILED(range->GetBoundingRectangles(&rectangles))||!rectangles)return false;
 LONG lo=0,hi=-1;SafeArrayGetLBound(rectangles,1,&lo);SafeArrayGetUBound(rectangles,1,&hi);double* values=nullptr;bool ok=false;
 if(hi-lo+1>=4&&SUCCEEDED(SafeArrayAccessData(rectangles,(void**)&values))){
  LONG offset=rightEdge?(hi-lo+1)/4*4-4:0;double x=values[offset],y=values[offset+1],w=values[offset+2],h=values[offset+3];
  if(isfinite(x)&&isfinite(y)&&isfinite(w)&&isfinite(h)&&h>0){point={LONG(lround(x+(rightEdge?w:0))),LONG(lround(y+h*.5))};ok=true;}
  SafeArrayUnaccessData(rectangles);
 }
 SafeArrayDestroy(rectangles);return ok;
}
static bool DiscoverTypingCaret(IUIAutomation* uia,POINT& point){
 IUIAutomationElement* focused=nullptr;if(!uia||FAILED(uia->GetFocusedElement(&focused))||!focused)return false;
 BOOL password=TRUE;RECT bounds{};bool safe=SUCCEEDED(focused->get_CurrentIsPassword(&password))&&!password&&SUCCEEDED(focused->get_CurrentBoundingRectangle(&bounds));
 if(!safe){focused->Release();return false;}
 GUITHREADINFO gui{sizeof(gui)};bool found=false;
 if(GetGUIThreadInfo(0,&gui)&&gui.hwndCaret&&gui.rcCaret.bottom>gui.rcCaret.top){
  point={gui.rcCaret.left,(gui.rcCaret.top+gui.rcCaret.bottom)/2};found=ClientToScreen(gui.hwndCaret,&point)!=FALSE;
 }
 IUIAutomationTextRange* range=nullptr;
 if(!found){
  IUIAutomationTextPattern2* pattern=nullptr;
  if(SUCCEEDED(focused->GetCurrentPatternAs(UIA_TextPattern2Id,__uuidof(IUIAutomationTextPattern2),(void**)&pattern))&&pattern){BOOL active=FALSE;if(FAILED(pattern->GetCaretRange(&active,&range))||!active){if(range)range->Release();range=nullptr;}pattern->Release();}
  if(!range){IUIAutomationTextPattern* base=nullptr;if(SUCCEEDED(focused->GetCurrentPatternAs(UIA_TextPatternId,__uuidof(IUIAutomationTextPattern),(void**)&base))&&base){IUIAutomationTextRangeArray* selections=nullptr;if(SUCCEEDED(base->GetSelection(&selections))&&selections){int count=0;selections->get_Length(&count);if(count==1)selections->GetElement(0,&range);selections->Release();}base->Release();}}
  if(range){
   range->MoveEndpointByRange(TextPatternRangeEndpoint_Start,range,TextPatternRangeEndpoint_End);
   found=CaretRectangle(range,point);
   if(!found){IUIAutomationTextRange* next=nullptr;if(SUCCEEDED(range->Clone(&next))&&next){int moved=0;next->MoveEndpointByUnit(TextPatternRangeEndpoint_End,TextUnit_Character,1,&moved);if(moved)found=CaretRectangle(next,point);next->Release();}}
   if(!found){int moved=0;range->MoveEndpointByUnit(TextPatternRangeEndpoint_Start,TextUnit_Character,-1,&moved);if(moved)found=CaretRectangle(range,point,true);}
   range->Release();
  }
 }
 focused->Release();return found&&PtInRect(&bounds,point)&&MonitorFromPoint(point,MONITOR_DEFAULTTONULL);
}
static void UpdateTypingCaret(IUIAutomation* uia){
 if(!g_typingEnabled||g_paused||g_protectedDesktop){g_typingRequested=false;g_typingAttached=false;return;}
 if(!g_typingRequested&&!g_typingAttached)return;
 unsigned generation=g_typingGeneration;POINT point{};bool found=DiscoverTypingCaret(uia,point);
 if(generation!=g_typingGeneration)return;
 if(found){AcquireSRWLockExclusive(&g_typingLock);g_typingTarget=point;ReleaseSRWLockExclusive(&g_typingLock);g_typingAttached=true;g_idleActive=false;SetEvent(g_moved);SetEvent(g_mouseWake);}
 else if(Now()-g_typingKeyTime>700){g_typingRequested=false;g_typingAttached=false;SetEvent(g_moved);SetEvent(g_mouseWake);}
}
static double TypingEase(double t){t=std::clamp(t,0.0,1.0);return t*t*t*(t*(t*6-15)+10);}
static bool TypingPoints(Sample* points,int& count){
 static Sample history[MAX_SAMPLES];static int length=0;static bool following=false,returning=false;static double x=0,y=0,fromX=0,fromY=0;static LONGLONG returnStart=0,lastFrame=0,attachStart=0;
 LONGLONG now=Now();bool attached=g_typingAttached&&g_typingEnabled&&!g_paused&&!g_protectedDesktop;
 double dt=lastFrame?std::clamp(double(now-lastFrame),0.0,40.0):7.0;lastFrame=now;
 if(attached){POINT target{};AcquireSRWLockShared(&g_typingLock);target=g_typingTarget;ReleaseSRWLockShared(&g_typingLock);
  if(!following){if(!returning){x=points[count-1].x;y=points[count-1].y;}length=0;attachStart=now;}
  double smoothing=now-attachStart<350?110.0:55.0;
  double blend=1-exp(-dt/smoothing);
  x+=(target.x+3-x)*blend;y+=(target.y-y)*blend;
  following=true;returning=false;
 }
 else if(following){following=false;returning=true;returnStart=now;fromX=x;fromY=y;}
 if(g_protectedDesktop||g_paused){following=returning=false;length=0;g_typingVisual=false;return false;}
 if(returning){double t=double(now-returnStart)/g_typingReturnDuration.load();double eased=TypingEase(t);x=fromX+(points[count-1].x-fromX)*eased;y=fromY+(points[count-1].y-fromY)*eased;if(t>=1){returning=false;}}
 if(!attached&&!returning&&length==0){g_typingVisual=false;return false;}
 if(length==MAX_SAMPLES){memmove(history,history+1,sizeof(Sample)*(--length));}
 history[length++]={LONG(lround(x)),LONG(lround(y)),now};
 int drop=0;while(length-drop>1&&now-history[drop].t>g_trailMs)drop++;if(drop){memmove(history,history+drop,sizeof(Sample)*(length-drop));length-=drop;}
 memcpy(points,history,sizeof(Sample)*length);count=length;
 bool active=attached||returning;g_typingVisual=active;
 if(!active)length=0;
 return true;
}

static bool KnightPoints(Sample* points,int& count){
 static Sample history[MAX_SAMPLES];static int length=0;static double x=0,y=0;static LONGLONG last=0;static double returnVX=0,returnVY=0;
 LONGLONG now=Now();double dt=last?std::clamp(double(now-last),1.0,40.0):7;last=now;
 bool active=g_knightActive&&!g_typingVisual&&!g_typingAttached&&!g_paused&&!g_protectedDesktop;
 double targetX=0,targetY=0;
 if(active){double phase=double(now-g_knightStart)/1000;targetX=sin(phase*1.47*g_knightHorizontalSpeed/100.0)*g_knightHorizontal;targetY=sin(phase*2.35*g_knightVerticalSpeed/100.0)*g_knightVertical;}
 double mix=1-exp(-dt/(active?110.0:90.0));
 if(!active&&g_idleReturnElastic){for(int i=0;i<4;i++){double step=dt/4000;returnVX+=(-x*300-returnVX*22)*step;returnVY+=(-y*300-returnVY*22)*step;x+=returnVX*step;y+=returnVY*step;}}
 else{x+=(targetX-x)*mix;y+=(targetY-y)*mix;returnVX=returnVY=0;}
 if(!active&&hypot(x,y)<.03&&hypot(returnVX,returnVY)<.3){x=y=0;length=0;g_knightVisual=false;return false;}
 if(g_protectedDesktop||g_paused){x=y=0;length=0;g_knightVisual=false;return false;}
 if(length==MAX_SAMPLES){memmove(history,history+1,sizeof(Sample)*(--length));}
 Sample real=points[count-1];history[length++]={LONG(lround(real.x+(g_idleTrailPinned?0:x))),LONG(lround(real.y+(g_idleTrailPinned?0:y))),now};
 int drop=0;while(length-drop>1&&now-history[drop].t>g_trailMs)drop++;if(drop){memmove(history,history+drop,sizeof(Sample)*(length-drop));length-=drop;}
 memcpy(points,history,sizeof(Sample)*length);count=length;g_knightVisual=true;return true;
}
// Accessibility discovery runs separately from sampling/rendering. Only control
// type, enabled state and bounds are read; no names, values, or text are collected.
static DWORD WINAPI ContextThread(void*){
 HRESULT com=CoInitializeEx(nullptr,COINIT_MULTITHREADED);IUIAutomation* uia=nullptr;IUIAutomationTreeWalker* walker=nullptr;
 if(SUCCEEDED(com)){
  CoCreateInstance(__uuidof(CUIAutomation8),nullptr,CLSCTX_INPROC_SERVER,__uuidof(IUIAutomation),reinterpret_cast<void**>(&uia));
  if(uia){IUIAutomation2* uia2=nullptr;if(SUCCEEDED(uia->QueryInterface(__uuidof(IUIAutomation2),reinterpret_cast<void**>(&uia2)))){uia2->put_ConnectionTimeout(100);uia2->put_TransactionTimeout(100);uia2->Release();}uia->get_ControlViewWalker(&walker);}
 }
 while(!g_stop){
  POINT p{};GetCursorPos(&p);double magnetic=0,magnetX=0,magnetY=0;CURSORINFO ci{sizeof(ci)};GetCursorInfo(&ci);
  int axis=0;
  if(ci.hCursor==LoadCursorW(nullptr,IDC_SIZEWE))axis=1;
  else if(ci.hCursor==LoadCursorW(nullptr,IDC_SIZENS))axis=2;
  else if(ci.hCursor==LoadCursorW(nullptr,IDC_SIZENWSE))axis=3;
  else if(ci.hCursor==LoadCursorW(nullptr,IDC_SIZENESW))axis=4;
  g_resizeAxis=axis;
  if(!g_paused&&(g_magnetEnabled||g_clickNudge)&&uia&&!(GetAsyncKeyState(VK_LBUTTON)&0x8000)){
   IUIAutomationElement* e=nullptr;
   if(SUCCEEDED(uia->ElementFromPoint(p,&e))&&e){
    for(int depth=0;e&&depth<3;depth++){
     CONTROLTYPEID type=0;BOOL enabled=FALSE;RECT r{};
     e->get_CurrentControlType(&type);e->get_CurrentIsEnabled(&enabled);
     bool clickable=type==UIA_ButtonControlTypeId||type==UIA_HyperlinkControlTypeId||type==UIA_CheckBoxControlTypeId||type==UIA_RadioButtonControlTypeId||type==UIA_MenuItemControlTypeId||type==UIA_TabItemControlTypeId;
     if(clickable&&enabled&&SUCCEEDED(e->get_CurrentBoundingRectangle(&r))&&PtInRect(&r,p)&&r.right-r.left>0&&r.bottom-r.top>0&&r.right-r.left<800&&r.bottom-r.top<400){
      double x=((r.left+r.right)*.5-p.x)/(r.right-r.left),y=((r.top+r.bottom)*.5-p.y)/(r.bottom-r.top);
      magnetic=std::clamp(x+y*.5,-.75,.75)*g_magnetAmount;
      magnetX=std::clamp(x*2,-1.0,1.0);magnetY=std::clamp(y*2,-1.0,1.0);
      AcquireSRWLockExclusive(&g_buttonLock);g_buttonBounds=r;g_buttonBoundsTime=Now();ReleaseSRWLockExclusive(&g_buttonLock);break;
     }
     IUIAutomationElement* parent=nullptr;if(walker)walker->GetParentElement(e,&parent);e->Release();e=parent;
    }
    if(e)e->Release();
   }
  }
  RECT closeRect{};bool closeFound=false;int captionType=1;HWND effectWindow=GetAncestor(WindowFromPoint(p),GA_ROOT);if(effectWindow==g_wnd)effectWindow=GetForegroundWindow();
  if(g_closeEnabled&&!g_paused&&!g_protectedDesktop&&!g_typingAttached){
   HWND root=effectWindow;
   DWORD_PTR hit=0;
   if(root&&SendMessageTimeoutW(root,WM_NCHITTEST,0,MAKELPARAM(p.x,p.y),SMTO_ABORTIFHUNG|SMTO_BLOCK,20,&hit)&&(hit==HTCLOSE||hit==HTMINBUTTON||hit==HTMAXBUTTON)){captionType=hit==HTMINBUTTON?2:hit==HTMAXBUTTON?3:1;
    struct TitleInfo {DWORD size;RECT title;DWORD state[6];RECT rect[6];} info{};info.size=sizeof(info);DWORD_PTR ignored=0;
    if(SendMessageTimeoutW(root,0x033F,0,(LPARAM)&info,SMTO_ABORTIFHUNG|SMTO_BLOCK,20,&ignored)){closeRect=info.rect[captionType==2?2:captionType==3?3:5];closeFound=PtInRect(&closeRect,p);}
   }
   if(!closeFound&&uia){IUIAutomationElement* button=nullptr;if(SUCCEEDED(uia->ElementFromPoint(p,&button))&&button){
    for(int depth=0;button&&depth<3;++depth){CONTROLTYPEID type=0;BOOL enabled=FALSE;button->get_CurrentControlType(&type);button->get_CurrentIsEnabled(&enabled);BSTR id=nullptr;button->get_CurrentAutomationId(&id);
     bool match=false;if(id){if(_wcsicmp(id,L"Close")==0||_wcsicmp(id,L"CloseButton")==0||_wcsicmp(id,L"WindowCloseButton")==0){match=true;captionType=1;}else if(_wcsicmp(id,L"Minimize")==0||_wcsicmp(id,L"MinimizeButton")==0){match=true;captionType=2;}else if(_wcsicmp(id,L"Maximize")==0||_wcsicmp(id,L"MaximizeButton")==0||_wcsicmp(id,L"Restore")==0||_wcsicmp(id,L"RestoreButton")==0){match=true;captionType=3;}SysFreeString(id);}
     if(type==UIA_ButtonControlTypeId&&enabled&&match&&SUCCEEDED(button->get_CurrentBoundingRectangle(&closeRect))&&PtInRect(&closeRect,p)){closeFound=true;break;}
     IUIAutomationElement* parent=nullptr;if(walker)walker->GetParentElement(button,&parent);button->Release();button=parent;
    }if(button)button->Release();}}
  }
  closeFound=closeFound&&closeRect.right>closeRect.left&&closeRect.bottom>closeRect.top&&closeRect.right-closeRect.left<=200&&closeRect.bottom-closeRect.top<=120;
  AcquireSRWLockExclusive(&g_closeLock);g_closeTarget=closeFound?closeRect:RECT{};g_closeSource=closeFound?effectWindow:nullptr;g_captionTargetType=captionType;g_closeTime=Now();ReleaseSRWLockExclusive(&g_closeLock);
  if(closeFound||g_closeVisual){SetEvent(g_moved);SetEvent(g_mouseWake);}
  UpdateTypingCaret(uia);
  LASTINPUTINFO input{sizeof(input)};
  bool floating=g_knightEnabled&&!g_paused&&!g_protectedDesktop&&!g_typingRequested&&!g_typingAttached&&!g_typingVisual&&GetLastInputInfo(&input)&&GetTickCount()-input.dwTime>=DWORD(g_knightDelay.load())*1000&&ApplicationShowsCursor(ci);
  static DWORD knightLastInput=0;if(GetLastInputInfo(&input)&&input.dwTime!=knightLastInput){knightLastInput=input.dwTime;g_knightStart=0;}
  if(floating&&g_knightDuration>0&&g_knightStart>0&&Now()-g_knightStart>=g_knightDuration.load()*1000LL)floating=false;
  if(GetLastInputInfo(&input)&&GetTickCount()-input.dwTime<DWORD(g_knightDelay.load())*1000)g_knightStart=0;
  if(g_knightActive.exchange(floating)!=floating){if(floating&&g_knightStart==0)g_knightStart=Now();SetEvent(g_moved);SetEvent(g_mouseWake);}
  if(floating)g_idleActive=false;
  if(!g_magnetEnabled||g_typingAttached||g_typingVisual||g_knightActive||g_protectedDesktop){magnetic=0;magnetX=magnetY=0;}
  if(fabs(magnetX-g_magnetX.load())>.005||fabs(magnetY-g_magnetY.load())>.005){g_magnetX=magnetX;g_magnetY=magnetY;SetEvent(g_moved);SetEvent(g_mouseWake);}
  if(fabs(magnetic-g_magnetAngle.load())>.02){g_magnetAngle=magnetic;SetEvent(g_moved);SetEvent(g_mouseWake);}
  Sleep(g_paused?150:60);
 }
 if(walker)walker->Release();if(uia)uia->Release();if(SUCCEEDED(com))CoUninitialize();return 0;
}

// ---- hider ----

struct NativeCaretSignal { DWORD tick,pid,active; };
static HANDLE g_caretSignalMapping=nullptr;
static NativeCaretSignal* g_caretSignal=nullptr;
static void PublishNativeCaret(bool active){
 if(!g_caretSignal){g_caretSignalMapping=CreateFileMappingW(INVALID_HANDLE_VALUE,nullptr,PAGE_READWRITE,0,sizeof(NativeCaretSignal),L"Local\\FluentPointerNativeCaret");if(g_caretSignalMapping)g_caretSignal=(NativeCaretSignal*)MapViewOfFile(g_caretSignalMapping,FILE_MAP_WRITE,0,0,sizeof(NativeCaretSignal));}
 if(g_caretSignal){DWORD pid=0;GetWindowThreadProcessId(GetForegroundWindow(),&pid);g_caretSignal->pid=pid;g_caretSignal->active=active;MemoryBarrier();g_caretSignal->tick=GetTickCount();}
}
static DWORD WINAPI HideThread(void*) {
 if(!LoadMagnification()||!g_magInit()){
  Wh_SetStringValue(L"pointerStatus",L"Magnification cursor visibility API unavailable; native cursor retained");
  g_drawFailed=true;return 0;
 }
 while(!g_stop){
  bool protectedDesktop=!PointerDesktopIsActive();
  if(g_protectedDesktop.exchange(protectedDesktop)!=protectedDesktop){
   Wh_SetStringValue(L"pointerDesktopStatus",protectedDesktop?L"Other/protected desktop: native cursor restored":L"Normal desktop: pointer effects resumed");
   SetEvent(g_mouseWake);SetEvent(g_moved);
  }
  AcquireSRWLockExclusive(&g_nativeVisibilityLock);
  CURSORINFO current{sizeof(current)};
  bool replacementReady=g_overlayReady&&!g_drawFailed&&g_wnd&&IsWindowVisible(g_wnd)&&GetCursorInfo(&current)&&(ApplicationShowsCursor(current)||g_typingAttached||g_typingVisual);
  PublishNativeCaret(g_typingAttached&&g_typingHideNative&&replacementReady&&!protectedDesktop&&!g_paused);
  bool typingReplacement=g_typingAttached||g_typingVisual;
  bool forced=(g_alwaysHideCursor.load()||typingReplacement||g_closeVisual)&&!protectedDesktop&&replacementReady;
  EnterCriticalSection(&g_gate);bool want=!protectedDesktop&&replacementReady&&(forced||g_hideWanted);LeaveCriticalSection(&g_gate);
  if(!replacementReady&&g_blankActive)SetEvent(g_moved);
  // Reassert forced suppression periodically: another component can restore
  // native visibility without changing our bookkeeping flag. No cursor handles
  // or system cursor images are replaced, so ANI frames remain available.
  if(forced||protectedDesktop||want!=g_blankActive.load()){
   if(g_magShow(!want)){
    bool changed=want!=g_blankActive.load();SetBlank(want);
    if(changed)Wh_SetStringValue(L"pointerStatus",forced?L"Always hide cursor: native suppression forced":want?L"Native cursor suppressed only while a visible replacement is ready":L"Native cursor restored");
   }
   else {g_drawFailed=true;g_magShow(TRUE);SetBlank(false);Wh_SetStringValue(L"pointerStatus",L"Native hide failed; native cursor retained");}
  }
  ReleaseSRWLockExclusive(&g_nativeVisibilityLock);
  WaitForSingleObject(g_hideChanged,(g_typingAttached||g_typingVisual)?16:100);
 }
 PublishNativeCaret(false);if(g_caretSignal){UnmapViewOfFile(g_caretSignal);g_caretSignal=nullptr;}if(g_caretSignalMapping){CloseHandle(g_caretSignalMapping);g_caretSignalMapping=nullptr;}
 ShowNativeCursor();SetBlank(false);g_magUninit();return 0;
}

// ---- cursor pictures (render thread only) ----

static struct { HCURSOR h; Sprite* sp; } g_sprites[MAX_SPRITES];
static int g_nSprites, g_nextId;
// Cursors an app made invisible on purpose: no blur for them. Checked again after a while, because another tool that
// hides the cursor the way this mod does (swapping the system cursors) would otherwise switch the blur off for good.
static struct { HCURSOR h; LONGLONG t; } g_empties[MAX_SPRITES];
static int g_nEmpties;
static Sprite* g_lastSprite;             // the last real picture, drawn while the real cursor is hidden

// Takes the cursor's picture by drawing it on black and on white: what shows through gives the opacity.
static Sprite* Grab(HCURSOR h, bool* empty) {
    *empty = false;
    ICONINFO ii;
    if (!GetIconInfo(h, &ii)) return nullptr;
    BITMAP bm = {};
    GetObjectW(ii.hbmColor ? ii.hbmColor : ii.hbmMask, sizeof(bm), &bm);
    // a monochrome mask holds AND and XOR halves
    int w = bm.bmWidth, ht = ii.hbmColor ? bm.bmHeight : bm.bmHeight / 2;
    if (ii.hbmColor) DeleteObject(ii.hbmColor);
    if (ii.hbmMask) DeleteObject(ii.hbmMask);
    if (w <= 0 || ht <= 0 || w > 256 || ht > 256) return nullptr;

    int n = w * ht;
    BITMAPINFO bi = {};
    bi.bmiHeader = {sizeof(BITMAPINFOHEADER), w, -ht, 1, 32, BI_RGB, 0, 0, 0, 0, 0};
    void* bits;
    HDC dc = CreateCompatibleDC(nullptr);
    HBITMAP dib = dc ? CreateDIBSection(dc, &bi, DIB_RGB_COLORS, &bits, nullptr, 0) : nullptr;
    Sprite* sp = dib ? (Sprite*)malloc(sizeof(Sprite) + n * 4) : nullptr;
    if (sp) {
        HGDIOBJ old = SelectObject(dc, dib);
        UINT32 *px = (UINT32*)(sp + 1), *onWhite = (UINT32*)bits;
        memset(bits, 0, n * 4);
        DrawIconEx(dc, 0, 0, h, w, ht, 0, nullptr, DI_NORMAL);
        GdiFlush();
        memcpy(px, bits, n * 4);   // on black
        memset(bits, 0xFF, n * 4);
        DrawIconEx(dc, 0, 0, h, w, ht, 0, nullptr, DI_NORMAL);
        GdiFlush();
        bool any = false;
        for (int i = 0; i < n; i++) {
            UINT32 b = px[i];
            // how much of the background shows through
            int a = 255 - ((int)(onWhite[i] >> 8 & 255) - (int)(b >> 8 & 255));
            a = Clamp(a, 0, 255);
            // on black = premultiplied colour
            UINT32 r = b >> 16 & 255, g = b >> 8 & 255, bl = b & 255, ua = (UINT32)a;
            px[i] = ua << 24 | (r < ua ? r : ua) << 16 | (g < ua ? g : ua) << 8 | (bl < ua ? bl : ua);
            any |= a != 0;
        }
        SelectObject(dc, old);
        *sp = {w, ht, (int)ii.xHotspot, (int)ii.yHotspot, ++g_nextId, px};
        if (!any) { free(sp); sp = nullptr; *empty = true; }
    }
    if (dib) DeleteObject(dib);
    if (dc) DeleteDC(dc);
    return sp;
}

static Sprite* g_originShadowSprite=nullptr;
static void FreeSprites() {
    free(g_originShadowSprite);g_originShadowSprite=nullptr;
    for (int i = 0; i < g_nSprites; i++) free(g_sprites[i].sp);
    g_nSprites = g_nEmpties = 0;
    g_lastSprite = nullptr;
}

static bool IsEmpty(HCURSOR h) {
    for (int i = 0; i < g_nEmpties; i++)
        if (g_empties[i].h == h) {
            if (Now() - g_empties[i].t < 2000) return true;
            g_empties[i] = g_empties[--g_nEmpties];   // look at it again
            return false;
        }
    return false;
}

static Sprite* GetSprite(HCURSOR h) {
    for (int i = 0; i < g_nSprites; i++)
        if (g_sprites[i].h == h) return g_sprites[i].sp;
    if (IsEmpty(h)) return nullptr;
    bool empty=false;
    Sprite* sp = Grab(h, &empty);
    // Magnification hides visibility only, so the current handle remains drawable.
    if (sp) {
        if (g_nSprites == MAX_SPRITES) FreeSprites();
        g_sprites[g_nSprites++] = {h, sp};
    }
    // Empty while our own blanking was off the whole time: the app itself shows an invisible cursor.
    if (empty) {
        if (g_nEmpties == MAX_SPRITES) g_nEmpties = 0;
        g_empties[g_nEmpties++] = {h, Now()};
    }
    return sp;
}

// GetCursorInfo keeps the ANI handle, not the currently displayed frame.
// The old cache always drew step zero. Read the frame sequence and its native
// per-step rate (jiffies = 1/60 s); keep cursor identity separate from its picture.
using CursorFrameInfo=HCURSOR(WINAPI*)(HCURSOR,DWORD,DWORD,DWORD*,DWORD*);
static HCURSOR AnimatedCursorFrame(HCURSOR cursor,bool& animated){
 static auto info=reinterpret_cast<CursorFrameInfo>(GetProcAddress(GetModuleHandleW(L"user32.dll"),"GetCursorFrameInfo"));
 static HCURSOR source=nullptr,frame=nullptr;
 static DWORD step=0,count=1,rate=0;
 static double next=0;
 animated=false;if(!info||!cursor)return cursor;
 double now=double(Now());
 if(cursor!=source){
  source=cursor;step=0;count=1;rate=0;
  frame=info(cursor,0,0,&rate,&count);
  next=now+std::max(1UL,rate)*1000.0/60;
 }
 if(!frame||count<2||count>4096)return cursor;
 animated=true;
 for(DWORD advanced=0;now>=next&&advanced<count;advanced++){
  step=(step+1)%count;DWORD newCount=count;
  HCURSOR updated=info(cursor,0,step,&rate,&newCount);
  if(!updated||newCount!=count){source=nullptr;animated=false;return cursor;}
  frame=updated;next+=std::max(1UL,rate)*1000.0/60;
 }
 if(now>=next)next=now+std::max(1UL,rate)*1000.0/60;
 return frame;
}

// ---- drawing (render thread only) ----


static HDC g_memDc;
static HBITMAP g_dib;
static HGDIOBJ g_dibOld;
// the canvas: premultiplied BGRA, top-down, g_canvasW wide; frames use its top-left corner
static UINT32* g_bits;
static int g_canvasW, g_canvasH;
static UINT32* g_pic;          // the cursor picture at the drawn size
static int* g_span;            // per row of g_pic: first and one-past-last visible pixel
static int g_picId, g_picW, g_picH;
static bool g_shown;
static bool g_staticFrame=false; // render thread only: retain the frame without redrawing

static void ReleaseCanvas() {
    if (g_dib) {
        SelectObject(g_memDc, g_dibOld);
        DeleteObject(g_dib);
        g_dib = nullptr;
    }
    g_canvasW = g_canvasH = 0;
}

static void EnsureCanvas(int w, int h) {   // grows only, in 128 px steps
    if (g_dib && w <= g_canvasW && h <= g_canvasH) return;
    int nw = (w + 127) / 128 * 128, nh = (h + 127) / 128 * 128;
    if (nw < g_canvasW) nw = g_canvasW;
    if (nh < g_canvasH) nh = g_canvasH;
    ReleaseCanvas();
    BITMAPINFO bi = {};
    bi.bmiHeader = {sizeof(BITMAPINFOHEADER), nw, -nh, 1, 32, BI_RGB, 0, 0, 0, 0, 0};
    g_dib = CreateDIBSection(g_memDc, &bi, DIB_RGB_COLORS, (void**)&g_bits, nullptr, 0);
    if (!g_dib) return;
    g_dibOld = SelectObject(g_memDc, g_dib);
    g_canvasW = nw;
    g_canvasH = nh;
}

// One pixel of the cursor picture resized to w x h, on premultiplied colours: the average of the pixels it covers when
// shrinking, bilinear when growing.
static UINT32 Pixel(const Sprite* sp, int x, int y, int w, int h) {
    float acc[4] = {}, n = 0;
    if (sp->w > w) {
        int x0 = x * sp->w / w, x1 = (x + 1) * sp->w / w, y0 = y * sp->h / h, y1 = (y + 1) * sp->h / h;
        if (x1 <= x0) x1 = x0 + 1;
        if (y1 <= y0) y1 = y0 + 1;
        for (int yy = y0; yy < y1; yy++)
            for (int xx = x0; xx < x1; xx++, n++)
                for (int s = 0; s < 4; s++) acc[s] += sp->px[yy * sp->w + xx] >> (8 * s) & 255;
    } else {
        float fx = (x + 0.5f) * sp->w / w - 0.5f, fy = (y + 0.5f) * sp->h / h - 0.5f;
        if (fx < 0) fx = 0;
        if (fy < 0) fy = 0;
        int x0 = (int)fx, y0 = (int)fy, x1 = x0 + 1 < sp->w ? x0 + 1 : x0, y1 = y0 + 1 < sp->h ? y0 + 1 : y0;
        float ax = fx - x0, ay = fy - y0, wt[4] = {(1 - ax) * (1 - ay), ax * (1 - ay), (1 - ax) * ay, ax * ay};
        UINT32 p[4] = {sp->px[y0 * sp->w + x0], sp->px[y0 * sp->w + x1],
                       sp->px[y1 * sp->w + x0], sp->px[y1 * sp->w + x1]};
        for (int i = 0; i < 4; i++)
            for (int s = 0; s < 4; s++) acc[s] += (p[i] >> (8 * s) & 255) * wt[i];
        n = 1;
    }
    UINT32 out = 0;
    for (int s = 0; s < 4; s++) out |= (UINT32)(acc[s] / n + 0.5f) << (8 * s);
    return out;
}

// The cursor picture at the drawn size, made again only when the cursor or the size changes.
static bool Resize(const Sprite* sp, int w, int h) {
    if (g_pic && g_picId == sp->id && g_picW == w && g_picH == h) return true;
    free(g_pic);
    g_pic = (UINT32*)malloc(w * h * 4 + h * 2 * sizeof(int));
    if (!g_pic) return false;
    g_span = (int*)(g_pic + w * h);
    g_picId = sp->id;
    g_picW = w;
    g_picH = h;
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
            g_pic[y * w + x] = w == sp->w && h == sp->h ? sp->px[y * w + x] : Pixel(sp, x, y, w, h);
    // most of a cursor is transparent: blending only touches the visible part of each row
    for (int y = 0; y < h; y++) {
        int a = 0, b = w;
        while (a < w && !g_pic[y * w + a]) a++;
        while (b > a && !g_pic[y * w + b - 1]) b--;
        g_span[2 * y] = a;
        g_span[2 * y + 1] = b;
    }
    return true;
}

// Draws the picture at (ox, oy) at opacity a (1-255): premultiplied "source over", two channels per multiplication.
static UINT32 RainbowPixel(UINT32 pixel,double hue){
 double h=(hue-floor(hue))*6;int sector=int(h);double t=h-sector;double r=0,g=0,b=0;
 switch(sector){case 0:r=1;g=t;break;case 1:r=1-t;g=1;break;case 2:g=1;b=t;break;case 3:g=1-t;b=1;break;case 4:r=t;b=1;break;default:r=1;b=1-t;}
 UINT32 light=std::max({pixel&255,(pixel>>8)&255,(pixel>>16)&255});
 return (pixel&0xFF000000)|(UINT32(lround(light*r))<<16)|(UINT32(lround(light*g))<<8)|UINT32(lround(light*b));
}
static void Blend(int a, int ox, int oy,double hue=-1,bool custom=false,double white=0,bool dim=false,double scale=1) {
    UINT32 k = (UINT32)a + 1;   // 2-256, so full opacity is exact
    for (int y = 0; y < g_picH; y++) {
        UINT32* row = g_bits + (oy + y) * g_canvasW + ox;
        const UINT32* src = g_pic + y * g_picW;
        for (int x = scale<.999?0:g_span[2 * y]; x < (scale<.999?g_picW:g_span[2 * y + 1]); x++) {
            int sourceX=x,sourceY=y;
            if(scale<.999){sourceX=int(floor((x-g_picW*.5)/scale+g_picW*.5));sourceY=int(floor((y-g_picH*.5)/scale+g_picH*.5));if(sourceX<0||sourceY<0||sourceX>=g_picW||sourceY>=g_picH)continue;}
            UINT32 s = scale<.999?g_pic[sourceY*g_picW+sourceX]:src[x];
            if (!s) continue;
            if(g_cursorColorMode&&(g_cursorColorFull||g_drawBusyCursor)){
                UINT32 r=(s>>16)&255,g=(s>>8)&255,b=s&255;
                bool wheel=b>r+12&&g>r+6;
                if(g_cursorColorFull||wheel){UINT32 luminance=std::max({r,g,b}),color=g_cursorColor.load();s=(s&0xFF000000)|((luminance*((color>>16)&255)/255)<<16)|((luminance*((color>>8)&255)/255)<<8)|(luminance*(color&255)/255);}
            }
            if(white>0){UINT32 alpha=s>>24;UINT32 red=(s>>16)&255,green=(s>>8)&255,blue=s&255;s=(alpha<<24)|(UINT32(lround(red+(alpha-red)*white))<<16)|(UINT32(lround(green+(alpha-green)*white))<<8)|UINT32(lround(blue+(alpha-blue)*white));}
            else if(hue>=0)s=RainbowPixel(s,hue);
            else if(custom){UINT32 light=std::max({s&255,(s>>8)&255,(s>>16)&255}),color=g_trailCustomColor.load();s=(s&0xFF000000)|((light*((color>>16)&255)/255)<<16)|((light*((color>>8)&255)/255)<<8)|(light*(color&255)/255);}
            if(dim){s=(s&0xFF000000)|((((s>>16)&255)*78/100)<<16)|((((s>>8)&255)*78/100)<<8)|((s&255)*78/100);}
            s = ((s & 0xFF00FF) * k >> 8 & 0xFF00FF) | ((s >> 8 & 0xFF00FF) * k & 0xFF00FF00);
            UINT32 inv = 256 - (s >> 24), d = row[x];
            row[x] = s + (((d & 0xFF00FF) * inv >> 8 & 0xFF00FF) | ((d >> 8 & 0xFF00FF) * inv & 0xFF00FF00));
        }
    }
}

static void KeepOverlayOnTop(){
 if(!g_wnd||!IsWindowVisible(g_wnd))return;
 bool raise=!(GetWindowLongPtrW(g_wnd,GWL_EXSTYLE)&WS_EX_TOPMOST);
 // TOPMOST is a group, not a permanent first position. Another topmost
 // window can be inserted above us without clearing our extended style.
 for(HWND above=GetWindow(g_wnd,GW_HWNDPREV);above&&!raise;above=GetWindow(above,GW_HWNDPREV)){
  if(IsWindowVisible(above)&&(GetWindowLongPtrW(above,GWL_EXSTYLE)&WS_EX_TOPMOST))raise=true;
 }
 if(raise)SetWindowPos(g_wnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOOWNERZORDER);
}
static bool Draw(const Sprite* sp, int sw, int sh, const Copy* c, int n, int x, int y, int w, int h, const Sprite* shadow=nullptr, int shadowX=0, int shadowY=0, int shadowSize=0, double shadowOpacity=0) {
    static bool cachedWhite=false;static int cachedW=0,cachedH=0,cachedMouseX=0,cachedMouseY=0,cachedOpacity=0;
    bool whiteOnly=!g_knightCloseDraw&&(!g_softRipples||g_rippleAge>=3000)&&g_closeMix==1&&g_closeRadiusPhase==1&&g_rayPulse==0&&n>0;for(int i=0;i<n&&whiteOnly;++i)whiteOnly=c[i].a==0;
    int relativeX=g_closeMouse.x-x,relativeY=g_closeMouse.y-y;
    if(whiteOnly&&cachedWhite&&IsWindowVisible(g_wnd)&&cachedW==w&&cachedH==h&&cachedOpacity==g_closeOpacity&&abs(relativeX-cachedMouseX)<=2&&abs(relativeY-cachedMouseY)<=2){
        SetWindowPos(g_wnd,HWND_TOPMOST,x,y,0,0,SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOOWNERZORDER);return true;
    }
    cachedWhite=false;
    if (!Resize(sp, sw, sh)) return false;
    EnsureCanvas(w, h);
    if (!g_dib) return false;
    for (int r = 0; r < h; r++) memset(g_bits + r * g_canvasW, 0, w * 4);
    if(shadow&&shadowOpacity>0){
        int gh=int(lround(double(shadow->h)*shadowSize/shadow->w));UINT32 k=UINT32(lround(shadowOpacity*255))+1;
        for(int sy=0;sy<gh;sy++)for(int sx=0;sx<shadowSize;sx++){
            int dx=shadowX+sx,dy=shadowY+sy;if(dx<0||dy<0||dx>=w||dy>=h)continue;
            UINT32 src=Pixel(shadow,sx,sy,shadowSize,gh);if(!src)continue;
            src=((src&0xFF00FF)*k>>8&0xFF00FF)|((src>>8&0xFF00FF)*k&0xFF00FF00);
            UINT32& dest=g_bits[dy*g_canvasW+dx];UINT32 inv=256-(src>>24);
            dest=src+(((dest&0xFF00FF)*inv>>8&0xFF00FF)|((dest>>8&0xFF00FF)*inv&0xFF00FF00));
        }
    }
    for (int i = 0; i < n; i++) {
        int a = (int)(c[i].a * 255 + 0.5f);
        if (a > 0 && c[i].x >= 0 && c[i].y >= 0 && c[i].x + sw <= w && c[i].y + sh <= h) Blend(a, c[i].x, c[i].y,g_trailColorMode==1&&i<n-1?double(Now())/1800*(g_trailColorSpeed/100.0)+double(i)/std::max(1,n-1):-1,g_trailColorMode==2&&i<n-1,i==n-1?g_hoverWhiteMix:0,g_knightVisual&&i<n-1&&g_trailColorMode!=1,g_trailShrink&&i<n-1?1-g_trailShrinkAmount/100.0*(1-double(i+1)/n):1);
    }
    bool knightFalling=g_knightCloseDraw&&Now()-g_knightCloseStart>=KnightFallStart()&&!g_knightSnapshot.empty();
    if(knightFalling){
      RECT r=g_knightSnapshotRect;int width=r.right-r.left,height=r.bottom-r.top;double t=std::clamp((Now()-g_knightCloseStart-KnightFallStart())/double(KnightFallDuration()),0.0,1.0);
      double cosine[352],sine[352],shiftX[352],shiftY[352];for(int piece=0;piece<352;++piece){double direction=(piece&1)?1:-1;double angle=(direction*t*(.18+(piece%7)*.09)+sin(t*5)*.035)*g_knightRotation/100.0;cosine[piece]=cos(angle);sine[piece]=sin(angle);shiftX[piece]=direction*(t*12+t*t*(14+(piece%5)*12));shiftY[piece]=t*t*(std::max(double(height),double(g_knightFloor-r.top))+hypot(width,height)+64)*1.25*(0.82+(piece%7)*.032);}
      for(int sy=0;sy<height;++sy)for(int sx=0;sx<width;++sx){size_t index=size_t(sy)*width+sx;int piece=g_knightPieces[index];double lx=sx-g_pieceCenterX[piece],ly=sy-g_pieceCenterY[piece];int dx=int(lround(r.left+g_pieceCenterX[piece]+lx*cosine[piece]-ly*sine[piece]+shiftX[piece]))-x,dy=int(lround(r.top+g_pieceCenterY[piece]+lx*sine[piece]+ly*cosine[piece]+shiftY[piece]))-y;for(int oy=0;oy<2;++oy)for(int ox=0;ox<2;++ox)if(dx+ox>=0&&dx+ox<w&&dy+oy>=0&&dy+oy<h)g_bits[(dy+oy)*g_canvasW+dx+ox]=g_knightSnapshot[index];}

    }
    if(g_closeMix>.001&&!knightFalling&&!g_knightCloseDraw){
        RECT r=g_closeDrawRect;double rw=r.right-r.left,rh=r.bottom-r.top,rounding=std::min(9.0,rh*.24);
        double cx=std::clamp(double(g_closeMouse.x),double(r.left),double(r.right)),cy=std::clamp(double(g_closeMouse.y),double(r.top),double(r.bottom));
        double knightCloseAge=g_knightCloseDraw?double(Now()-g_knightCloseStart):0;
        double radius=8+hypot(rw,rh)*(g_knightCloseDraw?1:g_closeRadiusPhase),radius2=radius*radius,feather=std::max(1.0,radius*10);
        double materialBlend=std::clamp((g_rippleAge-400)/600.0,0.0,1.0);materialBlend=materialBlend*materialBlend*(3-2*materialBlend);
        double wobble=1+.10*exp(-g_rippleAge/520.0)*sin(g_rippleAge/65.0);
        double rippleRadius[3]{},rippleLife[3]{},rippleBand[3]{};
        double settled=std::clamp((g_rippleAge-450)/1350.0,0.0,1.0);settled=settled*settled*(3-2*settled);
        if(g_softRipples)for(int ring=0;ring<3;++ring){
            double phase=(g_rippleAge-450-ring*180)/900.0;
            if(phase>=0&&phase<1){double bounce=1-exp(-4*phase)*(cos(11*phase)+4.0/11*sin(11*phase));rippleRadius[ring]=std::max(1.0,hypot(rw,rh)*bounce);rippleBand[ring]=std::max(20.0,hypot(rw,rh)*.055);rippleLife[ring]=sin(phase*3.141592653589793);}
        }
        for(int py=std::max(0,int(r.top)-y);py<std::min(h,int(r.bottom)-y);++py){
          double sy=y+py+.5,dy=(sy-cy)*wobble,dy2=dy*dy;if(dy2>radius2+feather)continue;
          for(int px=std::max(0,int(r.left)-x);px<std::min(w,int(r.right)-x);++px){
            double sx=x+px+.5,dx=(sx-cx)/wobble,dist2=dx*dx+dy2;
            double reveal=std::clamp((radius2-dist2)/feather+.5,0.0,1.0);if(reveal<=0)continue;
            double coverage=1,qx=fabs(sx-(r.left+r.right)*.5)-(rw*.5-rounding),qy=fabs(sy-(r.top+r.bottom)*.5)-(rh*.5-rounding);
            if(qx>0&&qy>0&&!(g_closeButtonDraw&&sx<(r.left+r.right)*.5&&sy>(r.top+r.bottom)*.5))coverage=std::clamp(rounding+.5-hypot(qx,qy),0.0,1.0);
            double reflection=1/(1+dist2/std::max(144.0,rw*rw*.3));
            double lineDistance=std::min({fabs(dx),fabs(dy),fabs(dx-dy)*.70710678,fabs(dx+dy)*.70710678});
            double lines=g_pressRays&&dist2>144?std::clamp(2-lineDistance,0.0,1.0)*g_rayPulse:0;
            double white=std::clamp(((1+(g_closeOpacity/100.0-1)*materialBlend)*(1-materialBlend*(.25-.25*reflection))+lines*.45)*coverage*reveal,0.0,1.0);
            if(g_softRipples){double rings=0;for(int ring=0;ring<3;++ring)if(rippleLife[ring]>0){double rr=rippleRadius[ring],delta=(dist2-rr*rr)/std::max(1.0,2*rr*rippleBand[ring]);rings=std::max(rings,rippleLife[ring]/(1+delta*delta));}
                white*=1-std::clamp(g_rippleDepth/100.0*(settled*.55+rings*.8)*materialBlend,0.0,.9);
            }
            double dark=std::clamp(g_darkPulse*.13*coverage*reveal,0.0,1.0);
            UINT32 light=UINT32(lround(255*white)),a=UINT32(lround(255*std::clamp(white+dark,0.0,1.0)));
            UINT32 red=light,green=light,blue=light;
            if(g_closeButtonDraw){UINT32 color=g_captionDrawType==1?0xF04B55:g_captionDrawType==2?0x4C9CFF:0x66DDCC;red=light*((color>>16)&255)/255;green=light*((color>>8)&255)/255;blue=light*(color&255)/255;
             // Soft white spotlight follows the physical mouse within the caption button.
             double glowRadius=std::max(12.0,rh*.60),glowDistance=(sx-cx)*(sx-cx)+(sy-cy)*(sy-cy);
             double glow=.72*exp(-glowDistance/(2*glowRadius*glowRadius))*coverage*reveal;
             UINT32 ga=UINT32(lround(255*glow));
             red=ga+red*(255-ga)/255;green=ga+green*(255-ga)/255;blue=ga+blue*(255-ga)/255;a=ga+a*(255-ga)/255;
             double ix=sx-(r.left+r.right)*.5,iy=sy-(r.top+r.bottom)*.5;bool glyph=g_captionDrawType==1?(fabs(ix)<5&&fabs(iy)<5&&std::min(fabs(ix-iy),fabs(ix+iy))<.8):g_captionDrawType==2?(fabs(ix)<5&&fabs(iy)<.8):(fabs(ix)<=5&&fabs(iy)<=5&&(fabs(fabs(ix)-4.5)<.8||fabs(fabs(iy)-4.5)<.8));
             if(glyph){a=UINT32(lround(255*coverage*reveal));red=green=blue=a;}
            }

            UINT32 src=(a<<24)|(red<<16)|(green<<8)|blue;UINT32& dest=g_bits[py*g_canvasW+px];UINT32 inv=256-a;dest=src+(((dest&0xFF00FF)*inv>>8&0xFF00FF)|((dest>>8&0xFF00FF)*inv&0xFF00FF00));
          }
        }
    }
    if(g_knightCloseDraw&&!knightFalling){
      RECT r=g_closeDrawRect;double rw=r.right-r.left,rh=r.bottom-r.top,age=double(Now()-g_knightCloseStart);
      double burst=std::clamp((age-KnightCutsEnd())/double(g_knightRunExpansion),0.0,1.0);burst=burst*burst*(3-2*burst);
      // Draw glow strips, then black cores: no full-window per-pixel cut search.
      for(int pass=0;pass<2;++pass)for(int cut=0;cut<g_knightRunCount;++cut){
       double elapsed=age-cut*g_knightRunInterval;if(elapsed<=0)continue;
       double t=std::clamp((elapsed-80)/100.0,0.0,1.0),anticipation=sin(std::clamp(elapsed/110.0,0.0,1.0)*3.141592653589793);
       double glowWidth=g_knightGlowWidth.load()+anticipation*6+burst*g_knightGlowExpansion.load(),opening=std::max(1.0,t*t*std::min(double(g_knightLineWidth.load()),std::min(rw,rh)*.025)*(1-burst*(1-g_knightLineFinal/100.0)));
       if(pass==1&&t<=0)continue;
       UINT32 lookup[1024]{};int extent=std::min(511,int(ceil(pass?opening+1:opening+burst*4+glowWidth*3)));
       for(int i=0;i<=extent*2;++i){double distance=i*.5,edge=distance-opening,alpha;if(pass)alpha=std::clamp(.5-edge,0.0,1.0);else{double outside=std::max(0.0,edge-burst*4);alpha=std::min(1.0,elapsed/35)*.98*exp(-outside*outside/(2*glowWidth*glowWidth));}UINT32 a=UINT32(lround(alpha*255));lookup[i]=(a<<24)|(pass?0:(a<<16)|((a*18/255)<<8)|(a*32/255));}
       double c=KnightCutSigned(cut,0,0,rw,rh),ax=KnightCutSigned(cut,1,0,rw,rh)-c,by=KnightCutSigned(cut,0,1,rw,rh)-c;
       auto blend=[&](int px,int py,double distance){if(px<0||px>=w||py<0||py>=h)return;int index=int(fabs(distance)*2+.5);if(index>extent*2)return;UINT32 src=lookup[index];UINT32& dest=g_bits[py*g_canvasW+px];UINT32 inv=256-(src>>24);dest=src+(((dest&0xFF00FF)*inv>>8&0xFF00FF)|((dest>>8&0xFF00FF)*inv&0xFF00FF00));};
       if(fabs(ax)>=fabs(by)){for(int sy=std::max(int(r.top),y);sy<std::min(int(r.bottom),y+h);++sy){double center=r.left-(by*(sy-r.top+.5)+c)/ax,span=extent/fabs(ax);int lo=std::max(int(r.left),int(floor(center-span))),hi=std::min(int(r.right),int(ceil(center+span)));for(int sx=lo;sx<hi;++sx)blend(sx-x,sy-y,ax*(sx-r.left+.5)+by*(sy-r.top+.5)+c);}}
       else{for(int sx=std::max(int(r.left),x);sx<std::min(int(r.right),x+w);++sx){double center=r.top-(ax*(sx-r.left+.5)+c)/by,span=extent/fabs(by);int lo=std::max(int(r.top),int(floor(center-span))),hi=std::min(int(r.bottom),int(ceil(center+span)));for(int sy=lo;sy<hi;++sy)blend(sx-x,sy-y,ax*(sx-r.left+.5)+by*(sy-r.top+.5)+c);}}
      }
    }
    if(g_knightCloseDraw&&g_knightSwoon){
      static std::vector<UINT32> word;
      const int tw=160,th=64;
      if(word.empty()){
        static const BYTE letters[6][7]={{15,16,16,14,1,1,30},{17,17,17,21,21,21,10},{14,17,17,17,17,17,14},{14,17,17,17,17,17,14},{17,25,25,21,19,19,17},{0,0,0,0,0,0,0}};
        // SWOON, with block edges matching the reference.
        word.assign(tw*th,0);for(int letter=0;letter<5;++letter)for(int row=0;row<7;++row)for(int col=0;col<5;++col)if(letters[letter][row]&(1<<(4-col)))for(int yy=0;yy<4;++yy)for(int xx=0;xx<4;++xx){int px=22+letter*24+col*4+xx,py=18+row*4+yy;if(px<tw&&py<th)word[py*tw+px]=0xFFFF0C18;}
        auto mask=word;for(int py=0;py<th;++py)for(int px=0;px<tw;++px)if(!mask[py*tw+px]){bool outline=false;for(int yy=-2;yy<=2&&!outline;++yy)for(int xx=-2;xx<=2;++xx)if(px+xx>=0&&px+xx<tw&&py+yy>=0&&py+yy<th&&mask[(py+yy)*tw+px+xx]){outline=true;break;}if(outline)word[py*tw+px]=0xFF000000;}
      }
      double wordAge=double(Now()-g_knightCloseStart-KnightFallStart()),t=std::clamp(wordAge/double(KnightFallDuration()),0.0,1.0),grow=1-pow(1-std::min(1.0,t/.18),3),exit=std::clamp((t-.72)/.28,0.0,1.0),opacity=wordAge<0?0:1-exit;int drawWidth=std::max(1,tw*g_knightWordSize/100),height=std::max(2,int((3+grow*65+exit*42)*g_knightWordSize/100.0)),left=(g_closeDrawRect.left+g_closeDrawRect.right-drawWidth)/2-x,top=int((g_closeDrawRect.top+g_closeDrawRect.bottom)*.5-exit*g_knightWordRise)-height/2-y;
      if(!word.empty())for(int py=0;py<height;++py)for(int px=0;px<drawWidth;++px){int dx=left+px,dy=top+py;if(dx<0||dx>=w||dy<0||dy>=h)continue;UINT32 pixel=word[(py*th/height)*tw+px*tw/drawWidth];UINT32 alpha=UINT32(lround((pixel>>24)*opacity));UINT32 src=(alpha<<24)|(UINT32(lround(((pixel>>16)&255)*opacity))<<16)|(UINT32(lround(((pixel>>8)&255)*opacity))<<8)|UINT32(lround((pixel&255)*opacity));UINT32& dest=g_bits[dy*g_canvasW+dx];UINT32 inv=256-alpha;dest=src+(((dest&0xFF00FF)*inv>>8&0xFF00FF)|((dest>>8&0xFF00FF)*inv&0xFF00FF00));}
    }
    // The window is resized to every frame: keeping its size and only moving it is a little cheaper, but Windows can then
    // show the move a frame before the new picture, and the blur jitters.
    POINT dst = {x, y}, src = {0, 0};
    SIZE size = {w, h};
    BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
    if(!UpdateLayeredWindow(g_wnd, nullptr, &dst, &size, g_memDc, &src, 0, &bf, ULW_ALPHA))return false;
    if (!IsWindowVisible(g_wnd))
        SetWindowPos(g_wnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    KeepOverlayOnTop();
    cachedWhite=whiteOnly;cachedW=w;cachedH=h;cachedMouseX=relativeX;cachedMouseY=relativeY;cachedOpacity=g_closeOpacity;
    return true;
}

static bool Hide() {
    // Restore the native cursor before removing its replacement. Serialize with
    // the hider so it cannot re-hide it using a stale visibility request.
    AcquireSRWLockExclusive(&g_nativeVisibilityLock);
    g_overlayReady=false;g_pointerReady=false;g_staticFrame=false;
    if(g_blankActive&&g_magShow){g_magShow(TRUE);SetBlank(false);}
    ReleaseSRWLockExclusive(&g_nativeVisibilityLock);
    SetEvent(g_hideChanged);
    if (g_shown) {
        ShowWindow(g_wnd, SW_HIDE);
        g_shown = false;
    }
    return false;
}


// Keep owned raster snapshots, so cache eviction and another shape change
// during a transition cannot leave a dangling sprite or restart from an old form.
static void CaptureClickNudge(){
 POINT p{};GetCursorPos(&p);RECT rect{};LONGLONG time;
 AcquireSRWLockShared(&g_buttonLock);rect=g_buttonBounds;time=g_buttonBoundsTime;ReleaseSRWLockShared(&g_buttonLock);
 double x=0,y=1;
 if(Now()-time<250&&PtInRect(&rect,p)){
  x=(rect.left+rect.right)*.5-p.x;y=(rect.top+rect.bottom)*.5-p.y;double length=hypot(x,y);
  if(length>1){x/=length;y/=length;}else{x=y=0;}
 }
 g_clickNudgeX=x;g_clickNudgeY=y;g_clickPulse=Now();
}
static double ShapeGrowth(double t){
 t=std::clamp(t,0.0,1.0);double ease=t*t*(3-2*t);
 if(!g_shapeElastic)return .94+.06*ease;
 double u=t-1,c=3.0*g_springBounce/100.0,back=1+(c+1)*u*u*u+c*u*u;
 double amount=g_shapeElasticAmount/100.0;return 1-amount+amount*back;
}
static Sprite* TransitionSprite(const Sprite* native,HCURSOR handle,int size){
 static HCURSOR lastHandle=nullptr;static int lastSize=0;static LONGLONG start=0;
 static std::vector<UINT32> from,current;static Sprite result{};
 int side=size*3+1,hot=side/2;LONGLONG now=Now();
 bool changed=handle!=lastHandle||size!=lastSize;
 bool first=current.size()!=size_t(side)*side;
 if(changed){from=current;start=first?now-g_transitionDuration.load():now;lastHandle=handle;lastSize=size;}
 double t=first||!g_shapeTransition?1:std::clamp(double(now-start)/g_transitionDuration.load(),0.0,1.0);
 double ease=t*t*(3-2*t),grow=ShapeGrowth(t);
 g_transitionActive=t<1;
 if(g_transitionActive)g_animating=true;
 std::vector<UINT32> next(size_t(side)*side,0);
 int w=std::max(1,int(lround(size*grow))),h=std::max(1,int(lround(native->h*double(w)/native->w)));
 int ox=hot-int(lround(native->hx*double(w)/native->w)),oy=hot-int(lround(native->hy*double(w)/native->w));
 for(int y=0;y<h;y++)for(int x=0;x<w;x++)if(ox+x>=0&&oy+y>=0&&ox+x<side&&oy+y<side)next[size_t(oy+y)*side+ox+x]=Pixel(native,x,y,w,h);
 current.resize(next.size());
 for(size_t i=0;i<next.size();i++){
  UINT32 pixel=0,previous=!first&&i<from.size()?from[i]:0;
  for(int c=0;c<4;c++){double v=((previous>>(8*c))&255)*(1-ease)+((next[i]>>(8*c))&255)*ease;pixel|=UINT32(v+.5)<<(8*c);}
  current[i]=pixel;
 }
 result={side,side,hot,hot,++g_nextId,current.data()};return &result;
}
struct StretchMatrix {double xx=1,xy=0,yy=1;};
static double ScrollCompressionAt(double age,double amount){
 if(age<0||age>=350)return 0;
 // Smooth attack and decay; repeated wheel ticks retarget the spring without
 // resetting its position or velocity. The final 50 ms ease the target to zero.
 double attack=1-exp(-age/18),tail=std::clamp((350-age)/50,0.0,1.0);
 tail=tail*tail*(3-2*tail);
 return std::clamp(amount,0.0,.3)*attack*exp(-age/85)*tail;
}
static StretchMatrix SpringStretch(const Sample* pts,int n){
 static double state[3]={},velocity[3]={};static LONGLONG last=0;
 LONGLONG now=Now();double dt=last?std::clamp(double(now-last)/1000,.001,.033):.008;last=now;
 static double priorVX=0,priorVY=0;static LONGLONG reversal=0;
 double target[3]={};
 if(g_stretchEnabled&&n>1){
  int j=n-1;while(j>0&&now-pts[j-1].t<30)--j;
  double elapsed=std::max(1.0,double(now-pts[j].t));
  double vx=(pts[n-1].x-pts[j].x)*1000/elapsed,vy=(pts[n-1].y-pts[j].y)*1000/elapsed,speed=hypot(vx,vy);
  if(g_reverseElastic&&speed>120&&hypot(priorVX,priorVY)>120&&vx*priorVX+vy*priorVY<-.35*speed*hypot(priorVX,priorVY)&&now-reversal>80)reversal=now;
  if(speed>120){priorVX=vx;priorVY=vy;}else if(speed<20){priorVX=priorVY=0;}
  if(speed>1){
   double ux=vx/speed,uy=vy/speed;
   double along=g_stretchAmount/100.0*std::clamp(speed/g_stretchSpeed.load(),0.0,1.0);
   double across=1/sqrt(1+along)-1;
   // Spring the symmetric deformation itself: direction changes do not snap
   // an angle from +180 to -180 or keep stretching after the mouse stops.
   target[0]=along*ux*ux+across*uy*uy;
   target[1]=(along-across)*ux*uy;
   target[2]=along*uy*uy+across*ux*ux;
  }
 }
 if(g_magnetEnabled&&!g_typingAttached&&!g_typingVisual&&!g_knightActive&&!g_resizeAxis&&!(GetAsyncKeyState(VK_LBUTTON)&0x8000)){
  double mx=g_magnetX.load(),my=g_magnetY.load(),distance=hypot(mx,my);
  if(distance>.001){double ux=mx/distance,uy=my/distance,along=g_magnetStretch/100.0*std::min(1.0,distance),across=1/sqrt(1+along)-1;
   target[0]+=along*ux*ux+across*uy*uy;target[1]+=(along-across)*ux*uy;target[2]+=along*uy*uy+across*ux*ux;}
 }
 if(g_wheelEnabled&&g_wheelElastic){
  double compression=ScrollCompressionAt(double(now-g_wheelTime.load()),g_wheelElasticAmount/100.0);
  int axis=g_wheelHorizontal?0:2,across=g_wheelHorizontal?2:0;
  target[axis]-=compression;target[across]+=compression/(1-compression);
 }
 if(g_resizeEnabled&&g_resizeAxis.load()){
  int axis=g_resizeAxis.load();double a=g_resizeAmount/100.0;
  if(!(GetAsyncKeyState(VK_LBUTTON)&0x8000))a*=.35;
  if(axis==1){target[0]+=a;target[2]-=a*.3;}
  else if(axis==2){target[2]+=a;target[0]-=a*.3;}
  else {target[0]+=a*.4;target[2]+=a*.4;target[1]+=(axis==3?1:-1)*a*.6;}
 }
 if(g_reverseElastic&&reversal){double t=double(now-reversal)/g_reverseDuration.load();if(t<1){double pulse=-sin(t*6.283185307179586)*pow(1-t,2)*g_reverseAmount/100.0;target[0]+=pulse;target[2]+=pulse;g_animating=true;}else reversal=0;}
 double damping=28-g_springBounce*.18;bool settling=false;
 for(int c=0;c<3;c++){
  for(int i=0;i<4;i++){double step=dt/4;velocity[c]+=((target[c]-state[c])*220-damping*velocity[c])*step;state[c]+=velocity[c]*step;}
  if(fabs(target[c])<.0001&&fabs(state[c])<.0005&&fabs(velocity[c])<.002){state[c]=0;velocity[c]=0;}
  settling|=fabs(state[c])>.0005||fabs(velocity[c])>.002;
 }
 if(settling)g_animating=true;
 return {1+std::clamp(state[0],-.3,.5),std::clamp(state[1],-.3,.3),1+std::clamp(state[2],-.3,.5)};
}
struct MotionOffset {double x=0,y=0;};
static MotionOffset SpringContextOffset(const Sample* pts,int n){
 static double value[2]={},velocity[2]={};static LONGLONG last=0;LONGLONG now=Now();
 double dt=last?std::clamp(double(now-last)/1000,.001,.033):.008;last=now;double target[2]={};
 if(g_dragEnabled&&(GetAsyncKeyState(VK_LBUTTON)&0x8000)&&n>1){
  int j=n-1;while(j>0&&now-pts[j-1].t<30)--j;double elapsed=std::max(1.0,double(now-pts[j].t));
  target[0]=-std::clamp((pts[n-1].x-pts[j].x)*1000/elapsed/1400,-1.0,1.0)*g_dragAmount;
  target[1]=-std::clamp((pts[n-1].y-pts[j].y)*1000/elapsed/1400,-1.0,1.0)*g_dragAmount;
 }
 double age=double(now-g_wheelTime.load());if(g_wheelEnabled&&age>=0&&age<350){
  int axis=g_wheelHorizontal?0:1;
  target[axis]+=(g_wheelHorizontal?1:-1)*g_wheelDirection*g_wheelAmount*exp(-age/70);
 }
 double clickAge=double(now-g_clickPulse.load());
 if(g_clickNudge&&clickAge>=0&&clickAge<240){
  double kick=g_clickNudgeAmount*exp(-clickAge/45.0);
  target[0]+=g_clickNudgeX.load()*kick;target[1]+=g_clickNudgeY.load()*kick;
 }
 double damping=28-g_springBounce*.18;bool settling=false;
 for(int c=0;c<2;c++){
  for(int i=0;i<4;i++){double step=dt/4;velocity[c]+=((target[c]-value[c])*280-damping*velocity[c])*step;value[c]+=velocity[c]*step;}
  if(fabs(value[c])>.02||fabs(velocity[c])>.1)settling=true;
 }
 if(settling)g_animating=true;return {value[0],value[1]};
}
static Sprite* AnimatedSprite(const Sprite* original,int size,double angle,double pressScale,StretchMatrix stretch,MotionOffset offset){
 static std::vector<UINT32> pixels;static Sprite result{};
 double scale=double(size)/original->w;
 int w=size,h=int(lround(original->h*scale));double hx=original->hx*scale,hy=original->hy*scale;

 int left=original->w,top=original->h,right=0,bottom=0;
 for(int y=0;y<original->h;y++)for(int x=0;x<original->w;x++)if((original->px[y*original->w+x]>>24)>16){left=std::min(left,x);top=std::min(top,y);right=std::max(right,x);bottom=std::max(bottom,y);}
 double extentX=std::max(fabs(left*scale-hx),fabs((right+1)*scale-hx));
 double extentY=std::max(fabs(top*scale-hy),fabs((bottom+1)*scale-hy));
 int radius=int(ceil(hypot(extentX,extentY)*2.3))+3,side=radius*2+1;
 double pivotX=0,pivotY=0;int pivot=g_rotationPivot.load();
 if(left<=right&&top<=bottom&&pivot){
  pivotX=(pivot==2?right:(left+right)*.5)*scale-hx;
  pivotY=(pivot==2?bottom:(top+bottom)*.5)*scale-hy;
 }
 pixels.assign(size_t(side)*side,0);
 double radians=angle*3.141592653589793/180,cs=cos(radians),sn=sin(radians);
 for(int y=0;y<side;y++)for(int x=0;x<side;x++){
  double dx=x-radius-offset.x,dy=y-radius-offset.y;
  double determinant=stretch.xx*stretch.yy-stretch.xy*stretch.xy;
  double tx=(stretch.yy*dx-stretch.xy*dy)/determinant;
  dy=(-stretch.xy*dx+stretch.xx*dy)/determinant;dx=tx;
  // Rotation uses the chosen glyph pivot; pressure remains anchored at the
  // click hotspot. Invert rotation first, then pressure scaling.
  double localX=cs*(dx-pivotX)+sn*(dy-pivotY)+pivotX;
  double localY=-sn*(dx-pivotX)+cs*(dy-pivotY)+pivotY;
  double sx=(localX/pressScale+hx)/scale,sy=(localY/pressScale+hy)/scale;
  int ix=int(floor(sx)),iy=int(floor(sy));double ax=sx-ix,ay=sy-iy;double sum[4]={};
  for(int j=0;j<2;j++)for(int i=0;i<2;i++){
   int px=ix+i,py=iy+j;if(px<0||py<0||px>=original->w||py>=original->h)continue;
   double weight=(i?ax:1-ax)*(j?ay:1-ay);UINT32 color=original->px[py*original->w+px];
   for(int c=0;c<4;c++)sum[c]+=((color>>(c*8))&255)*weight;
  }
  UINT32 color=0;for(int c=0;c<4;c++)color|=UINT32(sum[c]+.5)<<(c*8);pixels[size_t(y)*side+x]=color;
 }
 result={side,side,radius,radius,++g_nextId,pixels.data()};return &result;
}
static bool ExcludeHeldRotation(HCURSOR cursor){
 if(g_skipResizeRotation&&(cursor==LoadCursorW(nullptr,IDC_SIZEWE)||cursor==LoadCursorW(nullptr,IDC_SIZENS)||cursor==LoadCursorW(nullptr,IDC_SIZENWSE)||cursor==LoadCursorW(nullptr,IDC_SIZENESW)||cursor==LoadCursorW(nullptr,IDC_SIZEALL)))return true;
 if(g_skipTextRotation&&cursor==LoadCursorW(nullptr,IDC_IBEAM))return true;
 if(g_skipBusyRotation&&(cursor==LoadCursorW(nullptr,IDC_WAIT)||cursor==LoadCursorW(nullptr,IDC_APPSTARTING)))return true;
 return false;
}
static double SpringAngle(const Sample* pts,int n,HCURSOR cursor){
 static double angle=0,velocity=0;static LONGLONG last=0;LONGLONG now=Now();
 double dt=last?std::clamp(double(now-last)/1000,.001,.033):.008;last=now;
 double target=0;
 LONGLONG shapeAge=now-g_shapePulse.load();
 if((GetAsyncKeyState(VK_LBUTTON)&0x8000)&&ExcludeHeldRotation(cursor))target=0;
 else if(g_selectionEnabled&&(GetAsyncKeyState(VK_LBUTTON)&0x8000))target=g_selectionAngle;
 else if(g_shapeBounce&&shapeAge>=0&&shapeAge<300)target=g_tiltAmount*exp(-double(shapeAge)/60);
 else if(g_tiltEnabled&&n>1){
  int j=n-1;while(j>0&&now-pts[j-1].t<30)--j;
  double elapsed=std::max(1.0,double(now-pts[j].t));
  double vx=(pts[n-1].x-pts[j].x)*1000/elapsed;
  double vy=(pts[n-1].y-pts[j].y)*1000/elapsed;
  double horizontal=std::clamp(vx/1200.0,-1.0,1.0)*g_tiltAmount;
  double vertical=std::clamp(vy/1200.0,-1.0,1.0)*g_verticalTilt;
  target=std::clamp(horizontal+vertical,-45.0,45.0);
 }
 if(g_typingAttached){double age=double(now-g_typingKeyTime.load());if(age>=0&&age<450)target+=-14.0*exp(-age/110.0);}
 if(g_magnetEnabled&&!(GetAsyncKeyState(VK_LBUTTON)&0x8000)&&!g_typingAttached)target+=g_magnetAngle.load();
 double damping=28-g_springBounce*.18;
 for(int i=0;i<4;i++){double step=dt/4;velocity+=((target-angle)*220-velocity*damping)*step;angle+=velocity*step;}
 g_angleAnimating=fabs(angle)>.08||fabs(velocity)>.15||fabs(target)>.08;
 return angle;
}
static double SpringPressure(){
 static double scale=1,velocity=0;static LONGLONG last=0;LONGLONG now=Now();
 double dt=last?std::clamp(double(now-last)/1000,.001,.033):.008;last=now;
 bool held=g_pressureEnabled&&((GetAsyncKeyState(VK_LBUTTON)&0x8000)!=0);
 double target=held?1.0-g_pressureAmount/100.0:1.0;
 double damping=28-g_springBounce*.18;
 for(int i=0;i<4;i++){double step=dt/4;velocity+=((target-scale)*260-damping*velocity)*step;scale+=velocity*step;}
 if(!held&&fabs(scale-1)<.0005&&fabs(velocity)<.002){scale=1;velocity=0;}
 g_animating=g_angleAnimating||held||fabs(scale-1)>.0005||fabs(velocity)>.002;
 return std::clamp(scale,.65,1.15);
}
// Draws one frame of the blur; false if there is nothing to draw (then the overlay is hidden).
struct TrailSample {double x,y;};
static TrailSample ClassicPositionAt(const Sample* points,int count,double time){
 if(time<=points[0].t)return {double(points[0].x),double(points[0].y)};
 for(int i=1;i<count;i++)if(time<=points[i].t){
  double t=(time-points[i-1].t)/std::max<LONGLONG>(1,points[i].t-points[i-1].t);
  return {points[i-1].x+(points[i].x-points[i-1].x)*t,points[i-1].y+(points[i].y-points[i-1].y)*t};
 }
 return {double(points[count-1].x),double(points[count-1].y)};
}
static bool Render() {
    if(g_protectedDesktop){g_animating=false;g_idleActive=false;g_busyActive=false;return Hide();}
    // Recheck at draw time as well: a hide event can arrive between sampling
    // and rendering, especially while a stationary frame is retained.
    CURSORINFO native{sizeof(native)};
    if(!GetCursorInfo(&native)||(!ApplicationShowsCursor(native)&&!g_typingAttached&&!g_typingVisual)){
     g_animating=false;g_idleActive=false;g_busyActive=false;return Hide();
    }
    static Sample pts[MAX_SAMPLES];
    static Copy copies[COPY_BUF];
    int np, size;
    HCURSOR handle;
    bool visible;
    EnterCriticalSection(&g_gate);
    visible = g_curVisible;
    handle = g_curHandle;
    size = g_monCursor;
    np = g_histN;
    memcpy(pts, g_hist, np * sizeof(Sample));
    LeaveCriticalSection(&g_gate);
    if (!visible || np < 1) {g_animating=false;return Hide();}
    if (g_spritesStale) {   // the cursor scheme, size or colour may have changed
        g_spritesStale = 0;
        FreeSprites();
    }

    Sample realMouse=pts[np-1];
    bool typingFrame=TypingPoints(pts,np);
    bool knightFrame=!typingFrame&&KnightPoints(pts,np);
    if(g_typingAttached)handle=LoadCursorW(nullptr,IDC_IBEAM);
    if(IsEmpty(handle))return Hide();
    bool nativeAnimated=false;
    HCURSOR picture=AnimatedCursorFrame(handle,nativeAnimated);
    Sprite* sp=GetSprite(picture);
    if(!sp)return Hide();
    g_lastSprite=sp;g_drawBusyCursor=IsBusyCursor(handle);
    g_pointerReady=true;

    Sample last = pts[np - 1];
    bool anyMove = false;
    for (int i = 0; i < np && !anyMove; i++) anyMove = pts[i].x != last.x || pts[i].y != last.y;
    bool animated=g_tiltEnabled||g_selectionEnabled||g_shapeBounce||g_pressureEnabled||g_shapeTransition||g_stretchEnabled||g_magnetEnabled||g_wheelEnabled||g_dragEnabled||g_resizeEnabled||g_clickNudge||g_idleActive||g_busyActive;
    double angle=SpringAngle(pts,np,handle);
    double pressScale=SpringPressure();
    StretchMatrix stretch=SpringStretch(pts,np);
    MotionOffset offset=SpringContextOffset(pts,np);
    if(nativeAnimated||typingFrame||knightFrame)g_animating=true;
    if(g_idleActive){
     IdlePose pose=IdlePoseAt(g_idleKind,double(Now()-g_idleStart.load())/g_idleDuration.load(),g_idleAmount);
     angle+=pose.angle;offset.x+=pose.x;offset.y+=pose.y;pressScale*=pose.scale;g_animating=true;
    }
    static HCURSOR busyHandle=nullptr;static LONGLONG busyStart=0;
    if(g_busyActive&&IsBusyCursor(handle)){
     if(handle!=busyHandle){busyHandle=handle;busyStart=Now();}
     double phase=double(Now()-busyStart)/g_busyHopDuration.load();
     offset.y+=BusyHopAt(phase,g_busyHopAmount);
     if(g_busyHopElastic&&g_busyHopAmount>0){
      double sy=BusyHopScaleAt(phase,g_busyHopElasticAmount/100.0);
      // S*M*S keeps the existing symmetric stretch positive and preserves its determinant.
      stretch.xx/=sy;stretch.yy*=sy;
     }
     g_animating=true;
    }else busyHandle=nullptr;
    int drawSize=size;
    if(g_shapeTransition){sp=TransitionSprite(sp,handle,size);drawSize=sp->w;}else g_transitionActive=false;
    if (!anyMove&&!animated&&!g_alwaysVisible) return Hide();

    // the picture at the size Windows draws the cursor on this monitor
    double f = (double)drawSize / sp->w;
    int sw = drawSize, sh = (int)lround(sp->h * f), hx = (int)lround(sp->hx * f), hy = (int)lround(sp->hy * f);
    if(animated){sp=AnimatedSprite(sp,drawSize,angle,pressScale,stretch,offset);sw=sp->w;sh=sp->h;hx=sp->hx;hy=sp->hy;}

    // Idle trails flow to the right, independent of the bobbing direction.
    // Build only the trail geometry after tilt/stretch have sampled real motion.
    if(knightFrame&&g_idleTrailWhen!=2&&(g_idleTrailWhen==0||g_knightActive)){
        const double duration=std::max(1,g_trailStyle==1?g_classicDuration.load():g_trailMs.load());
        const double phase=double(Now()-g_knightStart)/1000.0;
        const double amplitude=g_knightVertical.load()*g_idleTrailWave/100.0;
        const Sample head=last;np=64;
        for(int i=0;i<np;++i){
            double age=double(np-1-i)/(np-1),delay=age*.42;
            double along=age*size*g_idleTrailSpacing*(g_idleTrailCopies+1)/32.0;
            double wave=(sin((phase-delay)*2.35*g_knightVerticalSpeed/100.0)-sin(phase*2.35*g_knightVerticalSpeed/100.0))*amplitude;
            int direction=g_idleTrailDirection;double dx=direction==0?along:direction==1?-along:wave;
            double dy=direction==2?-along:direction==3?along:wave;
            pts[i]={head.x+LONG(lround(dx)),head.y+LONG(lround(dy)),head.t-LONGLONG(lround(age*duration))};
        }
    }
    int minX = INT_MAX, minY = INT_MAX, maxX = INT_MIN, maxY = INT_MIN;
    double total = 0;
    for (int i = 0; i < np; i++) {
        if (pts[i].x - hx < minX) minX = pts[i].x - hx;
        if (pts[i].y - hy < minY) minY = pts[i].y - hy;
        if (pts[i].x - hx + sw > maxX) maxX = pts[i].x - hx + sw;
        if (pts[i].y - hy + sh > maxY) maxY = pts[i].y - hy + sh;
        if (i) total += Dist(pts[i - 1], pts[i]);
    }
    Sprite* shadow=nullptr;int shadowX=0,shadowY=0;double shadowOpacity=0;
    if(typingFrame&&g_typingOriginShadow){
        if(!g_originShadowSprite){bool empty=false;g_originShadowSprite=Grab(LoadCursorW(nullptr,IDC_ARROW),&empty);}
        shadow=g_originShadowSprite;
        if(shadow){
            double sf=double(size)/shadow->w;shadowX=realMouse.x-int(lround(shadow->hx*sf));shadowY=realMouse.y-int(lround(shadow->hy*sf));
            int gh=int(lround(shadow->h*sf));shadowOpacity=(g_typingAttached||hypot(double(last.x-realMouse.x),double(last.y-realMouse.y))>.75)?.28:0;
            minX=std::min(minX,shadowX);minY=std::min(minY,shadowY);maxX=std::max(maxX,shadowX+size);maxY=std::max(maxY,shadowY+gh);
        }
    }
    double step = total / MAX_COPIES > 1 ? total / MAX_COPIES : 1;   // dense, faint copies read as a smooth blur
    double cover = COVER_PX * size / 32.0, peak = g_opacity / 100.0, trail = g_trailMs;
    LONGLONG now = Now();
    int n = 0;
    if(knightFrame&&g_idleTrailWhen!=2&&(g_idleTrailWhen==0||g_knightActive)&&g_idleTrailOpacity>0){
        for(int i=g_idleTrailCopies;i>=1;--i){double fraction=double(i)/(g_idleTrailCopies+1);double time=last.t-(g_trailStyle==1?g_classicDuration.load():g_trailMs.load())*fraction;auto point=ClassicPositionAt(pts,np,time);float alpha=float(g_idleTrailOpacity/100.0*pow(1-fraction,1.7));if(alpha*255>=.5)copies[n++]={int(lround(point.x))-hx-minX,int(lround(point.y))-hy-minY,alpha};}
    }
    if(!knightFrame&&!(typingFrame&&g_typingAttached&&!g_typingTrails)&&g_trailStyle==1&&g_classicOpacity>0){
     int count=g_classicCopies;double duration=g_classicDuration;
     // Fixed temporal spacing, interpolated every render frame. Unlike copying
     // raw mouse reports, this is independent of the device's polling rate.
     for(int i=count;i>=1;i--){
      double fraction=double(i)/(count+1),time=now-duration*fraction;
      if(time<pts[0].t)continue;
      auto point=ClassicPositionAt(pts,np,time);
      double distance=hypot(point.x-last.x,point.y-last.y);
      double separation=std::clamp(distance/(size*.35),0.0,1.0);
      separation=separation*separation*(3-2*separation);
      float alpha=float(g_classicOpacity/100.0*(1-fraction)*(1-fraction)*separation);
      if(alpha*255<.5)continue;
      copies[n++]={int(lround(point.x))-hx-minX,int(lround(point.y))-hy-minY,alpha};
     }
    }
    if(!knightFrame&&!(typingFrame&&g_typingAttached&&!g_typingTrails)&&g_trailStyle==2)for (int i = 1; i < np && n < COPY_BUF; i++) {
        Sample a = pts[i - 1], b = pts[i];
        double dx = b.x - a.x, dy = b.y - a.y, d = sqrt(dx * dx + dy * dy);
        if (d < 0.5) continue;
        int k = (int)(d / step);
        if (k < 1) k = 1;
        for (int c = 0; c < k && n < COPY_BUF; c++) {
            double t = (double)c / k;
            double life = 1.0 - (now - (a.t + (b.t - a.t) * t)) / trail;
            if (life <= 0) continue;
            life *= life;   // steeper fade
            double alpha = peak * life * step / cover;   // copies overlap by about cover/step: each gets that share
            if (alpha * 255 < 0.5) continue;
            copies[n++] = {(int)lround(a.x + dx * t) - hx - minX, (int)lround(a.y + dy * t) - hy - minY,
                           alpha > 1 ? 1.0f : (float)alpha};
        }
    }
    if((g_alwaysVisible||(animated&&g_blankActive))&&n<COPY_BUF)copies[n++]={last.x-hx-minX,last.y-hy-minY,1.0f};
    if(typingFrame&&g_typingAttached&&now-g_typingKeyTime>=1000){
        double phase=double(now-g_typingKeyTime-1000)/g_typingFadePeriod.load();
        float fade=float(.25+.75*(.5+.5*cos(phase*6.283185307179586)));
        for(int i=0;i<n;i++)copies[i].a*=fade;
    }
    int cursorMinX=minX,cursorMinY=minY;
    RECT closeRect{};HWND effectWindow=nullptr;LONGLONG closeTime=0;int captionType=1;
    AcquireSRWLockShared(&g_closeLock);closeRect=g_closeTarget;effectWindow=g_closeSource;captionType=g_captionTargetType;closeTime=g_closeTime;ReleaseSRWLockShared(&g_closeLock);
    static HWND heldTitle=nullptr;static bool previousTitleDown=false;
    bool titleDown=(GetAsyncKeyState(VK_LBUTTON)&0x8000)!=0;POINT point{realMouse.x,realMouse.y};
    HWND root=GetAncestor(WindowFromPoint(point),GA_ROOT);if(root==g_wnd)root=GetForegroundWindow();DWORD_PTR titleHit=0;
    bool caption=g_titleDragWhite&&!typingFrame&&!knightFrame&&!g_paused&&!g_protectedDesktop&&root&&SendMessageTimeoutW(root,WM_NCHITTEST,0,MAKELPARAM(point.x,point.y),SMTO_ABORTIFHUNG|SMTO_BLOCK,2,&titleHit)&&titleHit==HTCAPTION;
    g_titleHoverWhite=caption;
    if(titleDown&&!previousTitleDown&&caption)heldTitle=root;
    if(!titleDown||!IsWindow(heldTitle)||g_paused||g_protectedDesktop||!g_titleDragWhite)heldTitle=nullptr;previousTitleDown=titleDown;
    bool closeButtonHover=g_closeEnabled&&!typingFrame&&!knightFrame&&!g_paused&&!g_protectedDesktop&&now-closeTime<250&&PtInRect(&closeRect,point);
    if(g_closeEnabled&&(titleHit==HTCLOSE||titleHit==HTMINBUTTON||titleHit==HTMAXBUTTON)&&!typingFrame&&!knightFrame&&!g_paused&&!g_protectedDesktop){
     struct TitleInfo {DWORD size;RECT title;DWORD state[6];RECT rect[6];} info{};info.size=sizeof(info);DWORD_PTR ignored=0;
     captionType=titleHit==HTMINBUTTON?2:titleHit==HTMAXBUTTON?3:1;int index=captionType==2?2:captionType==3?3:5;
     if(SendMessageTimeoutW(root,0x033F,0,(LPARAM)&info,SMTO_ABORTIFHUNG|SMTO_BLOCK,2,&ignored)&&PtInRect(&info.rect[index],point)){closeRect=info.rect[index];effectWindow=root;closeButtonHover=true;}
    }
    if(closeButtonHover&&captionType==1&&!g_closeMouseEffect)closeButtonHover=false;
    if(closeButtonHover)g_titleHoverWhite=true;
    bool closeHover=closeButtonHover;
    g_knightCloseDraw=false;
    if(g_knightCloseWindow){
      LONGLONG age=Now()-g_knightCloseStart;
      if(!IsWindow(g_knightCloseWindow)||g_protectedDesktop||g_paused){if(g_knightWindowHidden&&IsWindow(g_knightCloseWindow))ShowWindow(g_knightCloseWindow,SW_SHOWNOACTIVATE);g_knightWindowHidden=false;g_knightCloseWindow=nullptr;g_knightSnapshot.clear();g_knightPieces.clear();}
      else if(age>=KnightFallStart()+KnightFallDuration()){HWND target=g_knightCloseWindow;if(g_knightWindowHidden)ShowWindow(target,SW_SHOWNOACTIVATE);g_knightWindowHidden=false;g_knightCloseWindow=nullptr;g_knightSnapshot.clear();g_knightPieces.clear();PostMessageW(target,WM_SYSCOMMAND,SC_CLOSE,0);}
      else {if(age<KnightFallStart()&&g_knightSnapshot.empty())CaptureKnightWindow(g_knightCloseWindow);if(age>=KnightFallStart()&&!g_knightSnapshot.empty()&&!g_knightWindowHidden){ShowWindow(g_knightCloseWindow,SW_HIDE);g_knightWindowHidden=true;}effectWindow=g_knightCloseWindow;closeButtonHover=false;closeHover=true;g_knightCloseDraw=true;}
    }

    if(heldTitle&&!closeButtonHover&&!g_knightCloseDraw){effectWindow=heldTitle;closeHover=true;g_titleHoverWhite=true;}
    // Fast geometry path: no accessibility call or 60ms worker interval here.
    if(closeHover&&!closeButtonHover&&effectWindow){RECT frame{};using FrameFn=HRESULT(WINAPI*)(HWND,DWORD,PVOID,DWORD);static HMODULE dwm=LoadLibraryW(L"dwmapi.dll");static auto frameFn=dwm?(FrameFn)GetProcAddress(dwm,"DwmGetWindowAttribute"):nullptr;
     static HWND insetWindow=nullptr;static RECT insets{};RECT native{};GetWindowRect(effectWindow,&native);
     if(insetWindow!=effectWindow){if(!frameFn||FAILED(frameFn(effectWindow,9,&frame,sizeof(frame))))frame=native;insets={frame.left-native.left,frame.top-native.top,native.right-frame.right,native.bottom-frame.bottom};insetWindow=effectWindow;}
     frame={native.left+insets.left,native.top+insets.top,native.right-insets.right,native.bottom-insets.bottom};
     if(frame.right>frame.left&&frame.bottom>frame.top&&size_t(frame.right-frame.left)*(frame.bottom-frame.top)<=16000000)closeRect=frame;else closeHover=false;
    }
    if(g_knightCloseDraw&&!g_knightSnapshot.empty())closeRect=g_knightSnapshotRect;
    static LONGLONG closeLast=0,ballStart=0;static double closePhase=0;static HWND previousEffect=nullptr;static bool previousButton=false;
    double closeDt=closeLast?std::clamp(double(now-closeLast),0.0,40.0):8;closeLast=now;
    if(closeHover){if(previousEffect!=effectWindow||previousButton!=closeButtonHover||(closeButtonHover&&g_captionDrawType!=captionType)){ballStart=now;closePhase=0;}g_closeDrawRect=closeRect;g_closeButtonDraw=closeButtonHover;g_captionDrawType=captionType;closePhase=std::clamp(double(now-ballStart-70)/(g_closeDuration.load()*1.0),0.0,1.0);}else closePhase=std::max(0.0,closePhase-closeDt/g_closeDuration.load());
    double age=double(now-ballStart),strength=g_pulseStrength/100.0;g_rippleAge=age;
    g_whiteFlash=1;
    g_darkPulse=0;
    g_rayPulse=closeHover&&age>70&&age<360?pow(sin((age-70)/290.0*3.141592653589793),2)*strength:0;
    g_closeMix=(closeHover||closePhase>0)?1:0;
    double hoverTarget=g_titleHoverWhite?1.0:0.0;g_hoverWhiteMix+=(hoverTarget-g_hoverWhiteMix)*(1-exp(-closeDt/65.0));if(fabs(hoverTarget-g_hoverWhiteMix)<.002)g_hoverWhiteMix=hoverTarget;else g_animating=true;
    double back=closePhase-1;g_closeRadiusPhase=closeHover?(closePhase>=1?1:1-exp(-4*closePhase)*(cos(11*closePhase)+4.0/11*sin(11*closePhase))):closePhase;
    g_closeMouse=point;previousEffect=closeHover?effectWindow:nullptr;previousButton=closeButtonHover;g_whiteFollowWindow=closeHover?effectWindow:nullptr;
    if(g_closeMix>.001){minX=std::min(minX,int(g_closeDrawRect.left)-(g_knightCloseDraw?160:0));minY=std::min(minY,int(g_closeDrawRect.top));maxX=std::max(maxX,int(g_closeDrawRect.right)+(g_knightCloseDraw?160:0));maxY=std::max(maxY,int(g_closeDrawRect.bottom));if(closeHover||closePhase>0)g_animating=true;
     for(int i=0;i<n;++i){copies[i].x+=cursorMinX-minX;copies[i].y+=cursorMinY-minY;}
    }
    if(g_knightCloseDraw){maxY=std::max(maxY,g_knightFloor);}
    if(closeHover)for(int i=0;i<n;++i)copies[i].a=0;
    g_closeVisual=closeHover;
    if (!n) return Hide();
    g_drawFailed = !Draw(sp, sw, sh, copies, n, minX, minY, maxX - minX, maxY - minY,shadow,shadowX-minX,shadowY-minY,size,shadowOpacity);
    if (g_drawFailed) return Hide();
    g_shown = true;
    g_staticFrame=!anyMove&&!g_animating&&!g_transitionActive&&!g_idleActive&&!g_busyActive;
    if(!g_overlayReady.exchange(true)){SetEvent(g_mouseWake);SetEvent(g_hideChanged);}
    return true;
}

// Raw mouse input: while on, Windows tells the overlay about every mouse report (even while other apps are active), so
// the sampler can sleep until the mouse really moves. Off while paused for a fullscreen game.
static bool ListenToMouse(bool on) {
    // generic desktop / mouse
    RAWINPUTDEVICE devices[]={{1,2,on?(DWORD)RIDEV_INPUTSINK:(DWORD)RIDEV_REMOVE,on?g_wnd.load():nullptr},{1,6,on?(DWORD)RIDEV_INPUTSINK:(DWORD)RIDEV_REMOVE,on?g_wnd.load():nullptr}};
    if (RegisterRawInputDevices(devices, ARRAYSIZE(devices), sizeof(RAWINPUTDEVICE))) return true;
    Wh_Log(L"RegisterRawInputDevices(%d) failed: %lu", on, GetLastError());
    return false;
}

// Whether a window belongs to a process with more rights than this one (an administrator app such as Task Manager or
// Windhawk itself): this process may not open its token, and Windows does not send it raw mouse input meanwhile.
static bool HigherRights(HWND w) {
    DWORD pid = 0;
    if (!w || !GetWindowThreadProcessId(w, &pid)) return false;
    HANDLE p = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (!p) return true;
    HANDLE token = nullptr;
    bool denied = !OpenProcessToken(p, TOKEN_QUERY, &token);
    if (token) CloseHandle(token);
    CloseHandle(p);
    return denied;
}

static void CALLBACK ForegroundChanged(HWINEVENTHOOK, DWORD, HWND w, LONG, LONG, DWORD, DWORD) {
    KeepOverlayOnTop();
    g_typingGeneration++;g_typingRequested=false;g_typingAttached=false;SetEvent(g_moved);
    bool admin = HigherRights(w);
    if (admin != g_adminFront) Wh_Log(L"Administrator window in front: %d", admin);
    g_adminFront = admin;
    g_recheck = true;   // a fullscreen app may also have come to the front or left it
    SetEvent(g_mouseWake);
}

static void CALLBACK OverlayOrderChanged(HWINEVENTHOOK,DWORD,HWND hwnd,LONG object,LONG,DWORD,DWORD){
 if(hwnd==g_wnd||object==OBJID_CURSOR)return;
 KeepOverlayOnTop();
}

static void CALLBACK CursorStateChanged(HWINEVENTHOOK,DWORD event,HWND,LONG object,LONG,DWORD,DWORD){
 if(object!=OBJID_CURSOR||(event!=EVENT_OBJECT_SHOW&&event!=EVENT_OBJECT_HIDE&&event!=EVENT_OBJECT_NAMECHANGE))return;
 SetEvent(g_mouseWake);SetEvent(g_moved);
}

static LRESULT CALLBACK OverlayProc(HWND h, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_INPUT: {   // Movement and button reports drive the animation.
            RAWINPUT ri{};
            UINT size = sizeof(ri);
            if (GetRawInputData((HRAWINPUT)lp, RID_INPUT, &ri, &size, sizeof(RAWINPUTHEADER)) != (UINT)-1 &&
                ri.header.dwType == RIM_TYPEMOUSE &&
                (ri.data.mouse.lLastX || ri.data.mouse.lLastY || ri.data.mouse.usButtonFlags || (ri.data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE))){
                g_typingGeneration++;g_typingRequested=false;g_typingAttached=false;
                g_idleActive=false;g_idleInputGeneration.fetch_add(1);
                if(ri.data.mouse.usButtonFlags&(RI_MOUSE_WHEEL|RI_MOUSE_HWHEEL)){
                 SHORT delta=static_cast<SHORT>(ri.data.mouse.usButtonData);
                 if(delta){g_wheelHorizontal=(ri.data.mouse.usButtonFlags&RI_MOUSE_HWHEEL)!=0;g_wheelDirection=delta>0?1:-1;g_wheelTime=Now();}
                }
                if(ri.data.mouse.usButtonFlags&(RI_MOUSE_LEFT_BUTTON_DOWN|RI_MOUSE_RIGHT_BUTTON_DOWN|RI_MOUSE_MIDDLE_BUTTON_DOWN))CaptureClickNudge();
                SetEvent(g_mouseWake);SetEvent(g_moved);
            }
            if(ri.header.dwType==RIM_TYPEKEYBOARD&&!(ri.data.keyboard.Flags&RI_KEY_BREAK)&&g_typingEnabled&&!(GetAsyncKeyState(VK_CONTROL)&0x8000)&&!(GetAsyncKeyState(VK_MENU)&0x8000)&&!(GetAsyncKeyState(VK_LWIN)&0x8000)&&!(GetAsyncKeyState(VK_RWIN)&0x8000)){
                USHORT key=ri.data.keyboard.VKey;
                if((key>=VK_SPACE&&key<='Z')||(key>=VK_NUMPAD0&&key<=VK_DIVIDE)||(key>=VK_OEM_1&&key<=VK_OEM_102)||key==VK_BACK||key==VK_RETURN||key==VK_PROCESSKEY||key==VK_PACKET){g_typingKeyTime=Now();g_typingRequested=true;g_typingGeneration++;SetEvent(g_moved);SetEvent(g_mouseWake);}
            }
            break;   // then DefWindowProc, which cleans up after the input
        }
        case WM_APP_PAUSED:
            ListenToMouse(!g_paused);
            return 0;
        case WM_SETTINGCHANGE:
        case WM_DISPLAYCHANGE:   // monitors, scaling or cursor scheme changed
            g_monStale = 1;
            g_spritesStale = 1;
            break;
    }
    return DefWindowProcW(h, msg, wp, lp);
}

// Draws when the mouse moves, at most once per half refresh of the monitor, and keeps drawing until the trail has faded
// after a stop. Idle, it wakes once more to give back memory, then sleeps until the mouse moves.
static LRESULT CALLBACK KnightCloseMouseHook(int code,WPARAM message,LPARAM data){
 static HWND pressed=nullptr;static RECT button{};
 if(code==HC_ACTION){auto* mouse=reinterpret_cast<MSLLHOOKSTRUCT*>(data);
  if(message==WM_LBUTTONDOWN&&g_knightCloseEnabled&&g_knightEnabled&&g_knightDuration==0&&!g_protectedDesktop&&!g_paused){
   AcquireSRWLockShared(&g_closeLock);HWND target=g_closeSource;RECT r=g_closeTarget;bool valid=g_captionTargetType==1&&Now()-g_closeTime<250;ReleaseSRWLockShared(&g_closeLock);
   if(valid&&target&&IsWindow(target)&&PtInRect(&r,mouse->pt)){pressed=target;button=r;return 1;}
  }
  if(message==WM_LBUTTONUP&&pressed){HWND target=pressed;pressed=nullptr;if(IsWindow(target)&&PtInRect(&button,mouse->pt)&&!g_knightCloseWindow){g_knightRunCount=g_knightSlashCount;g_knightRunInterval=g_knightSlashInterval;g_knightRunExpansion=g_knightExpansionMs;g_knightRunPause=g_knightPauseMs;g_knightSnapshot.clear();g_knightPieces.clear();g_knightWindowHidden=false;g_knightCloseWindow=target;g_knightCloseStart=Now();g_animating=true;SetEvent(g_moved);}return 1;}
 }
 return CallNextHookEx(nullptr,code,message,data);
}
static DWORD WINAPI RenderThread(void*) {
    HINSTANCE inst = GetModuleHandleW(nullptr);
    WNDCLASSW wc = {};
    wc.lpfnWndProc = OverlayProc;
    wc.hInstance = inst;
    wc.lpszClassName = L"PointerAfterimageMotionBlur";
    RegisterClassW(&wc);
    g_wnd = CreateWindowExW(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_NOACTIVATE,
                            wc.lpszClassName, L"", WS_POPUP, 0, 0, 1, 1, nullptr, nullptr, inst, nullptr);
    g_memDc = CreateCompatibleDC(nullptr);
    HWINEVENTHOOK hook = nullptr,cursorHook=nullptr,orderHook=nullptr;
    if (!g_wnd || !g_memDc || !ListenToMouse(true)) {
        Wh_Log(L"Could not set up the overlay window");
        if (g_wnd) DestroyWindow(g_wnd);
        g_wnd = nullptr;
    } else {
        hook = SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, nullptr, ForegroundChanged, 0, 0,
                               WINEVENT_OUTOFCONTEXT);
        cursorHook=SetWinEventHook(EVENT_OBJECT_SHOW,EVENT_OBJECT_NAMECHANGE,nullptr,CursorStateChanged,0,0,WINEVENT_OUTOFCONTEXT);
        orderHook=SetWinEventHook(EVENT_OBJECT_REORDER,EVENT_OBJECT_REORDER,nullptr,OverlayOrderChanged,0,0,WINEVENT_OUTOFCONTEXT);
        g_adminFront = HigherRights(GetForegroundWindow());
    }
    HHOOK knightCloseHook=SetWindowsHookExW(WH_MOUSE_LL,KnightCloseMouseHook,GetModuleHandleW(nullptr),0);
    SetEvent(g_ready);

    LONGLONG lastDraw = -1000;
    bool trimmed = true, pending = false;   // pending: the mouse moved, but too soon after the last frame
    while (!g_stop && g_wnd) {
        LONGLONG wait = g_monGap - (Now() - lastDraw);
        DWORD timeout = pending || (g_shown&&!g_staticFrame) || g_animating ? (wait > 0 ? (DWORD)wait : 0) : trimmed ? INFINITE : 1500;
        // Also repair ordering while the cursor is stationary, without redrawing
        // its pixels or showing a cursor deliberately hidden by the application.
        if(g_shown)timeout=std::min<DWORD>(timeout,250);
        DWORD r = MsgWaitForMultipleObjects(1, &g_moved, FALSE, timeout, QS_ALLINPUT);
        if (g_stop) break;
        KeepOverlayOnTop();
        if (r == WAIT_OBJECT_0 + 1) {
            MSG m;
            while (PeekMessageW(&m, nullptr, 0, 0, PM_REMOVE)) DispatchMessageW(&m);
            continue;
        }
        if (r == WAIT_OBJECT_0) pending = true;
        if(!pending&&g_shown&&g_staticFrame&&!g_animating)continue;
        if (!pending && !g_shown && !g_animating) {   // idle for 1.5 s: give back the memory that can be rebuilt
            ReleaseCanvas();
            free(g_pic);
            g_pic = nullptr;
            trimmed = true;
            continue;
        }
        if (Now() - lastDraw < g_monGap) continue;
        pending = false;
        if (Render()) {
            lastDraw = Now();
            trimmed = false;
        }
    }

    if(g_knightWindowHidden&&IsWindow(g_knightCloseWindow))ShowWindow(g_knightCloseWindow,SW_SHOWNOACTIVATE);g_knightWindowHidden=false;
    if(knightCloseHook)UnhookWindowsHookEx(knightCloseHook);
    if (hook) UnhookWinEvent(hook);
    if(cursorHook)UnhookWinEvent(cursorHook);
    if(orderHook)UnhookWinEvent(orderHook);
    if (g_wnd) {
        ListenToMouse(false);
        DestroyWindow(g_wnd);
        g_wnd = nullptr;
    }
    UnregisterClassW(wc.lpszClassName, inst);
    ReleaseCanvas();
    if (g_memDc) DeleteDC(g_memDc);
    g_memDc = nullptr;
    free(g_pic);
    g_pic = nullptr;
    FreeSprites();
    return 0;
}

// ---- mod lifecycle ----

static LPTOP_LEVEL_EXCEPTION_FILTER g_prevFilter;

// This process exists only for this mod: if it ever crashes while the real cursor is hidden, give the cursor back.
static LONG WINAPI CrashFilter(EXCEPTION_POINTERS* e) {
    if (g_blankActive&&g_magShow) g_magShow(TRUE);
    return g_prevFilter ? g_prevFilter(e) : EXCEPTION_CONTINUE_SEARCH;
}

// Signals every thread to finish, waits for them, and makes sure the user's cursors are back.
static void StopThreads() {
    g_stop = true;
    SetEvent(g_mouseWake);
    SetEvent(g_moved);
    SetEvent(g_hideChanged);
    for (HANDLE& t : g_threads)
        if (t) {
            WaitForSingleObject(t, 5000);
            CloseHandle(t);
            t = nullptr;
        }
    if (g_blankActive) {
        ShowNativeCursor();
        SetBlank(false);
    }
}

BOOL WhTool_ModInit() {
    Wh_Log(L"Init");
    LoadSettings();
    QueryPerformanceFrequency(&g_freq);
    QueryPerformanceCounter(&g_start);
    InitializeCriticalSection(&g_gate);
    HANDLE token;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token)) {
        alignas(TOKEN_USER) BYTE buf[SECURITY_MAX_SID_SIZE + sizeof(TOKEN_USER)];
        DWORD len;
        LPWSTR sid;
        if (GetTokenInformation(token, TokenUser, buf, sizeof(buf), &len) &&
            ConvertSidToStringSidW(((TOKEN_USER*)buf)->User.Sid, &sid)) {
            swprintf_s(g_hiddenKey, L"cursorsHidden_%s", sid);
            LocalFree(sid);
        }
        CloseHandle(token);
    }
    if (Wh_GetIntValue(g_hiddenKey, 0)) {   // an earlier run was killed while hiding the cursor
        Wh_Log(L"Restoring the cursors after an earlier run");
        ReloadCursors();
        if(LoadMagnification()&&g_magInit()){ShowNativeCursor();g_magUninit();}
        Wh_SetIntValue(g_hiddenKey, 0);
    }
    g_prevFilter = SetUnhandledExceptionFilter(CrashFilter);

    // Windows 11 ignores a finer timer for a process with no visible window; the short waits here need it while moving.
    PROCESS_POWER_THROTTLING_STATE pt = {PROCESS_POWER_THROTTLING_CURRENT_VERSION,
                                         PROCESS_POWER_THROTTLING_IGNORE_TIMER_RESOLUTION, 0};
    SetProcessInformation(GetCurrentProcess(), ProcessPowerThrottling, &pt, sizeof(pt));

    g_moved = CreateEventW(nullptr, FALSE, FALSE, nullptr);
    g_mouseWake = CreateEventW(nullptr, FALSE, FALSE, nullptr);
    g_hideChanged = CreateEventW(nullptr, FALSE, FALSE, nullptr);
    g_ready = CreateEventW(nullptr, TRUE, FALSE, nullptr);
    if (!g_moved || !g_mouseWake || !g_hideChanged || !g_ready) return FALSE;

    // The render thread first: it creates the overlay window that receives the mouse input, and the others need it.
    LPTHREAD_START_ROUTINE procs[4] = {RenderThread, SampleThread, HideThread, ContextThread};
    for (int i = 0; i < 4; i++) {
        g_threads[i] = CreateThread(nullptr, 0, procs[i], nullptr, 0, nullptr);
        if (!g_threads[i]) {
            Wh_Log(L"CreateThread failed");
            StopThreads();
            return FALSE;
        }
        if (i < 2) SetThreadPriority(g_threads[i], THREAD_PRIORITY_ABOVE_NORMAL);
        if (i == 0 && (WaitForSingleObject(g_ready, 5000) != WAIT_OBJECT_0 || !g_wnd)) {
            StopThreads();
            return FALSE;
        }
    }
    return TRUE;
}

void WhTool_ModSettingsChanged() {
    Wh_Log(L"SettingsChanged");
    LoadSettings();
    g_recheck = true;
    SetEvent(g_mouseWake);
    SetEvent(g_hideChanged);SetEvent(g_moved);
}

void WhTool_ModUninit() {
    Wh_Log(L"Uninit");
    StopThreads();
    SetUnhandledExceptionFilter(g_prevFilter);
}

////////////////////////////////////////////////////////////////////////////////
// Windhawk tool mod implementation for mods which don't need to inject to other
// processes or hook other functions. Context:
// https://github.com/ramensoftware/windhawk/wiki/Mods-as-tools:-Running-mods-in-a-dedicated-process
//
// The mod will load and run in a dedicated windhawk.exe process.
//
// Paste the code below as part of the mod code, and use these callbacks:
// * WhTool_ModInit
// * WhTool_ModSettingsChanged
// * WhTool_ModUninit
//
// Currently, other callbacks are not supported.

bool g_isToolModProcessLauncher;
HANDLE g_toolModProcessMutex;

static HANDLE g_ownedToolProcess = nullptr;
static DWORD g_ownedToolPid = 0;
static HANDLE g_toolStopEvent = nullptr;
static std::atomic<bool> g_toolCleanupStarted{false};

static DWORD WINAPI ToolStopWatcher(void*) {
    DWORD result = WaitForSingleObject(
        g_toolStopEvent, INFINITE
    );

    if (result == WAIT_OBJECT_0 &&
        !g_toolCleanupStarted.exchange(true)) {

        Wh_Log(L"Fluent Pointer: graceful shutdown");
        WhTool_ModUninit();
        ExitProcess(0);
    }

    return 0;
}

void WINAPI EntryPoint_Hook() {
    Wh_Log(L">");
    ExitThread(0);
}

// Native Windows caret integration. Runs on the owning UI thread; no input is injected.
namespace NativeCaretBridge {
static std::atomic<bool> stopping{false};
static SRWLOCK lock=SRWLOCK_INIT;
static std::vector<HWND> helpers;
static HINSTANCE module=nullptr;
static HANDLE mapping=nullptr;
static const NativeCaretSignal* signal=nullptr;
static wchar_t cls[96]=L"FluentPointer.NativeCaret.039";
static BOOL (WINAPI* originalShow)(HWND)=nullptr;
static BOOL (WINAPI* originalHide)(HWND)=nullptr;
static BOOL (WINAPI* originalCreate)(HWND,HBITMAP,int,int)=nullptr;
static BOOL (WINAPI* originalDestroy)()=nullptr;
static BOOL (WINAPI* originalPosition)(int,int)=nullptr;
struct State { HWND hidden=nullptr; bool inside=false; };
static thread_local HWND helper=nullptr;
static bool Wanted(){
 if(stopping)return false;
 AcquireSRWLockExclusive(&lock);
 if(!signal){mapping=OpenFileMappingW(FILE_MAP_READ,FALSE,L"Local\\FluentPointerNativeCaret");if(mapping)signal=(const NativeCaretSignal*)MapViewOfFile(mapping,FILE_MAP_READ,0,0,sizeof(NativeCaretSignal));}
 bool want=signal&&signal->active&&signal->pid==GetCurrentProcessId()&&DWORD(GetTickCount()-signal->tick)<500;
 ReleaseSRWLockExclusive(&lock);return want;
}
static void Restore(State* state){
 if(!state||!state->hidden)return;
 GUITHREADINFO gui{sizeof(gui)};
 if(GetGUIThreadInfo(GetCurrentThreadId(),&gui)&&gui.hwndCaret==state->hidden)originalShow(state->hidden);
 state->hidden=nullptr;
}
static void Sync(State* state){
 if(!state||state->inside)return;state->inside=true;
 GUITHREADINFO gui{sizeof(gui)};GetGUIThreadInfo(GetCurrentThreadId(),&gui);
 if(state->hidden!=gui.hwndCaret)state->hidden=nullptr;
 if(!Wanted())Restore(state);
 else if(!state->hidden&&gui.hwndCaret&&(gui.flags&GUI_CARETBLINKING)&&originalHide(gui.hwndCaret))state->hidden=gui.hwndCaret;
 state->inside=false;
}
static LRESULT CALLBACK Proc(HWND w,UINT msg,WPARAM wp,LPARAM lp){
 auto state=(State*)GetWindowLongPtrW(w,GWLP_USERDATA);
 if(msg==WM_NCCREATE){state=new State;SetWindowLongPtrW(w,GWLP_USERDATA,(LONG_PTR)state);return TRUE;}
 if(msg==WM_TIMER){if(stopping){Restore(state);DestroyWindow(w);}else Sync(state);return 0;}
 if(msg==WM_APP+77){Restore(state);DestroyWindow(w);return 0;}
 if(msg==WM_NCDESTROY){Restore(state);delete state;SetWindowLongPtrW(w,GWLP_USERDATA,0);helper=nullptr;AcquireSRWLockExclusive(&lock);helpers.erase(std::remove(helpers.begin(),helpers.end(),w),helpers.end());ReleaseSRWLockExclusive(&lock);}
 return DefWindowProcW(w,msg,wp,lp);
}
static State* Ensure(){
 if(stopping)return nullptr;
 if(!helper){
  // Keep cleanup callbacks valid even if an application's UI thread is hung at unload.
  HMODULE pinned=nullptr;GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS|GET_MODULE_HANDLE_EX_FLAG_PIN,(LPCWSTR)&Proc,&pinned);
  helper=CreateWindowExW(0,cls,L"",0,0,0,0,0,HWND_MESSAGE,nullptr,module,nullptr);
  if(helper){AcquireSRWLockExclusive(&lock);helpers.push_back(helper);ReleaseSRWLockExclusive(&lock);SetTimer(helper,1,30,nullptr);}
 }
 return helper?(State*)GetWindowLongPtrW(helper,GWLP_USERDATA):nullptr;
}
static BOOL WINAPI Show(HWND w){auto state=Ensure();Restore(state);BOOL ok=originalShow(w);Sync(state);return ok;}
static BOOL WINAPI Hide(HWND w){auto state=Ensure();Restore(state);BOOL ok=originalHide(w);Sync(state);return ok;}
static BOOL WINAPI Create(HWND w,HBITMAP b,int x,int y){auto state=Ensure();Restore(state);BOOL ok=originalCreate(w,b,x,y);Sync(state);return ok;}
static BOOL WINAPI Destroy(){auto state=Ensure();Restore(state);return originalDestroy();}
static BOOL WINAPI Position(int x,int y){auto state=Ensure();BOOL ok=originalPosition(x,y);Sync(state);return ok;}
static bool Init(){
 stopping.store(false);
 auto user=GetModuleHandleW(L"user32.dll");
 if(!user)return false;

 HMODULE self=nullptr;
 if(!GetModuleHandleExW(
     GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS|
     GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
     (LPCWSTR)&Proc,&self))return false;

 module=self;
 swprintf_s(cls,L"FluentPointer.NativeCaret.%p",(void*)self);

 WNDCLASSW wc{};
 wc.hInstance=module;
 wc.lpfnWndProc=Proc;
 wc.lpszClassName=cls;

 if(!RegisterClassW(&wc)){
     WNDCLASSW existing{};
     if(GetLastError()!=ERROR_CLASS_ALREADY_EXISTS||
        !GetClassInfoW(module,cls,&existing)||
        existing.lpfnWndProc!=Proc)
         return false;
 }
 struct Hook {const char* name;void* target;void** original;};
 Hook hooks[]={{"ShowCaret",(void*)&Show,(void**)&originalShow},{"HideCaret",(void*)&Hide,(void**)&originalHide},{"CreateCaret",(void*)&Create,(void**)&originalCreate},{"DestroyCaret",(void*)&Destroy,(void**)&originalDestroy},{"SetCaretPos",(void*)&Position,(void**)&originalPosition}};
 for(auto& h:hooks)if(!Wh_SetFunctionHook((void*)GetProcAddress(user,h.name),h.target,h.original))return false;
 return true;
}
static void Uninit(){
 stopping=true;std::vector<HWND> list;AcquireSRWLockShared(&lock);list=helpers;ReleaseSRWLockShared(&lock);
 for(HWND w:list){DWORD_PTR result=0;SendMessageTimeoutW(w,WM_APP+77,0,0,SMTO_ABORTIFHUNG|SMTO_BLOCK,300,&result);}
 // Mapping stays alive with pinned cleanup code until process exit.
}
}
static bool g_nativeCaretClient=false;

BOOL Wh_ModInit() {
    DWORD sessionId;
    if (ProcessIdToSessionId(GetCurrentProcessId(), &sessionId) &&
        sessionId == 0) {
        return FALSE;
    }

    wchar_t processPath[MAX_PATH]{};GetModuleFileNameW(nullptr,processPath,MAX_PATH);
    const wchar_t* processName=wcsrchr(processPath,L'\\');processName=processName?processName+1:processPath;
    if(_wcsicmp(processName,L"windhawk.exe")!=0){g_nativeCaretClient=true;return NativeCaretBridge::Init();}

    bool isExcluded = false;
    bool isToolModProcess = false;
    bool isCurrentToolModProcess = false;
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLine(), &argc);
    if (!argv) {
        Wh_Log(L"CommandLineToArgvW failed");
        return FALSE;
    }

    for (int i = 1; i < argc; i++) {
        if (wcscmp(argv[i], L"-service") == 0 ||
            wcscmp(argv[i], L"-service-start") == 0 ||
            wcscmp(argv[i], L"-service-stop") == 0) {
            isExcluded = true;
            break;
        }
    }

    for (int i = 1; i < argc - 1; i++) {
        if (wcscmp(argv[i], L"-tool-mod") == 0) {
            isToolModProcess = true;
            if (wcscmp(argv[i + 1], WH_MOD_ID) == 0) {
                isCurrentToolModProcess = true;
            }
            break;
        }
    }

    LocalFree(argv);

    if (isExcluded) {
        return FALSE;
    }

    if (isCurrentToolModProcess) {
        g_toolModProcessMutex =
            CreateMutex(nullptr, TRUE, L"windhawk-tool-mod_" WH_MOD_ID);
        if (!g_toolModProcessMutex) {
            Wh_Log(L"CreateMutex failed");
            ExitProcess(1);
        }

        if (GetLastError() == ERROR_ALREADY_EXISTS) {
            Wh_Log(L"Tool mod already running (%s)", WH_MOD_ID);
            ExitProcess(1);
        }

        wchar_t stopName[96]{};
        swprintf_s(
            stopName,
            L"Local\\FluentPointerToolStop_%lu",
            GetCurrentProcessId()
        );

        g_toolStopEvent = CreateEventW(
            nullptr, TRUE, FALSE, stopName
        );

        if (!g_toolStopEvent) {
            Wh_Log(L"Failed to create tool stop event");
            ExitProcess(1);
        }

        if (!WhTool_ModInit()) {
            CloseHandle(g_toolStopEvent);
            ExitProcess(1);
        }

        HANDLE watcher = CreateThread(
            nullptr, 0,
            ToolStopWatcher,
            nullptr, 0, nullptr
        );

        if (!watcher) {
            Wh_Log(L"Failed to start tool watcher");
            WhTool_ModUninit();
            ExitProcess(1);
        }

        CloseHandle(watcher);

        IMAGE_DOS_HEADER* dosHeader =
            (IMAGE_DOS_HEADER*)GetModuleHandle(nullptr);
        IMAGE_NT_HEADERS* ntHeaders =
            (IMAGE_NT_HEADERS*)((BYTE*)dosHeader + dosHeader->e_lfanew);

        DWORD entryPointRVA = ntHeaders->OptionalHeader.AddressOfEntryPoint;
        void* entryPoint = (BYTE*)dosHeader + entryPointRVA;

        Wh_SetFunctionHook(entryPoint, (void*)EntryPoint_Hook, nullptr);
        return TRUE;
    }

    if (isToolModProcess) {
        return FALSE;
    }

    g_isToolModProcessLauncher = true;
    return TRUE;
}

static HANDLE g_uiHostProcess=nullptr;
void Wh_ModAfterInit() {
    if(g_nativeCaretClient)return;
    if (!g_isToolModProcessLauncher) {
        return;
    }

    // A signed UIAccess renderer in Program Files can use band 2. Its pixel
    // renderer is identical; ordinary Windhawk remains the fallback.
    WCHAR programFiles[MAX_PATH]{},hostPath[MAX_PATH]{};
    if(GetEnvironmentVariableW(L"ProgramFiles",programFiles,ARRAYSIZE(programFiles))){
        swprintf_s(hostPath,L"%s\\Fluent Pointer\\fluent-pointer-host.exe",programFiles);
        if(GetFileAttributesW(hostPath)!=INVALID_FILE_ATTRIBUTES){
            SHELLEXECUTEINFOW run{sizeof(run)};run.fMask=SEE_MASK_NOCLOSEPROCESS|SEE_MASK_FLAG_NO_UI;run.lpFile=hostPath;run.nShow=SW_HIDE;
            if(ShellExecuteExW(&run)&&run.hProcess){
                if(WaitForSingleObject(run.hProcess,1500)==WAIT_TIMEOUT){g_uiHostProcess=run.hProcess;Wh_SetStringValue(L"pointerHostStatus",L"Signed UIAccess renderer running");return;}
                CloseHandle(run.hProcess);
            }
            Wh_SetStringValue(L"pointerHostStatus",L"UIAccess renderer unavailable; ordinary overlay retained");
        }
    }
    WCHAR currentProcessPath[MAX_PATH];
    switch (GetModuleFileName(nullptr, currentProcessPath,
                              ARRAYSIZE(currentProcessPath))) {
        case 0:
        case ARRAYSIZE(currentProcessPath):
            Wh_Log(L"GetModuleFileName failed");
            return;
    }

    WCHAR
    commandLine[MAX_PATH + 2 +
                (sizeof(L" -tool-mod \"" WH_MOD_ID "\"") / sizeof(WCHAR)) - 1];
    swprintf_s(commandLine, L"\"%s\" -tool-mod \"%s\"", currentProcessPath,
               WH_MOD_ID);

    HMODULE kernelModule = GetModuleHandle(L"kernelbase.dll");
    if (!kernelModule) {
        kernelModule = GetModuleHandle(L"kernel32.dll");
        if (!kernelModule) {
            Wh_Log(L"No kernelbase.dll/kernel32.dll");
            return;
        }
    }

    using CreateProcessInternalW_t = BOOL(WINAPI*)(
        HANDLE hUserToken, LPCWSTR lpApplicationName, LPWSTR lpCommandLine,
        LPSECURITY_ATTRIBUTES lpProcessAttributes,
        LPSECURITY_ATTRIBUTES lpThreadAttributes, WINBOOL bInheritHandles,
        DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory,
        LPSTARTUPINFOW lpStartupInfo,
        LPPROCESS_INFORMATION lpProcessInformation,
        PHANDLE hRestrictedUserToken);
    CreateProcessInternalW_t pCreateProcessInternalW =
        (CreateProcessInternalW_t)GetProcAddress(kernelModule,
                                                 "CreateProcessInternalW");
    if (!pCreateProcessInternalW) {
        Wh_Log(L"No CreateProcessInternalW");
        return;
    }

    STARTUPINFO si{
        .cb = sizeof(STARTUPINFO),
        .dwFlags = STARTF_FORCEOFFFEEDBACK,
    };
    PROCESS_INFORMATION pi;
    if (!pCreateProcessInternalW(nullptr, currentProcessPath, commandLine,
                                 nullptr, nullptr, FALSE, NORMAL_PRIORITY_CLASS,
                                 nullptr, nullptr, &si, &pi, nullptr)) {
        Wh_Log(L"CreateProcess failed");
        return;
    }

    g_ownedToolProcess = pi.hProcess;
    g_ownedToolPid = pi.dwProcessId;
    CloseHandle(pi.hThread);

    Wh_Log(
        L"Fluent Pointer tool started: PID %lu",
        g_ownedToolPid
    );
}

void Wh_ModSettingsChanged() {
    if(g_nativeCaretClient)return;
    if (g_isToolModProcessLauncher) {
        return;
    }

    WhTool_ModSettingsChanged();
}

void Wh_ModUninit() {
    if(g_nativeCaretClient){NativeCaretBridge::Uninit();return;}
    if (g_isToolModProcessLauncher) {
        if(g_uiHostProcess){
            HANDLE stop=OpenEventW(EVENT_MODIFY_STATE,FALSE,L"Local\\FluentPointerHostStop");
            if(stop){SetEvent(stop);CloseHandle(stop);WaitForSingleObject(g_uiHostProcess,5000);}
            CloseHandle(g_uiHostProcess);g_uiHostProcess=nullptr;
        }

        if (g_ownedToolProcess) {
            if (WaitForSingleObject(
                    g_ownedToolProcess, 0
                ) == WAIT_TIMEOUT) {

                wchar_t stopName[96]{};
                swprintf_s(
                    stopName,
                    L"Local\\FluentPointerToolStop_%lu",
                    g_ownedToolPid
                );

                HANDLE stop = nullptr;

                for (int attempt = 0; attempt < 40; ++attempt) {
                    stop = OpenEventW(
                        EVENT_MODIFY_STATE,
                        FALSE,
                        stopName
                    );

                    if (stop) break;

                    if (WaitForSingleObject(
                            g_ownedToolProcess, 0
                        ) != WAIT_TIMEOUT) break;

                    Sleep(50);
                }

                if (stop) {
                    SetEvent(stop);
                    CloseHandle(stop);
                } else {
                    Wh_Log(L"Tool stop event unavailable");
                }

                DWORD result = WaitForSingleObject(
                    g_ownedToolProcess, 5000
                );

                if (result != WAIT_OBJECT_0) {
                    Wh_Log(
                        L"Tool shutdown incomplete: %lu",
                        result
                    );
                }
            }

            CloseHandle(g_ownedToolProcess);
            g_ownedToolProcess = nullptr;
            g_ownedToolPid = 0;
        }

        return;
    }

    if (g_toolCleanupStarted.exchange(true))
        return;

    WhTool_ModUninit();
    ExitProcess(0);
}
