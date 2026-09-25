# EyeTrackVR Carrier Firmware
Firmware repository for the EyeTrackVR Carrier Board for Valve Index.

Use the development container for development and building the firmware image.

# Getting started:

### Prerequisite
* If you plan to contribute, ensure you have [Git setup](https://docs.github.com/en/get-started/git-basics/set-up-git#setting-up-git) with your username and email address.
* VSCode and Docker installed.

1. Clone the repository
2. Open repository in VSCode and connect to the dev container. It will take some time to build the container.
3. Add MCU specific dependencies to TinyUSB `python3 ./tinyusb/tools/get_deps.py samd11`
4. Build the firmware binary by running the VSCode build task "Firmware Build Solution".