#include <stddef.h>
#include <n64/thread.h>
#include <n64/message.h>
#include <n64/pi.h>
#include <startup.h>
#include "gz/src/gz/z64.h"
#include "gz/src/gz/resource.h"
#include "gz/src/gz/gfx.h"

#include "sm64.h"
#include "input.h"

static _Bool __attribute((section(".data")))         uss64_ready = 0;
static const char HelloString[] = "hello n64";
static unsigned int x = 64;
static unsigned int y = 32;

// Display variables.
#define GFX_DISP_SIZE 1000
static struct gfx_font *font;

// gfx_printf variables
static const char USS64String[] = "hello from uss64";
static       int  master_dlist_addr;
static       Gfx* master_dlist_ptr;
static const uint32_t input_button_color[] =
{
  0xFFA000,
  0xFFA000,
  0xFFA000,
  0xFFA000,
  0xC0C0C0,
  0xC0C0C0,
  0x000000,
  0x000000,
  0xC8C8C8,
  0xC8C8C8,
  0xC8C8C8,
  0xC8C8C8,
  0xC80000,
  0xC0C0C0,
  0x009600,
  0x5A5AFF,
};

HOOK static void display_hook(void)
{

  // Call the function we overwrote.
  uint32_t addr = funcCalledAtCleanUpDisplayListHook;
  __asm__ volatile ("addiu  $sp, $sp, -0x18   \n"
                    "sw     $ra, 0x14($sp)    \n"
                    "jal    %0                \n"
                    "lw     $ra, 0x14($sp)    \n"
                    "addiu  $sp, $sp, 0x18    \n"
                    :: "r"(addr));

  if (uss64_ready)
  {
    // Branch off the tail of the master DL.
    //PrintXY(x,y, HelloString);
    
    // Try to manually write the DL
    gDPSetFillColor(SM64_gDisplayListHead++, GPACK_RGBA5551(255,0,0,1) << 16 | GPACK_RGBA5551(255,0,0,1));
    gDPFillRectangle(SM64_gDisplayListHead++, 0, 0, 10, 10);
    gfx_flush();

  }
}

HOOK static void main_hook(void)
{
  // Initialize gfx_* for this frame.
  gfx_mode_init();

  // Input display.
  float alpha = 0.2;
  struct gfx_texture *button_texture = resource_get(RES_ICON_BUTTONS);
  gfx_mode_set(GFX_MODE_COLOR, GPACK_RGBA8888(0xC0,0xC0,0xC0, alpha));
  static const int buttons[] =
  {
    15, 14, 12, 3, 2, 1, 0, 13, 5, 4, 11, 10, 9, 8,
  };
  uint16_t z_pad = SM64_gPlayer1Controller->buttonDown;
  for (int i = 0; i < sizeof(buttons) / sizeof(*buttons); ++i) {
    int b = buttons[i];
    if (!(z_pad & (1 << b)))
      continue;
    int x = (button_texture->tile_width) / 2 + i * 10;
    int y = -(gfx_font_xheight(font) + button_texture->tile_height + 1) / 2;
    struct gfx_sprite sprite =
    {
      button_texture, b,
      20 + x, 20 + y,
      1.f, 1.f,
    };
    gfx_mode_set(GFX_MODE_COLOR, GPACK_RGB24A8(input_button_color[b],
                                              alpha));
    gfx_sprite_draw(&sprite);
  }
}

HOOK static void init(void)
{
  // Initialize the static variables (I think).
  clear_bss();
  do_global_ctors();

  // Initialize gfx.
  {
      gfx_start();
      gfx_mode_configure(GFX_MODE_FILTER, G_TF_POINT);
      gfx_mode_configure(GFX_MODE_COMBINE, G_CC_MODE(G_CC_TEXEL0ONLY,
                                                     G_CC_TEXEL0ONLY));
      gfx_mode_configure(GFX_MODE_TEXT, GFX_TEXT_FAST);
  }

  font = resource_get(RES_FONT_FIPPS);
  uss64_ready = 1;

}

// Try to write Hello World using native GFx.
ENTRY void _start()
{

  init_gp();

  if (!uss64_ready)
  {
    init();
  }

  else
  {
    main_hook();
  }

}
