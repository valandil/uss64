#include "gz/src/gz/gz_api.h"

#include "sm64.h"

// gfx functions
Gfx* get_display_list_for_injection(void)
{
  return SM64_gDisplayListHead;
}

// input functions
int8_t get_raw_input_x(void)
{
  return SM64_gPlayer1Controller->controllerData->rawStickX;
}

int8_t get_raw_input_y(void)
{
  return SM64_gPlayer1Controller->controllerData->rawStickX;
}

uint16_t get_input_z_pad(void)
{
  return SM64_gPlayer1Controller->controllerData->button;
}
