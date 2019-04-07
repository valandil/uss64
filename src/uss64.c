#include <stddef.h>
#include <n64/thread.h>
#include <n64/message.h>
#include <n64/pi.h>
#include <startup.h>
#include "gz/src/gz/z64.h"
#include "gz/src/gz/input.h"
#include "gz/src/gz/resource.h"
#include "gz/src/gz/gfx.h"

#include "sm64.h"
#include "input.h"
#include "uss64.h"
#include "settings.h"

// uss64 variables. The ready variable must be accessible from the get-go.
__attribute__((section(".data")))
struct uss64 uss64 =
{
  .ready = 0,
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

  if (uss64.ready)
  {
    // Try to manually write the DL
    Gfx * dl = get_display_list_for_injection();
    gDPSetFillColor(dl++, GPACK_RGBA5551(255,0,0,1) << 16 | GPACK_RGBA5551(255,0,0,1));
    gDPFillRectangle(dl++, 0, 0, 10, 10);
    gfx_flush();
  }
}

HOOK static void main(void)
{
  input_update();
  gfx_mode_init();

  // Handle menu input.
  if (uss64.menu_active)
  {
    if (input_bind_pressed_raw(COMMAND_MENU))
      uss64_hide_menu();

    else if (input_bind_pressed(COMMAND_RETURN))
      menu_return(uss64.menu_main);

    else
    {
      uint16_t pad_pressed = input_pressed();

      if (pad_pressed & BUTTON_D_UP)
        menu_navigate(uss64.menu_main, MENU_NAVIGATE_UP);

      if (pad_pressed & BUTTON_D_DOWN)
        menu_navigate(uss64.menu_main, MENU_NAVIGATE_DOWN);

      if (pad_pressed & BUTTON_D_LEFT)
        menu_navigate(uss64.menu_main, MENU_NAVIGATE_LEFT);

      if (pad_pressed & BUTTON_D_RIGHT)
        menu_navigate(uss64.menu_main, MENU_NAVIGATE_RIGHT);

      if (pad_pressed & BUTTON_L)
        menu_activate(uss64.menu_main);
    }
  }

  else if (input_bind_pressed_raw(COMMAND_MENU))
    uss64_show_menu();

  // Animate menus
  while (uss64.menu_active && menu_think(uss64.menu_main));
  while (menu_think(uss64.menu_global));

  // Get menu appearance.
  struct gfx_font *font = menu_get_font(uss64.menu_main, 1);
  uint8_t alpha = menu_get_alpha_i(uss64.menu_main, 1);
  int cw = menu_get_cell_width(uss64.menu_main, 1);
  int ch = menu_get_cell_height(uss64.menu_main, 1);

    /* draw menus */
  if (uss64.menu_active)
    menu_draw(uss64.menu_main);
  menu_draw(uss64.menu_global);

  // Draw input display
  if (settings->bits.input_display)
  {
    // Stick display.
    gfx_printf(font, settings->input_display_x, settings->input_display_y,
                "%4i %4i", input_x(), input_y());

    // Input display.
    struct gfx_texture *button_texture = resource_get(RES_ICON_BUTTONS);
    gfx_mode_set(GFX_MODE_COLOR, GPACK_RGBA8888(0xC0, 0xC0, 0xC0, alpha));
    static const int buttons[] =
    {
      15, 14, 12, 3, 2, 1, 0, 13, 5, 4, 11, 10, 9, 8,
    };
    uint16_t z_pad = input_pad();
    for (int i = 0; i < sizeof(buttons) / sizeof(*buttons); ++i)
    {
      int b = buttons[i];
      if (!(z_pad & (1 << b)))
        continue;
      int x = (cw - button_texture->tile_width) / 2 + i * 14;
      int y = - (gfx_font_xheight(font) + button_texture->tile_width + 3) / 2;
      struct gfx_sprite sprite =
      {
        button_texture, b,
        settings->input_display_x + 10 * cw + x, settings->input_display_y + y,
        1.2f, 1.2f,
      };
      gfx_mode_replace(GFX_MODE_COLOR, GPACK_RGB24A8(input_button_color[b],alpha));
      gfx_sprite_draw(&sprite);
      gfx_mode_pop(GFX_MODE_COLOR);
    }
  }
}

static void main_return_proc(struct menu_item *item, void *data)
{
  uss64_hide_menu();
}

HOOK static void init(void)
{
  // Initialize the static variables (I think).
  clear_bss();
  do_global_ctors();

  // Initialize uss64 variables.
  uss64.menu_active = 0;

  // Initialize gfx.
  {
      gfx_start();
      gfx_mode_configure(GFX_MODE_FILTER, G_TF_POINT);
      gfx_mode_configure(GFX_MODE_COMBINE, G_CC_MODE(G_CC_MODULATEIA_PRIM,
                                                     G_CC_MODULATEIA_PRIM));
  }

  // Create menus.
  {
    // Initialize menu.
    static struct menu menu;
    static struct menu global;
    menu_init(&menu, MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);
    menu_init(&global, MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);
    uss64.menu_main = &menu;
    uss64.menu_global = &global;

    // Populate menu.
    menu.selector = menu_add_button(&menu, 0, 0, "return", main_return_proc, NULL);
  }

  settings_load_default();
  uss64_apply_settings();

  uss64.ready = 1;
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

  if (!uss64.ready)
  {
    init();
  }

  else
  {
    main();
  }

}
