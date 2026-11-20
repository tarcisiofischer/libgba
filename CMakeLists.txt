cmake_minimum_required(VERSION 3.16)
project(libgba)

enable_language(ASM)

# Mirror public include structure in build directory
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/include/libgba")
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_CURRENT_SOURCE_DIR}/base/gba.h" "${CMAKE_CURRENT_BINARY_DIR}/include/libgba/gba.h")
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_CURRENT_SOURCE_DIR}/base/types.h" "${CMAKE_CURRENT_BINARY_DIR}/include/libgba/types.h")
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_CURRENT_SOURCE_DIR}/base/random.h" "${CMAKE_CURRENT_BINARY_DIR}/include/libgba/random.h")

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
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(
  libgba-base
  libgba-rt
  libgba-libc
)

# Optional text rendering library
option(LIBGBA_BUILD_TXT "Build optional text rendering library (libgba-txt)" ON)

if (LIBGBA_BUILD_TXT)
  execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_CURRENT_SOURCE_DIR}/libtxt/txt.h" "${CMAKE_CURRENT_BINARY_DIR}/include/libgba/txt.h")

  set(TXT_FONT_PNG "${CMAKE_CURRENT_SOURCE_DIR}/libtxt/monogram-bitmap.png")
  set(TXT_FONT_INC "${CMAKE_CURRENT_BINARY_DIR}/font_tileset.inc")
  set(TXT_FONT_TOOL "${CMAKE_CURRENT_SOURCE_DIR}/tools/font2tiles")

  add_custom_command(
    OUTPUT "${TXT_FONT_INC}"
    COMMAND python3 "${TXT_FONT_TOOL}" "${TXT_FONT_PNG}" "${TXT_FONT_INC}"
    DEPENDS "${TXT_FONT_PNG}" "${TXT_FONT_TOOL}"
    COMMENT "Converting monogram font to tileset..."
    VERBATIM
  )
  add_custom_target(libgba_txt_font DEPENDS "${TXT_FONT_INC}")

  add_library(
    libgba-txt
    STATIC
    libtxt/txt.cc
    libtxt/txt.h
  )
  add_dependencies(libgba-txt libgba_txt_font)
  set_target_properties(libgba-txt PROPERTIES LINKER_LANGUAGE CXX)
  target_compile_options(
    libgba-txt PRIVATE
    -mthumb
    -mfpu=none
    -fno-rtti
    -nostdlib
  )
  target_include_directories(
    libgba-txt
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
  )
  target_link_libraries(
    libgba-txt
    PUBLIC
    libgba-base
  )
endif()

set(LIBGBA_INSTALL_TARGETS libgba-rt libgba-libc libgba-base)
if (LIBGBA_BUILD_TXT)
  list(APPEND LIBGBA_INSTALL_TARGETS libgba-txt)
endif()

# Install libraries
install(TARGETS ${LIBGBA_INSTALL_TARGETS}
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

if (LIBGBA_BUILD_TXT)
  install(FILES
    libtxt/txt.h
    DESTINATION include/libgba
  )
endif()

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
