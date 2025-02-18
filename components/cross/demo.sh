sudo apt install -y gcc-aarch64-linux-gnu qemu qemu-user

aarch64-linux-gnu-gcc -o hello-arm demo.c
./hello-arm 
qemu-aarch64 ./hello-arm
sudo apt install -y qemu-user-static libc6-arm64-cross libc6-armhf-cross
aarch64-linux-gnu-gcc -static -o hello-arm-static demo.c 
qemu-aarch64 ./hello-arm-static