#!/bin/bash -e

apt-get install -y curl gpg

curl -fsSL 'https://dl.cloudsmith.io/public/moonlight-game-streaming/moonlight-qt/setup.deb.sh' \
    | bash
