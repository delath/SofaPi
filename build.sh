#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

if [ ! -d "pi-gen" ]; then
    echo "Cloning pi-gen..."
    git clone --depth 1 https://github.com/RPi-Distro/pi-gen.git
fi

cd pi-gen

cp "$SCRIPT_DIR/config" .

rm -rf stage3 stage4 stage5

mkdir -p stage3
cp -r "$SCRIPT_DIR/SofaPi-Stage/"* stage3/
find stage3 -name "*.sh" -exec chmod +x {} +

touch stage2/SKIP_IMAGES
touch stage3/EXPORT_IMAGE

echo ""
echo "Building SofaPi image..."
echo ""

sudo ./build.sh
