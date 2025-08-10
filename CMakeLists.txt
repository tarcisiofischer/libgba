add_library(
  crt0-gba
  STATIC
  gba_crt0.s
  noop_libc.c
)

