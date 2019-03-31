#include "settings_uss64.h"
#include "gz/src/gz/input.h"
#include "gz/src/gz/resource.h"
#include "gz/src/gz/z64.h"

static _Alignas(16)
struct settings       settings_store;
struct settings_data *settings = &settings_store.data;

static uint16_t settings_checksum_compute(struct settings *settings)
{
  uint16_t checksum = 0;
  uint16_t *p = (void*)&settings->data;
  uint16_t *e = p + sizeof(settings->data) / sizeof(*p);
  while (p < e)
    checksum += *p++;
  return checksum;
}

static _Bool settings_validate(struct settings *settings)
{
  return settings->header.version == SETTINGS_VERSION &&
         settings->header.data_size == sizeof(settings->data) &&
         settings->header.data_checksum == settings_checksum_compute(settings);
}

void settings_load_default(void)
{
  settings_store.header.version = SETTINGS_VERSION;
  settings_store.header.data_size = sizeof(settings_store.data);
  struct settings_data *d = &settings_store.data;
  d->bits.font_resource = RES_FONT_PRESSSTART2P;
  d->bits.drop_shadow = 1;
  d->bits.input_display = 1;
  d->menu_x = 16;
  d->menu_y = 64;
  d->input_display_x = 16;
  d->input_display_y = Z64_SCREEN_HEIGHT - 6;
  d->cheats = 0;
  d->binds[COMMAND_MENU] = input_bind_make(2, BUTTON_R, BUTTON_L);
  d->binds[COMMAND_RETURN] = input_bind_make(2, BUTTON_R, BUTTON_D_LEFT);
  d->binds[COMMAND_FILESELECT] = input_bind_make(2, BUTTON_B, BUTTON_L);
  d->binds[COMMAND_RELOAD] = input_bind_make(1, BUTTON_L);
  d->binds[COMMAND_STARSELECT] = input_bind_make(2, BUTTON_A, BUTTON_L);
}

//! TODO
void settings_save(int profile)
{
  uint16_t *checksum = &settings_store.header.data_checksum;
  *checksum = settings_checksum_compute(&settings_store);
  // Write settings somewhere.
}

//! TODO
_Bool settings_load(int profile)
{

}
