#include <startup.h>
#include <stdint.h>

ENTRY void _start()
{
  void (*sm64_printf)(int x, int y, const char *format, ...) = (void*)0x802d66c0;

  // Lives back to normal position
  sm64_printf(0x36, 0xD1, (void*)0x80338388, *(int16_t*)0x8033B260);

  // Print hello world
  sm64_printf(80, 80, "hello world");
}