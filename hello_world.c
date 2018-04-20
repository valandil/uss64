#define F3D_GBI

#include <stddef.h>
#include <n64/thread.h>
#include <n64/message.h>
#include <n64/pi.h>
#include <startup.h>
#include "gz/src/gz/z64.h"
#include "gz/src/gz/resource.h"
#include "gz/src/gz/gfx.h"

//#include "gbi.h"

void (*PrintXY)(unsigned int x, unsigned int y, const char *str) = (void*)0x802D66C0;
void* (*alloc_displaylist)(unsigned int size) = (void*)0x8019CF44;
static const char HelloString[] = "hello n64";
static unsigned int x = 32;
static unsigned int y = 32;
static _Bool __attribute((section(".data")))         _ready = 0;

// Display variables.
#define GFX_DISP_SIZE 1000
static struct gfx_font *font;
static        void     *dlist;

// Try to write Hello World using native GFx.
ENTRY void _start(void)
{
  init_gp();

  if (!_ready)
  {
    clear_bss();
    do_global_ctors();
    // initialize gfx /
    {
        gfx_start();
        gfx_mode_configure(GFX_MODE_FILTER, G_TF_POINT);
        gfx_mode_configure(GFX_MODE_COMBINE, G_CC_MODE(G_CC_MODULATEIA_PRIM,
                                                     G_CC_MODULATEIA_PRIM));
    }
    // Initialize our custom dlist.
    {
      //dlist = alloc_displaylist(GFX_DISP_SIZE);
    }


    font = resource_get(RES_FONT_FIPPS);
    _ready = 1;
  }

  gfx_mode_init();
  //gfx_printf(font, x, y, HelloString);
  //gfx_flush();

  // Lives back to normal position
  //PrintXY(0x36, 0xD1, (void*)0x80338388, *(int16_t*)0x8033B260);

  // Print hello world
  PrintXY(x, y, HelloString);
}
