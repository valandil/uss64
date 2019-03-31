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
};

extern struct uss64            uss64;

void uss64_apply_settings();
void uss64_show_menu(void);
void uss64_hide_menu(void);

#endif // USS64_H
