#include "gz/src/gz/input.h"
#include "gz/src/gz/menu.h"
#include "gz/src/gz/resource.h"
#include "settings.h"
#include "sm64.h"
#include "uss64.h"

#include <stdlib.h>

static uint16_t font_options[] =
{
  RES_FONT_FIPPS,
  RES_FONT_NOTALOT35,
  RES_FONT_ORIGAMIMOMMY,
  RES_FONT_PCSENIOR,
  RES_FONT_PIXELINTV,
  RES_FONT_PRESSSTART2P,
  RES_FONT_SMWTEXTNC,
  RES_FONT_WERDNASRETURN,
  RES_FONT_PIXELZIM,
};

static int font_proc(struct menu_item *item,
                     enum menu_callback_reason reason,
                     void *data)
{
  if (reason == MENU_CALLBACK_THINK_INACTIVE) {
    if (settings->bits.font_resource != font_options[menu_option_get(item)]) {
      int n_font_options = sizeof(font_options) / sizeof(*font_options);
      for (int i = 0; i < n_font_options; ++i) {
        if (settings->bits.font_resource == font_options[i]) {
          menu_option_set(item, i);
          break;
        }
      }
    }
  }
  else if (reason == MENU_CALLBACK_CHANGED) {
    int font_resource = font_options[menu_option_get(item)];
    settings->bits.font_resource = font_resource;
    if (settings->bits.font_resource == RES_FONT_FIPPS)
      gfx_mode_configure(GFX_MODE_TEXT, GFX_TEXT_NORMAL);
    else
      gfx_mode_configure(GFX_MODE_TEXT, GFX_TEXT_FAST);
    struct gfx_font *font = resource_get(font_resource);
    menu_set_font(uss64.menu_main, font);
    menu_set_cell_width(uss64.menu_main, font->char_width + font->letter_spacing);
    menu_set_cell_height(uss64.menu_main, font->char_height + font->line_spacing);
    menu_imitate(uss64.menu_global, uss64.menu_main);
  }
  return 0;
}

static int drop_shadow_proc(struct menu_item *item,
                            enum menu_callback_reason reason,
                            void *data)
{
  if (reason == MENU_CALLBACK_CHANGED) {
    settings->bits.drop_shadow = menu_checkbox_get(item);
    gfx_mode_set(GFX_MODE_DROPSHADOW, settings->bits.drop_shadow);
  }
  else if (reason == MENU_CALLBACK_THINK)
    menu_checkbox_set(item, settings->bits.drop_shadow);
  return 0;
}

static int non_stop_proc(struct menu_item *item,
                         enum menu_callback_reason reason,
                         void *data)
{
  if (reason == MENU_CALLBACK_CHANGED)
  {
    settings->bits.non_stop = menu_checkbox_get(item);
  }

  else if (reason == MENU_CALLBACK_THINK)
    menu_checkbox_set(item, settings->bits.non_stop);

  return 0;
}

static int special_triple_jump_proc(struct menu_item *item,
                                    enum menu_callback_reason reason,
                                    void *data)
{
  if (reason == MENU_CALLBACK_CHANGED)
  {
    settings->bits.special_triple_jump = menu_checkbox_get(item);
  }

  else if (reason == MENU_CALLBACK_THINK)
    menu_checkbox_set(item, settings->bits.special_triple_jump);

  return 0;
}

static int input_display_proc(struct menu_item *item,
                              enum menu_callback_reason reason,
                              void *data)
{
  if (reason == MENU_CALLBACK_SWITCH_ON)
    settings->bits.input_display = 1;
  else if (reason == MENU_CALLBACK_SWITCH_OFF)
    settings->bits.input_display = 0;
  else if (reason == MENU_CALLBACK_THINK)
    menu_checkbox_set(item, settings->bits.input_display);
  return 0;
}

