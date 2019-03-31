#include "uss64.h"
#include "settings_uss64.h"
#include "gz/src/gz/menu.h"
#include "gz/src/gz/gfx.h"
#include "gz/src/gz/input.h"
#include "gz/src/gz/resource.h"

void uss64_apply_settings()
{
  struct gfx_font *font = resource_get(settings->bits.font_resource);
  menu_set_font(uss64.menu_main, font);
  menu_set_cell_width(uss64.menu_main, font->char_width + font->letter_spacing);
  menu_set_cell_height(uss64.menu_main, font->char_height + font->line_spacing);
  //gfx_mode_set(GFX_MODE_DROPSHADOW, settings->bits.drop_shadow);
  if (settings->bits.font_resource == RES_FONT_FIPPS)
    gfx_mode_configure(GFX_MODE_TEXT, GFX_TEXT_NORMAL);
  else
    gfx_mode_configure(GFX_MODE_TEXT, GFX_TEXT_FAST);
  menu_set_pxoffset(uss64.menu_main, settings->menu_x);
  menu_set_pyoffset(uss64.menu_main, settings->menu_y);
  menu_imitate(uss64.menu_global, uss64.menu_main);
}

void uss64_show_menu(void)
{
  menu_signal_enter(uss64.menu_main, MENU_SWITCH_SHOW);
  uss64.menu_active = 1;
  input_reserve(BUTTON_D_UP | BUTTON_D_DOWN | BUTTON_D_LEFT | BUTTON_D_RIGHT |
                BUTTON_L);
  input_reservation_set(1);
}

void uss64_hide_menu(void)
{
  menu_signal_leave(uss64.menu_main, MENU_SWITCH_HIDE);
  uss64.menu_active = 0;
  input_free(BUTTON_D_UP | BUTTON_D_DOWN | BUTTON_D_LEFT | BUTTON_D_RIGHT |
             BUTTON_L);
  input_reservation_set(0);
}