void __libc_init_array() {
  // Intentionally left blank.
}

void* memcpy(void* dest, const void* src, unsigned int count) {
  void* s = dest;
  while (count--) {
    *((char*)dest) = *((const char*)src);
    src++;
    dest++;
  }
  return s;
}

void* memset(void* dest, int ch, unsigned int count) {
  unsigned char* s = dest;
  while (count--) {
    *s = ch;
    ++s;
  }
  return dest;
}

