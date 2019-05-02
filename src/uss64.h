#ifndef USS64_H
#define USS64_H

#include "gz/src/gz/menu.h"

// State of the program.
struct uss64
{
  _Bool          ready;
  _Bool          menu_active;
  struct menu   *menu_main;
  struct menu   *menu_global;
  int32_t        frame_counter;
  int32_t        lag_vi_offset;
  int32_t        cpu_counter;
};

extern struct uss64            uss64;

void uss64_apply_settings();
void uss64_show_menu(void);
void uss64_hide_menu(void);

#endif // USS64_H
