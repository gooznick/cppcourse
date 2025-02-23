set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Set cross-compiler
set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)

# Set sysroot if needed (optional)
set(CMAKE_SYSROOT /usr/bin/aarch64-linux-gnu)

# Set find root path mode (prevents using host libraries)
set(CMAKE_FIND_ROOT_PATH /usr/bin/aarch64-linux-gnu)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# cmake -B build-aarch64 -DCMAKE_TOOLCHAIN_FILE=aarch64-toolchain.cmake
# cd build-aarch64/
# make
