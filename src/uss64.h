#ifndef USS64_H
#define USS64_H

#include "settings.h"

#include "gz/src/gz/menu.h"

// Level numbers for warps.
enum LevelNum
{
    LEVEL_BBH = 4,
    LEVEL_CCM,
    LEVEL_CASTLE,
    LEVEL_HMC,
    LEVEL_SSL,
    LEVEL_BOB,
    LEVEL_SL,
    LEVEL_WDW,
    LEVEL_JRB,
    LEVEL_THI,
    LEVEL_TTC,
    LEVEL_RR,
    LEVEL_CASTLE_GROUNDS,
    LEVEL_BITDW,
    LEVEL_VCUTM,
    LEVEL_BITFS,
    LEVEL_SA,
    LEVEL_BITS,
    LEVEL_LLL,
    LEVEL_DDD,
    LEVEL_WF,
    LEVEL_ENDING,
    LEVEL_CASTLE_COURTYARD,
    LEVEL_PSS,
    LEVEL_COTMC,
    LEVEL_TOTWC,
    LEVEL_BOWSER_1,
    LEVEL_WMOTR,
    LEVEL_UNKNOWN_32,
    LEVEL_BOWSER_2,
    LEVEL_BOWSER_3,
    LEVEL_UNKNOWN_35,
    LEVEL_TTM,
    LEVEL_UNKNOWN_37,
    LEVEL_UNKNOWN_38,
    LEVEL_MAX = LEVEL_UNKNOWN_38
};

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

struct uss64_timer
{
  uint32_t   count;
  int        hundreths;
  int        seconds;
  int        minutes;
  int        hours;
};

// State of the program.
struct uss64
{
  _Bool                 ready;
  _Bool                 menu_active;
  struct menu          *menu_main;
  struct menu          *menu_global;
  int32_t               frame_counter;
  int32_t               lag_vi_offset;
  int64_t               cpu_counter;
  _Bool                 timer_active;
  int64_t               timer_counter_offset;
  int64_t               timer_counter_prev;
  _Bool                 star_grabbed;
  struct uss64_timer    timer_star_grab;
  _Bool                 xcam_triggered;
  struct uss64_timer    timer_xcam;
  _Bool                 command_starselect_was_called;
  uint8_t               command_reset_was_called;
  int8_t                level_num_before_warp;
  int8_t                current_level_num;
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
void command_timer(void);
void command_resettimer(void);
void command_starttimer(void);
void command_stoptimer(void);

// Generic functions
void uss64_warp(int16_t, int16_t, int16_t, int32_t);
void uss64_count_to_timer(int64_t, struct uss64_timer*);

// Functions that define submenus.
struct menu *uss64_settings_menu(void);
struct menu *uss64_warps_menu(void);

// External variables, must be defined in C files.
extern struct uss64            uss64;
extern struct command_info     command_info[COMMAND_MAX];

#endif // USS64_H
