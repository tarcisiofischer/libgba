add_library(
  libgba-rt
  STATIC
  rt/gba_crt0.s
)

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
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/base
)

target_link_libraries(
  libgba-base
  libgba-rt
  libgba-libc
)

