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

// uss64 variables.
static _Bool __attribute((section(".data")))         uss64_ready = 0;

// Display variables.
static struct gfx_font *font;

// gfx_printf variables
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

  // Stick display.
  gfx_printf(font, 10, Z64_SCREEN_HEIGHT - 10, "%+06.2f %+06.2f", SM64_gPlayer1Controller->stickX,
                                                                  SM64_gPlayer1Controller->stickY);

  // Input display.
  float alpha = 0.7;
  struct gfx_texture *button_texture = resource_get(RES_ICON_BUTTONS);
  gfx_mode_set(GFX_MODE_COLOR, GPACK_RGBA8888(0xC0, 0xC0, 0xC0, alpha));
  static const int buttons[] =
  {
    15, 14, 12, 3, 2, 1, 0, 13, 5, 4, 11, 10, 9, 8,
  };
  uint16_t z_pad = SM64_gPlayer1Controller->buttonDown;
  for (int i = 0; i < sizeof(buttons) / sizeof(*buttons); ++i) {
    int b = buttons[i];
    if (!(z_pad & (1 << b)))
      continue;
    int x = (1.2*button_texture->tile_width) / 2 + i * 12;
    int y = Z64_SCREEN_HEIGHT - 10 - (gfx_font_xheight(font) + button_texture->tile_width + 3) / 2;
    struct gfx_sprite sprite =
    {
      button_texture, b,
      12 * font->char_width + x, y,
      1.2f, 1.2f,
    };
    gfx_mode_set(GFX_MODE_COLOR, GPACK_RGB24A8(input_button_color[b],alpha));
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
      gfx_mode_configure(GFX_MODE_TEXT, GFX_TEXT_NORMAL);
  }

  font = resource_get(RES_FONT_FIPPS);
  uss64_ready = 1;

}

ENTRY void _start()
{

// Call the function we overwrote in the Shindou version.
#ifdef SM64_S
  uint32_t addr = SM64_FuncCalledAtSoundInitHook;
  __asm__ volatile ("addiu  $sp, $sp, -0x18   \n"
                    "sw     $ra, 0x14($sp)    \n"
                    "jal    %0                \n"
                    "lw     $ra, 0x14($sp)    \n"
                    "addiu  $sp, $sp, 0x18    \n"
                    :: "r"(addr));

#endif // SM64_S

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
