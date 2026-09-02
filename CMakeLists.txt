cmake_minimum_required(VERSION 3.16)
project(libgba)

enable_language(ASM)

add_library(
  libgba-rt
  STATIC
  rt/gba_crt0.s
)
set_target_properties(libgba-rt PROPERTIES LINKER_LANGUAGE CXX)

add_library(
  libgba-libc
  STATIC
  libc/gba_libc.c
)

add_library(
  libgba-base
  STATIC
  base/gba.h
  base/types.h
  base/random.h
)

set_target_properties(libgba-base PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(
  libgba-base
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(
  libgba-base
  libgba-rt
  libgba-libc
)

# Install libraries
install(TARGETS libgba-rt libgba-libc libgba-base
  EXPORT libgbaTargets
  ARCHIVE DESTINATION lib
)

# Install headers
install(FILES
  base/gba.h
  base/types.h
  base/random.h
  DESTINATION include/libgba
)

# Install linker script
install(FILES rt/gba_cart.ld DESTINATION lib/libgba/rt)

# Install cmake helpers
install(FILES
  cmake/GBAExecutable.cmake
  cmake/gba-toolchain.cmake
  DESTINATION lib/cmake/libgba
)

# Export targets
install(EXPORT libgbaTargets
  FILE libgbaTargets.cmake
  NAMESPACE libgba::
  DESTINATION lib/cmake/libgba
)

# Generate and install config file
include(CMakePackageConfigHelpers)
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/libgbaConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/libgbaConfig.cmake
  INSTALL_DESTINATION lib/cmake/libgba
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libgbaConfig.cmake
  DESTINATION lib/cmake/libgba
)
