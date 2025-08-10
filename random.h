#ifndef __LIBGBA_RANDOM
#define __LIBGBA_RANDOM

u16_t rand(u16_t seed)
{
  seed = 0xa9f*seed + 0x1edd;
  seed = (seed << 8) ^ (seed >> 8);
  return seed;
}

#endif

