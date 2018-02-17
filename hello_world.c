#include <stddef.h>
#include <n64.h>
#include <startup.h>
#include "gz/src/gz/z64.h"
#include "gz/src/gz/resource.h"
#include "gz/src/gz/gfx.h"

extern void PrintXY(unsigned int x, unsigned int y, const char *str);
static const char HelloString[] = "- hello n64 -";
static unsigned int x = 32;
static unsigned int y = 32;
static _Bool         _ready = 0;

static struct gfx_font *font;

// Try to write Hello World using native GFx.
ENTRY void _start(void)
{
  if (!_ready)
  {
    /* initialize gfx */
    {
      gfx_start();
      gfx_mode_configure(GFX_MODE_FILTER, G_TF_POINT);
      gfx_mode_configure(GFX_MODE_COMBINE, G_CC_MODE(G_CC_MODULATEIA_PRIM,
                                                     G_CC_MODULATEIA_PRIM));
    }

    font = resource_get(RES_FONT_FIPPS);
    _ready = 1;
  }

  gfx_mode_init();
  gfx_printf(font, x, y, HelloString);


  // Lives back to normal position
  //PrintXY(0x36, 0xD1, (void*)0x80338388, *(int16_t*)0x8033B260);

  // Print hello world
  //PrintXY(x, y, HelloString);
}