static int timer_proc(struct menu_item *item,
                              enum menu_callback_reason reason,
                              void *data)
{
  if (reason == MENU_CALLBACK_SWITCH_ON)
    settings->bits.timer = 1;
  else if (reason == MENU_CALLBACK_SWITCH_OFF)
    settings->bits.timer = 0;
  else if (reason == MENU_CALLBACK_THINK)
    menu_checkbox_set(item, settings->bits.timer);
  return 0;
}

static int lag_counter_proc(struct menu_item *item,
                            enum menu_callback_reason reason,
                            void *data)
{
  if (reason == MENU_CALLBACK_SWITCH_ON)
    settings->bits.lag_counter = 1;
  else if (reason == MENU_CALLBACK_SWITCH_OFF)
    settings->bits.lag_counter = 0;
  else if (reason == MENU_CALLBACK_THINK)
    menu_checkbox_set(item, settings->bits.lag_counter);
  return 0;
}

static int generic_position_proc(struct menu_item *item,
                                 enum menu_callback_reason reason,
                                 void *data)
{
  int16_t *x = data;
  int16_t *y = x + 1;
  int dist = 2;
  if (input_pad() & BUTTON_Z)
    dist *= 2;
  switch (reason) {
    case MENU_CALLBACK_ACTIVATE:    input_reserve(BUTTON_Z);  break;
    case MENU_CALLBACK_DEACTIVATE:  input_free(BUTTON_Z);     break;
    case MENU_CALLBACK_NAV_UP:      *y -= dist;               break;
    case MENU_CALLBACK_NAV_DOWN:    *y += dist;               break;
    case MENU_CALLBACK_NAV_LEFT:    *x -= dist;               break;
    case MENU_CALLBACK_NAV_RIGHT:   *x += dist;               break;
    default:
      break;
  }
  return 0;
}

static int menu_position_proc(struct menu_item *item,
                              enum menu_callback_reason reason,
                              void *data)
{
  int r = generic_position_proc(item, reason, &settings->menu_x);
  menu_set_pxoffset(uss64.menu_main, settings->menu_x);
  menu_set_pyoffset(uss64.menu_main, settings->menu_y);
  return r;
}

static void activate_command_proc(struct menu_item *item, void *data)
{
  int command_index = (int)data;
  if (command_info[command_index].proc)
    command_info[command_index].proc();
}

static void tab_prev_proc(struct menu_item *item, void *data)
{
  menu_tab_previous(data);
}

static void tab_next_proc(struct menu_item *item, void *data)
{
  menu_tab_next(data);
}

static void line_spacing_inc_proc(struct menu_item *item, void *data)
{
  settings->line_spacing += 1;
  struct gfx_font *font = resource_get(settings->bits.font_resource);
  menu_set_cell_height(uss64.menu_main, font->char_height + font->line_spacing + settings->line_spacing);
}

static void  line_spacing_dec_proc(struct menu_item *item, void *data)
{
  if (settings->line_spacing > 0)
    settings->line_spacing -= 1;

  struct gfx_font *font = resource_get(settings->bits.font_resource);
  menu_set_cell_height(uss64.menu_main, font->char_height + font->line_spacing + settings->line_spacing);
}

struct menu *uss64_settings_menu(void)
{
  static struct menu menu;
  static struct menu timer;
  static struct menu commands;

  // Initialize menus.
  menu_init(&menu,     MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);
  menu_init(&timer,    MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);
  menu_init(&commands, MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);

  // Populate settings menu.
  int x_checkbox = 20;
  int y = 0;
  menu.selector = menu_add_submenu(&menu, 0, y, NULL, "return");

  /* Submenus, to be populated at the tail of this function */
  menu_add_submenu(&menu, 0, ++y, &timer, "timer");
  menu_add_submenu(&menu, 0, ++y, &commands, "commands");

