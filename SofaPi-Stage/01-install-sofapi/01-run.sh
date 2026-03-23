#!/bin/bash -e

install -m 644 files/sofapi.c "${ROOTFS_DIR}/home/pi/sofapi.c"
install -m 644 files/jellyfin-redirect.html "${ROOTFS_DIR}/home/pi/jellyfin-redirect.html"
mkdir -p "${ROOTFS_DIR}/home/pi/.config/antimicrox"
install -m 644 files/antimicrox.amgp "${ROOTFS_DIR}/home/pi/.config/antimicrox/sofapi.amgp"

# --- Compile the GTK launcher inside the image ---
on_chroot << 'EOF'
cd /home/pi
gcc $(pkg-config --cflags gtk+-3.0) -o sofapi sofapi.c $(pkg-config --libs gtk+-3.0)
chown pi:pi sofapi sofapi.c jellyfin-redirect.html
chown -R pi:pi /home/pi/.config
EOF

cat >> "${ROOTFS_DIR}/boot/firmware/config.txt" << OCEOF
# SofaPi Overclock
over_voltage=6
arm_freq=2000
gpu_freq=750
OCEOF

cat > "${ROOTFS_DIR}/etc/systemd/system/sofapi.service" << 'EOF'
[Unit]
Description=SofaPi Kiosk Launcher
After=systemd-user-sessions.service plymouth-quit-wait.service
Conflicts=getty@tty1.service

[Service]
Environment=WLR_LIBINPUT_NO_DEVICES=1
ExecStartPre=-/usr/bin/antimicrox --hidden --profile /home/pi/.config/antimicrox/sofapi.amgp
ExecStart=/usr/bin/cage -s -- /home/pi/sofapi
Type=simple
User=pi
Restart=always
StandardInput=tty
TTYPath=/dev/tty1

[Install]
WantedBy=multi-user.target
EOF

on_chroot << 'EOF'
systemctl disable getty@tty1.service
systemctl enable sofapi.service
EOF
