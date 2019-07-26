#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include "uss64.h"
#include "settings.h"
#include "sm64.h"
#include "gz/src/gz/menu.h"
#include "gz/src/gz/gfx.h"
#include "gz/src/gz/input.h"
#include "gz/src/gz/resource.h"

struct command_info command_info[COMMAND_MAX] =
{
  {"show/hide menu",    NULL,               CMDACT_PRESS_ONCE},
  {"return from menu",  NULL,               CMDACT_PRESS_ONCE},
  {"file select",       command_fileselect, CMDACT_PRESS_ONCE},
  {"reload level",      command_reload,     CMDACT_PRESS_ONCE},
  {"star select",       command_starselect, CMDACT_PRESS_ONCE},
  {"reset lag counter", command_resetlag,   CMDACT_PRESS_ONCE},
  {"start/stop timer",  command_timer,      CMDACT_PRESS_ONCE},
  {"reset timer",       command_resettimer, CMDACT_PRESS_ONCE},
  {"start timer",       command_starttimer, CMDACT_PRESS_ONCE},
  {"stop timer",        command_stoptimer,  CMDACT_PRESS_ONCE},
};

void uss64_apply_settings()
{
  struct gfx_font *font = resource_get(settings->bits.font_resource);
  menu_set_font(uss64.menu_main, font);
  menu_set_cell_width(uss64.menu_main, font->char_width + font->letter_spacing);
  menu_set_cell_height(uss64.menu_main, font->char_height + font->line_spacing + settings->line_spacing);
  gfx_mode_set(GFX_MODE_DROPSHADOW, settings->bits.drop_shadow);
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

void uss64_log(const char *fmt, ...)
{
  struct log_entry *ent = &uss64.log[SETTINGS_LOG_MAX - 1];
  if (ent->msg)
    free(ent->msg);
  for (int i = SETTINGS_LOG_MAX - 1; i > 0; --i)
    uss64.log[i] = uss64.log[i - 1];

  va_list va;
  va_start(va, fmt);
  int l = vsnprintf(NULL, 0, fmt, va);
  va_end(va);

  ent = &uss64.log[0];
  ent->msg = malloc(l + 1);
  if (!ent->msg)
    return;
  va_start(va, fmt);
  vsprintf(ent->msg, fmt, va);
  va_end(va);
  ent->age = 0;
}

void command_fileselect(void)
{
  func_8024975C(-2);
}

void command_reload(void)
{

  // Fill Mario's health, reset flags and coins.
  SM64_gMarioState->health = 0x0880;
  SM64_gMarioState->flags = 0;
  SM64_gMarioState->numCoins = 0;
  SM64_gHudDisplay.coins = 0;

  // Reset environment VFX.
  SM64_gSnowParticleCount = 5;

  reset_dialog_state();

  initiate_warp(uss64.current_level_num & 0x7F, 0x01, 0x0A, 0);

  // Change area warp type to trigger re-spawning.
  SM64_sCurrWarpType = 0x0002;
  uss64.command_reset_was_called = 1;

  // Camera fix.
  uint16_t * addr = (uint16_t*)SM64_lvl_reset_camera_fix_hook;
  if (uss64.current_level_num != LEVEL_TOTWC)
    *addr = 0x0001;
  else
    *addr = 0x0000;
}

void command_starselect(void)
{

  reset_dialog_state();
  initiate_warp(uss64.current_level_num & 0x7F, 0x01, 0xA, 0);
  SM64_sCurrWarpType = 0x0001;
  set_play_mode(4);
}

void command_resetlag(void)
{
  uss64.frame_counter = 0;
  uss64.lag_vi_offset = -(int32_t)SM64_sNumVblanks;
}

void command_timer(void)
{
  uss64.timer_active = !uss64.timer_active;
}

void command_resettimer(void)
{
  uss64.timer_counter_offset = -uss64.cpu_counter;
  uss64.timer_counter_prev = uss64.cpu_counter;
  uss64.star_grabbed = 0;
  uss64.xcam_triggered = 0;
}

void command_starttimer(void)
{
  if (!uss64.timer_active)
    command_timer();
}

void command_stoptimer(void)
{
  if (uss64.timer_active)
    command_timer();
}