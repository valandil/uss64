#include <stddef.h>
#include <n64/thread.h>
#include <n64/message.h>
#include <n64/pi.h>
#include <startup.h>
#include "gz/src/gz/gz_api.h"
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

HOOK static void interaction_star_hook1(void)
{
  if (!settings->bits.non_stop)
    func_8024924C(0x7E);
}

HOOK static void interaction_star_hook2(void)
{
  if (!settings->bits.non_stop)
    // This is fragile, as I assume that the a0-a2 registers are untouched
    // by this function. Other code should go below this function call.
    set_mario_action();
}

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
    Gfx ** dl = get_display_list_for_injection();
    gDPSetFillColor((*dl)++, GPACK_RGBA5551(255,0,0,1) << 16 | GPACK_RGBA5551(255,0,0,1));
    gDPFillRectangle((*dl)++, 0, 0, 10, 10);
    gfx_flush();
  }
}

HOOK static void main_hook(void)
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

  /* handle commands */
  for (int i = 0; i < COMMAND_MAX; ++i) {
    _Bool active = 0;
    switch (command_info[i].activation_type) {
      case CMDACT_HOLD:       active = input_bind_held(i);        break;
      case CMDACT_PRESS:      active = input_bind_pressed(i);     break;
      case CMDACT_PRESS_ONCE: active = input_bind_pressed_raw(i); break;
    }
    if (command_info[i].proc && active)
      command_info[i].proc();
  }

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

  // Draw lag counter.
  if (settings->bits.lag_counter)
  {
    // Count the lag frames.
    int32_t lag_frames = (int32_t)SM64_sNumVblanks +
                          uss64.lag_vi_offset - uss64.frame_counter;

    // Adjust x position of the frame counter.
    int x = settings->lag_counter_x - cw * 8;

    // Print the lag counter.
    gfx_mode_replace(GFX_MODE_COLOR, GPACK_RGBA8888(0xC0, 0xC0, 0xC0, alpha));
    gfx_printf(font, x, settings->lag_counter_y, "%8d", lag_frames);
    gfx_mode_pop(GFX_MODE_COLOR);

  }

  // There are two v-blanks per rendered frame (30 fps).
  uss64.frame_counter += 2;

  // Set special triple jump.
  if (settings->bits.special_triple_jump)
    SM64_gSpecialTripleJump = 1;
  else
    SM64_gSpecialTripleJump = 0;

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
  uss64.frame_counter = 0;
  uss64.lag_vi_offset = -(int32_t)SM64_sNumVblanks;
  uss64.cpu_counter = osGetCount();

  // Initialize gfx.
  {
      gfx_start();
      gfx_mode_configure(GFX_MODE_FILTER, G_TF_POINT);
      gfx_mode_configure(GFX_MODE_COMBINE, G_CC_MODE(G_CC_MODULATEIA_PRIM,
                                                     G_CC_MODULATEIA_PRIM));
  }

  // Create menus.
  {
    static struct menu menu;
    static struct menu global;

    // Initialize menu.
    menu_init(&menu, MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);
    menu_init(&global, MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);

    uss64.menu_main = &menu;
    uss64.menu_global = &global;

    // Populate menu.
    menu.selector = menu_add_button(&menu, 0, 0, "return", main_return_proc, NULL);
    menu_add_submenu(&menu, 0, 1, uss64_settings_menu(), "settings");
  }

    /* configure menu related commands */
    input_bind_set_override(COMMAND_MENU, 1);
    input_bind_set_override(COMMAND_RETURN, 1);

  // Load settings.
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
    main_hook();
  }

}
