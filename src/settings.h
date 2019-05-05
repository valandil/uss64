#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h>

#define SETTINGS_PADSIZE            ((sizeof(struct settings)+1)/2*2)
#define SETTINGS_VERSION            0x0001

#define SETTINGS_BIND_MAX           COMMAND_MAX

enum cheats
{
  CHEAT_LIFE,
  CHEAT_COINS,
  CHEAT_MAX,
};

enum commands
{
  COMMAND_MENU,
  COMMAND_RETURN,
  COMMAND_FILESELECT,
  COMMAND_RELOAD,
  COMMAND_STARSELECT,
  COMMAND_RESETLAG,
  COMMAND_MAX,
};

struct settings_bits
{
  uint32_t font_resource       : 4;
  uint32_t drop_shadow         : 1;
  uint32_t input_display       : 1;
  uint32_t lag_counter         : 1;
  uint32_t non_stop            : 1;
  uint32_t special_triple_jump : 1;
};

struct settings_data
{
  uint32_t             cheats;
  struct settings_bits bits;
  int16_t              menu_x;
  int16_t              menu_y;
  uint8_t              line_spacing;
  int16_t              input_display_x;
  int16_t              input_display_y;
  int16_t              lag_counter_x;
  int16_t              lag_counter_y;
  uint16_t             binds[SETTINGS_BIND_MAX];
};

struct settings_header
{
  uint16_t  version;
  uint16_t  data_size;
  uint16_t  data_checksum;
};

struct settings
{
  struct settings_header  header;
  struct settings_data    data;
};

void  settings_load_default(void);
void  settings_save(int profile);
_Bool settings_load(int profile);

extern struct settings_data *settings;

#endif // SETTINGS_USS64_H
