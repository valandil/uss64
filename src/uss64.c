#include <stddef.h>
#include <n64/thread.h>
#include <n64/message.h>
#include <n64/pi.h>
#include <startup.h>
#include "gz/src/gz/z64.h"
#include "gz/src/gz/resource.h"
#include "gz/src/gz/gfx.h"

#include "sm64.h"
#include "input.h"

static _Bool __attribute((section(".data")))         uss64_ready = 0;
static const char HelloString[] = "hello n64";
static unsigned int x = 64;
static unsigned int y = 32;

// Display variables.
#define GFX_DISP_SIZE 1000
static struct gfx_font *font;

// gfx_printf variables
static const char USS64String[] = "hello from uss64";
static       int  master_dlist_addr;
static       Gfx* master_dlist_ptr;

HOOK static void display_hook(void)
{

  // Call the function we overwrote.
  uint32_t addr = funcCalledAtCleanUpDisplayListHook;
  __asm__ volatile ("addiu  $sp, $sp, -0x18   \n"
                    "sw     $ra, 0x14($sp)    \n"
                    "jal    %0                \n"
                    "lw     $ra, 0x14($sp)    \n"
                    "addiu  $sp, $sp, 0x18    \n"
                    :: "r"(addr));

  if (uss64_ready)
  {
    // Branch off the tail of the master DL.
    //PrintXY(x,y, HelloString);
    
    // Try to manually write the DL
    gDPSetFillColor(SM64_gDisplayListHead++, GPACK_RGBA5551(255,0,0,1) << 16 | GPACK_RGBA5551(255,0,0,1));
    gDPFillRectangle(SM64_gDisplayListHead++, 0, 0, 10, 10);
    gfx_flush();

  }
}

HOOK static void main_hook(void)
{
  // Try to print with `gfx.c`.
  gfx_mode_init();
  //gfx_printf(font, 40, 20, "hello world");

  // Print out the buttonPressed and buttonDown values on screen.
  char buffer[33];
  itoa (SM64_gPlayer1Controller->buttonDown, buffer, 2);
  gfx_printf(font,40,20, "%s", buffer);
  gfx_printf(font,40,40, "test"); 

  // Try to print when the L button is pressed.
  if (SM64_gPlayer1Controller->buttonDown & L_TRIG)
  {
    gfx_printf(font,40,40, "L trigger is pressed");
  }
}

HOOK static void init(void)
{
  // Initialize the static variables (I think).
  clear_bss();
  do_global_ctors();

  // Initialize gfx.
  {
      gfx_start();
      gfx_mode_configure(GFX_MODE_FILTER, G_TF_POINT);
      gfx_mode_configure(GFX_MODE_COMBINE, G_CC_MODE(G_CC_TEXEL0ONLY,
                                                     G_CC_TEXEL0ONLY));
      gfx_mode_configure(GFX_MODE_TEXT, GFX_TEXT_FAST);
  }

  font = resource_get(RES_FONT_FIPPS);
  uss64_ready = 1;

}

// Try to write Hello World using native GFx.
ENTRY void _start()
{

  init_gp();

  if (!uss64_ready)
  {
    init();
  }

  else
  {
    main_hook();
  }

}
