#ifndef USS64_H
#define USS64_H

#include "settings.h"

#include "gz/src/gz/menu.h"

enum cmdact
{
  CMDACT_HOLD,
  CMDACT_PRESS,
  CMDACT_PRESS_ONCE,
};

struct command_info
{
  const char    *name;
  void         (*proc)(void);
  enum cmdact    activation_type;
};

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


// Menu-related functions.
void uss64_apply_settings();
void uss64_show_menu(void);
void uss64_hide_menu(void);

// Commands.
void command_fileselect(void);
void command_reload(void);
void command_starselect(void);
void command_resetlag(void);

// Functions that define submenus.
struct menu *uss64_settings_menu(void);

// External variables, must be defined in C files.
extern struct uss64            uss64;
extern struct command_info     command_info[COMMAND_MAX];

#endif // USS64_H