  /* Appearance controls */
  // Line spacing
  menu_add_static(&menu, 0, ++y, "line spacing", 0xC0C0C0);
  menu_add_watch(&menu, x_checkbox, y, (uint32_t)&settings->line_spacing, WATCH_TYPE_U8);
  menu_add_button(&menu, x_checkbox + 2, y, "-", line_spacing_dec_proc, NULL);
  menu_add_button(&menu, x_checkbox + 4, y, "+", line_spacing_inc_proc, NULL);

  // Font
  menu_add_static(&menu, 0, ++y, "font", 0xC0C0C0);
  menu_add_option(&menu, x_checkbox,  y, "fipps\0""notalot35\0" "origami mommy\0"
                                "pc senior\0""pixel intv\0""press start 2p\0"
                                "smw text nc\0""werdna's return\0""pixelzim\0",
                                font_proc, NULL);

  // Menu position
  menu_add_static(&menu, 0, ++y, "menu position", 0xC0C0C0);
  menu_add_positioning(&menu, x_checkbox + 2, y, menu_position_proc, NULL);

  // Drop shadow
  menu_add_static(&menu, 0,  ++y, "drop shadow", 0xC0C0C0);
  menu_add_checkbox(&menu, x_checkbox, y, drop_shadow_proc, NULL);

  // Non-stop
  menu_add_static(&menu, 0, ++y, "non-stop", 0xC0C0C0);
  menu_add_checkbox(&menu, x_checkbox, y, non_stop_proc, NULL);

  // Special triple jump
  menu_add_static(&menu, 0, ++y, "special triple jump", 0xC0C0C0);
  menu_add_checkbox(&menu, x_checkbox, y, special_triple_jump_proc, NULL);

  // Input display
  menu_add_static(&menu, 0, ++y, "input display", 0xC0C0C0);
  menu_add_checkbox(&menu, x_checkbox, y, input_display_proc, NULL);
  menu_add_positioning(&menu, x_checkbox + 2, y, generic_position_proc,
                       &settings->input_display_x);

  // Populate timer menu.
  y = 0;
  timer.selector = menu_add_submenu(&timer, 0, 0, NULL, "return");

  // Lag counter.
  menu_add_static(&timer, 0, ++y, "lag counter", 0xC0C0C0);
  menu_add_checkbox(&timer, x_checkbox, y, lag_counter_proc, NULL);
  menu_add_positioning(&timer, x_checkbox + 2, y, generic_position_proc,
                       &settings->lag_counter_x);

  // Timer
  menu_add_static(&timer, 0, ++y, "timer", 0xC0C0C0);
  menu_add_checkbox(&timer, x_checkbox, y, timer_proc, NULL);
  menu_add_positioning(&timer, x_checkbox + 2, y, generic_position_proc,
                       &settings->timer_x);

  // Populate commands menu.
  // We allow for multiple pages of commands.
  commands.selector = menu_add_submenu(&commands, 0, 0, NULL, "return");

  const int page_length = 16; // Number of commands per page.
  int n_pages = (COMMAND_MAX + page_length - 1) / page_length;

  struct menu *pages = malloc(sizeof(*pages) * n_pages);
  struct menu_item *tab = menu_add_tab(&commands, 0, 1, pages, n_pages);

  // Print commands.
  for (int i = 0; i < n_pages; ++i)
  {
    // Initialize the page.
    struct menu *page = &pages[i];
    menu_init(page, MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);

    // Draw this page's commands.
    for (int j = 0; j < page_length; ++j)
    {
      int n = i * page_length + j;

      if (n >= COMMAND_MAX)
        break;

      if (command_info[n].proc)
        menu_add_button(page, 0, j, command_info[n].name,activate_command_proc, (void*)n);

      else
        menu_add_static(page, 0, j, command_info[n].name, 0xC0C0C0);

      binder_create(page, 18, j,n);
    }
  }

  if (n_pages > 0)
    menu_tab_goto(tab, 0);

  menu_add_button(&commands, 8,  0, "<", tab_prev_proc, tab);
  menu_add_button(&commands, 10, 0, ">", tab_next_proc, tab);

  return &menu;
}