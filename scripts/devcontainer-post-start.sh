#!/usr/bin/env bash

git config --global --add safe.directory /workspaces/etvr-carrier-firmware
git submodule update --init --recursive

# Add MCU dependencies if not installed. Prevents from installing everytime a container is rebuilt.
if [[ ! -d "tinyusb/hw/mcu/microchip" ]]; then
    python3 ./tinyusb/tools/get_deps.py samd11
fi
