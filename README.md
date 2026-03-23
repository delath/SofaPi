# SofaPi

A lightweight, single-purpose OS for the Raspberry Pi 4 (2 GB+).  
Boots straight into a two-button launcher for **Moonlight** and **Jellyfin** — no desktop environment, no wasted RAM.

## What You Get

| Layer | Component |
|-------|-----------|
| Base OS | Raspberry Pi OS Lite (Bookworm 64-bit) |
| Display | `cage` — Wayland kiosk compositor |
| Launcher | Tiny C/GTK3 binary (~50 KB) |
| Streaming | `moonlight-qt` (Sunshine / GameStream) |
| Media | Chromium in kiosk mode → Jellyfin web UI |
| Controller | `antimicrox` maps gamepad → keyboard |

## Installation

### Flash a Pre-Built Image
1. Download the latest `.img.xz` from [Releases](https://github.com/delath/SofaPi/releases).
2. Flash it with [Raspberry Pi Imager](https://www.raspberrypi.com/software/) or [BalenaEtcher](https://etcher.balena.io/).
3. Boot. The launcher appears automatically.

> **Default login** (for SSH / TTY): user `pi`, password `raspberry`.  
> Change it with `passwd` if you enable SSH.

### First-Time Jellyfin Setup
The first time you click **Jellyfin**, a simple page asks for your server address (e.g. `http://192.168.1.100:8096`).  
Enter it once — it's saved in the browser and you won't be asked again.  
To change it later, open `http://localhost/jellyfin-redirect.html?setup` from within the Jellyfin browser,  
or press `Ctrl+Alt+F2` for a TTY (user `pi` / password `raspberry`) and delete the Chromium local storage:
```
rm -rf /home/pi/.config/chromium/Default/Local\ Storage
```
Next time you click **Jellyfin**, the setup page will appear again.

## Controls

| Input | Navigate | Select | Exit App |
|-------|----------|--------|----------|
| Keyboard | `Tab` | `Enter` | `Alt+F4` |
| Gamepad | D-Pad | A / Cross | Guide / PS button |

> Gamepad mapping is handled by `antimicrox`. You can customize it in  
> `/home/pi/.config/antimicrox/`.

## Building the Image Yourself

Requires a Debian/Ubuntu machine (or WSL / Docker on Windows).

```bash
git clone https://github.com/delath/SofaPi.git
cd SofaPi
chmod +x build.sh
./build.sh
```

The script clones [pi-gen](https://github.com/RPi-Distro/pi-gen), injects the SofaPi stage, and builds an image.  
Output lands in `pi-gen/deploy/`.

## Overclocking

The image ships with a stable Pi 4 overclock (**2.0 GHz CPU, 750 MHz GPU**).  
**Active cooling is required.** If unstable, edit `/boot/firmware/config.txt` and lower `arm_freq` to `1900`.

## Project Structure

```
SofaPi/
├── config                              # pi-gen build config
├── build.sh                            # One-command image builder
├── LICENSE
├── README.md
└── SofaPi-Stage/
    ├── 00-add-repos/
    │   └── 00-run-chroot.sh            # Adds moonlight-qt APT repository
    └── 01-install-sofapi/
        ├── 00-packages                 # apt packages to install
        ├── 01-run.sh                   # Compiles launcher, sets up systemd
        └── files/
            ├── sofapi.c                # The GTK3 launcher source
            ├── jellyfin-redirect.html  # First-run Jellyfin URL setup page
            └── antimicrox.amgp         # Gamepad-to-keyboard mapping profile
```

## License

[MIT](LICENSE)
