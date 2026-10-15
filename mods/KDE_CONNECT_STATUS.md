# KDE Connect Integration — requirements and source files

**Tested by the author and working on their Windows setup**, including the KDE Connect Integration taskbar panel and the companion Battery Module.

## Required: KDE Connect

**KDE Connect is required.** Install and run [KDE Connect for Windows](https://kdeconnect.kde.org/) on the PC, install KDE Connect on your phone and pair the two devices. Windhawk and the companion module **do not replace** KDE Connect itself.

These Windhawk mods enhance the existing KDE Connect connection; they do not use ADB or install a replacement KDE Connect service.

## Modules

- [KDE Connect Integration v0.6.0](kde-connect-integration.wh.cpp) — the taskbar button, notifications and phone information inside `explorer.exe`.
- [KDE Connect Battery Module v0.1.0](kde-connect-battery-module-windows.wh.cpp) — receives battery packets inside KDE Connect's `kdeconnectd.exe`, for display by the taskbar integration.

For battery reporting, enable **both Windhawk mods** alongside KDE Connect, then reconnect the phone if necessary to refresh KDE Connect capabilities.

The adapter was designed for the author's **Qt 6.11.1 x64** KDE Connect runtime. It deliberately refuses other detected Qt versions; support for other versions is not established here.

## Support headers not yet uploaded

Both mods **work on the author's installation**. However, these source files reference headers that are **not yet committed to this repository**. Anyone compiling from these standalone `.wh.cpp` files will also need the original corresponding `.hpp` sources:

- `kde-integration-json.hpp` — JSON parser used by the battery adapter
- `kde-integration-devices.hpp` — device selection and name
- `kde-integration-dbus.hpp` — KDE connection and notifications
- `kde-integration-panel.hpp` — phone popup
- `kde-integration-taskbar.hpp` — taskbar integration
- `kde-integration-settings-panel.hpp` — settings panel

This is a **repository completeness note**, not a claim that the author's working installation is broken. Independent compilation from the repository is pending those files.

## Screenshots

The README currently displays the author's existing GitHub-hosted screenshots. They can be moved into `screenshots/` after the original image files are provided, with the README links updated at the same time.
