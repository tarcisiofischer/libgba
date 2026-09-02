# GBA cross-compilation toolchain for LLVM/Clang
# Usage: cmake -DCMAKE_TOOLCHAIN_FILE=cmake/gba-toolchain.cmake ...
# Requires GBA_LLVM environment variable to point to the LLVM build directory.

if(NOT DEFINED ENV{GBA_LLVM})
  message(FATAL_ERROR "GBA_LLVM environment variable is not set. "
    "Point it to your LLVM build directory (e.g. /path/to/gba-llvm/build).")
endif()

set(GBA_LLVM_BIN "$ENV{GBA_LLVM}/bin")

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR armv4t)

set(CMAKE_ASM_COMPILER "${GBA_LLVM_BIN}/clang")
set(CMAKE_C_COMPILER   "${GBA_LLVM_BIN}/clang")
set(CMAKE_CXX_COMPILER "${GBA_LLVM_BIN}/clang++")
set(CMAKE_AR           "${GBA_LLVM_BIN}/llvm-ar")
set(CMAKE_RANLIB       "${GBA_LLVM_BIN}/llvm-ranlib")

# Target triple for the GBA (ARM7TDMI = armv4t, no OS)
set(CMAKE_ASM_COMPILER_TARGET "armv4t-none-unknown-eabi")
set(CMAKE_C_COMPILER_TARGET   "armv4t-none-unknown-eabi")
set(CMAKE_CXX_COMPILER_TARGET "armv4t-none-unknown-eabi")

# Don't try to link or run test executables (cross-compiling)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
