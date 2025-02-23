sudo apt install -y g++-aarch64-linux-gnu gcc-aarch64-linux-gnu qemu qemu-user

aarch64-linux-gnu-g++ -o simple simple.cpp

aarch64-linux-gnu-readelf -d simple | grep NEEDED

qemu-aarch64 ./simple
sudo apt install -y qemu-user-static libc6-arm64-cross libc6-armhf-cross

aarch64-linux-gnu-g++ -o simple simple.cpp -static
qemu-aarch64 ./simple