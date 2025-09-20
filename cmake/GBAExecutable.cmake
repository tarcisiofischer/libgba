function(gba_exec)
  set(oneValueArgs NAME TILEMAP)
  set(multiValueArgs SOURCES SPRITESHEETS)
  cmake_parse_arguments(arg "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  add_executable(
    ${arg_NAME}
    ${arg_SOURCES}
  )
  target_compile_options(
    ${arg_NAME} PRIVATE
    -mthumb
    -mfpu=none
    -fno-rtti
    -nostdlib
    -I${CMAKE_SOURCE_DIR}/libgba/rt/
    -I${CMAKE_SOURCE_DIR}/libgba/base/
  )
  target_link_options(
    ${arg_NAME} PRIVATE
    -nostdlib
    -L${CMAKE_BINARY_DIR}/libgba
    -llibgba-rt
    -llibgba-libc
    -Wl,-T,${CMAKE_SOURCE_DIR}/libgba/rt/gba_cart.ld
    -L$ENV{GBA_LLVM}/lib/clang/21/lib/armv4t-none-unknown-eabi/
    -lclang_rt.builtins
  )

  set(RESOLVED_TARGET_FILE $<TARGET_FILE:${arg_NAME}>)
  set(ASSETS_OUTPUT_DIR "${CMAKE_BINARY_DIR}/${arg_NAME}_assets/")
  add_custom_command(
    OUTPUT "${ASSETS_OUTPUT_DIR}"
    COMMAND mkdir -p "${ASSETS_OUTPUT_DIR}"
    COMMENT "Generating assets output dir ${SS_ASSETS_DIR}"
    VERBATIM
  )
  add_custom_target(${arg_NAME}_assets_outdir DEPENDS "${ASSETS_OUTPUT_DIR}")
  add_dependencies(${arg_NAME} ${arg_NAME}_assets_outdir)
  target_include_directories(${arg_NAME} PRIVATE ${ASSETS_OUTPUT_DIR})

  foreach (SS ${arg_SPRITESHEETS})
    get_filename_component(SS_TARGET "${SS}" NAME_WE)
    set(SS_TARGET "spritesheet_${SS_TARGET}")
    set(SS_OUTPUT "${ASSETS_OUTPUT_DIR}/${SS_TARGET}.inc")
    add_custom_command(
      OUTPUT "${SS_OUTPUT}"
      COMMAND img2bytes "${CMAKE_CURRENT_SOURCE_DIR}/${SS}" "${SS_OUTPUT}"
      DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${SS}"
      COMMENT "Generating spritesheet ${SS_OUTPUT}"
      VERBATIM
    )
    add_custom_target(${arg_NAME}_${SS_TARGET} DEPENDS "${SS_OUTPUT}")
    add_dependencies(${arg_NAME} ${arg_NAME}_${SS_TARGET})
    add_dependencies(${arg_NAME}_${SS_TARGET} ${arg_NAME}_assets_outdir)
  endforeach()

  if (DEFINED arg_TILEMAP)
    set(TILEMAP_TARGET "tilemap_${SS_TARGET}")
    set(TILEMAP_OUTPUT "${ASSETS_OUTPUT_DIR}/tilemap.inc")
    add_custom_command(
      OUTPUT "${TILEMAP_OUTPUT}"
      COMMAND prepare-tilemaps "${CMAKE_CURRENT_SOURCE_DIR}/${arg_TILEMAP}" "${TILEMAP_OUTPUT}"
      DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${arg_TILEMAP}"
      COMMENT "Generating tilemaps"
      VERBATIM
    )
    add_custom_target(${TILEMAP_TARGET} DEPENDS "${TILEMAP_OUTPUT}")
    add_dependencies(${arg_NAME} ${TILEMAP_TARGET})
    add_dependencies(${TILEMAP_TARGET} ${arg_NAME}_assets_outdir)
  endif()

  add_custom_command(
    TARGET ${arg_NAME}
    POST_BUILD
    COMMAND $(GBA_LLVM)/bin/llvm-objcopy -O binary ${RESOLVED_TARGET_FILE} ${arg_NAME}.gba
    COMMENT "Building GBA ROM for ${arg_NAME}..."
    VERBATIM
  )
endfunction()
