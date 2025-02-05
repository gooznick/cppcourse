# toolchain.cmake - Example CMake Toolchain File

# Target system
set(CMAKE_SYSTEM_NAME Linux)  # Could be Windows, Generic, etc.
set(CMAKE_SYSTEM_PROCESSOR arm)  # Change for architecture (x86_64, arm, etc.)

# Specify cross compiler (adjust paths as needed)
set(CMAKE_C_COMPILER /usr/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/arm-linux-gnueabihf-g++)

# Set sysroot if using a custom one
set(CMAKE_SYSROOT /opt/sysroot)

# Adjust flags
set(CMAKE_C_FLAGS "--sysroot=${CMAKE_SYSROOT} -O2")
set(CMAKE_CXX_FLAGS "--sysroot=${CMAKE_SYSROOT} -O2")

# Specify linker
set(CMAKE_LINKER /usr/bin/arm-linux-gnueabihf-ld)
set(CMAKE_EXE_LINKER_FLAGS "--sysroot=${CMAKE_SYSROOT}")

# Set paths for libraries and includes
set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER) # Don't search host programs
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)  # Search only in sysroot
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

