rm -rf ./build
cmake -S . -B ./build -G Ninja
ninja -C ./build
