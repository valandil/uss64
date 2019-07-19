#include <stdint.h>
#include <stddef.h>

#include "uss64.h"

#define DEFINE(sym, val) \
  __asm__ volatile("__AS_DEFINE__" #sym "\t%0" : : "n" ((unsigned long) val))

#define OFFSET(sym, str, mem) \
  DEFINE(sym, offsetof(struct str, mem))

int main(void) {

  OFFSET(USS64_CMD_STARSELECT_CALLED, uss64, command_starselect_was_called);
  OFFSET(USS64_CMD_RELOAD_CALLED,     uss64, command_reset_was_called);

  if (uss64.command_reset_was_called == 1)
  {
    OFFSET(USS64_CURRENT_LVL_NUM, uss64, current_level_num);
  }
}
