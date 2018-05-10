#include <stddef.h>
#include <n64/thread.h>
#include <n64/message.h>
#include <n64/pi.h>
#include <startup.h>
#include "gz/src/gz/z64.h"
#include "gz/src/gz/resource.h"
#include "gz/src/gz/gfx.h"

#include "sm64.h"

static const char HelloString[] = "hello n64";
static unsigned int x = 64;
static unsigned int y = 32;
static _Bool __attribute((section(".data")))         _ready = 0;

// Display variables.
#define GFX_DISP_SIZE 1000
static struct gfx_font *font;

// gfx_printf variables
static const char USS64String[] = " hello from uss64 ";
static       int  master_dlist_addr;
static       Gfx* master_dlist_ptr;

static void stack_thunk(void (*func)(void))
{
  static __attribute__((section(".stack"))) _Alignas(8)
  char stack[0x2000];
  __asm__ volatile ("la     $t0, %1       \n"
                    "sw     $sp, -4($t0)  \n"
                    "sw     $ra, -8($t0)  \n"
                    "addiu  $sp, $t0, -8  \n"
                    "jalr   %0            \n"
                    "lw     $ra, 0($sp)   \n"
                    "lw     $sp, 4($sp)   \n"
                    :: "r"(func), "i"(&stack[sizeof(stack)]));
}

HOOK static void display_hook(void)
{
  // Call the function we overwrote.
  stack_thunk(func_0x8024784C);

  // Branch off the tail of the master DL.
  gfx_flush();
}

HOOK static void main_hook(void)
{
  gfx_mode_init();
  gfx_printf(font, x, y, USS64String);
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
      gfx_mode_configure(GFX_MODE_COMBINE, G_CC_MODE(G_CC_MODULATEIA_PRIM,
                                                     G_CC_MODULATEIA_PRIM));
  }

  font = resource_get(RES_FONT_FIPPS);
  _ready = 1;

}

// Try to write Hello World using native GFx.
ENTRY void _start()
{

  init_gp();

  if (!_ready)
  {
    init();
  }

  else
  {
    main_hook();
  }

  // // Get the address of the master dlist.
  // master_dlist_addr  = GetSegmentBase(0x01);
  // master_dlist_ptr   = (Gfx*)SegmentedToVirtual((void*)master_dlist_addr);
  // gfx_flush();

  // // Print hello world
  // PrintXY(x, y, HelloString);
  // sm64_printf(x, y, (void*)0x80338388, master_dlist_addr);
}
