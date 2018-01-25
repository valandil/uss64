#include <stddef.h>

extern void PrintXY(unsigned int x, unsigned int y, const char *str);
static const char HelloString[] = "- hello n64 -";
static unsigned int x = 32;
static unsigned int y = 32;

void MainHook(void)
{

  // Lives back to normal position
  //PrintXY(0x36, 0xD1, (void*)0x80338388, *(int16_t*)0x8033B260);

  // Print hello world
  PrintXY(x, y, HelloString);
}