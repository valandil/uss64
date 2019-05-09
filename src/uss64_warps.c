#include "gz/src/gz/menu.h"

#include "sm64.h"
#include "uss64.h"

void uss64_warp(int16_t destLevelIndex, int16_t destAreaIndex, int16_t destWarpNodeIndex, int32_t destWarpArg)
{
  reset_dialog_state();
  initiate_warp(destLevelIndex & 0x7F,
                destAreaIndex,
                destWarpNodeIndex,
                destWarpArg);

  if (uss64.current_level_num == destLevelIndex)
    set_play_mode(3);

  else
    set_play_mode(4);
}

// Castle lobby
static void warp_to_bob_proc(struct menu_item *item, void * data)
{
  uss64_warp(LEVEL_BOB, 0x01, 0x0A, 0);
}

static void warp_to_wf_proc(struct menu_item *item, void * data)
{
  uss64_warp(LEVEL_WF, 0x01, 0x0A, 0);
}

static void warp_to_jrb_proc(struct menu_item *item, void *data)
{
  uss64_warp(LEVEL_JRB, 0x01, 0x0A, 0);
}

static void warp_to_ccm_proc(struct menu_item *item, void * data)
{
  uss64_warp(LEVEL_CCM, 0x01, 0x0A, 0);
}

static void warp_to_bbh_proc(struct menu_item *item, void *data)
{
  uss64_warp(LEVEL_BBH, 0x01, 0x0A, 0);
}

// Basement
static void warp_to_hmc_proc(struct menu_item *item, void *data)
{
  uss64_warp(LEVEL_HMC, 0x01, 0x0A, 0);
}

static void warp_to_lll_proc(struct menu_item *item, void *data)
{
  uss64_warp(LEVEL_LLL, 0x01, 0x0A, 0);
}

static void warp_to_ssl_proc(struct menu_item *item, void *data)
{
  uss64_warp(LEVEL_SSL, 0x01, 0x0A, 0);
}

static void warp_to_ddd_proc(struct menu_item *item, void *data)
{
  uss64_warp(LEVEL_DDD, 0x01, 0x0A, 0);
}

// Upstairs
static void warp_to_sl_proc(struct menu_item *item, void *data)
{
  uss64_warp(LEVEL_SL, 0x01, 0x0A, 0);
}

static void warp_to_wdw_proc(struct menu_item *item, void *data)
{
  uss64_warp(LEVEL_WDW, 0x01, 0x0A, 0);
}

static void warp_to_ttm_proc(struct menu_item *item, void *data)
{
  uss64_warp(LEVEL_TTM, 0x01, 0x0A, 0);
}

static void warp_to_thi_proc(struct menu_item *item, void *data)
{
  uss64_warp(LEVEL_THI, 0x01, 0x0A, 0);
}

// Tippy
static void warp_to_ttc_proc(struct menu_item *item, void *data)
{
  uss64_warp(LEVEL_TTC, 0x01, 0x0A, 0);
}

static void warp_to_rr_proc(struct menu_item *item, void *data)
{
  uss64_warp(LEVEL_RR, 0x01, 0x0A, 0);
}

// Bowser stages.
static void warp_to_bitdw_proc(struct menu_item *item, void *data)
{
  uss64_warp(LEVEL_BITDW, 0x01, 0x0A, 0);
}

static void warp_to_bitfs_proc(struct menu_item *item, void *data)
{
  uss64_warp(LEVEL_BITFS, 0x01, 0x0A, 0);
}

static void warp_to_bits_proc(struct menu_item *item, void *data)
{
  uss64_warp(LEVEL_BITS, 0x01, 0x0A, 0);
}

// Bowser fights
static void warp_to_bowser_1_proc(struct menu_item *item, void *data)
{
  uss64_warp(LEVEL_BOWSER_1, 0x01, 0x0A, 0);
}

static void warp_to_bowser_2_proc(struct menu_item *item, void *data)
{
  uss64_warp(LEVEL_BOWSER_2, 0x01, 0x0A, 0);
}

static void warp_to_bowser_3_proc(struct menu_item *item, void *data)
{
  uss64_warp(LEVEL_BOWSER_3, 0x01, 0x0A, 0);
}

// Powerup cap stages.
static void warp_to_totwc_proc(struct menu_item *item, void *data)
{
  uss64_warp(LEVEL_TOTWC, 0x01, 0x0A, 0);
}

static void warp_to_cotmc_proc(struct menu_item *item, void *data)
{
  uss64_warp(LEVEL_COTMC, 0x01, 0x0A, 0);
}

static void warp_to_vcutm_proc(struct menu_item *item, void *data)
{
  uss64_warp(LEVEL_VCUTM, 0x01, 0x0A, 0);
}

// Secret stages
static void warp_to_pss_proc(struct menu_item *item, void *data)
{
  uss64_warp(LEVEL_PSS, 0x01, 0x0A, 0);
}

static void warp_to_sa_proc(struct menu_item *item, void *data)
{
  uss64_warp(LEVEL_SA, 0x01, 0x0A, 0);
}

static void warp_to_wmotr_proc(struct menu_item *item, void *data)
{
  uss64_warp(LEVEL_WMOTR, 0x01, 0x0A, 0);
}

struct menu *uss64_warps_menu(void)
{

  static struct menu menu;
  static struct menu castle_lobby;
  static struct menu basement;
  static struct menu upstairs;
  static struct menu tippy;
  static struct menu bowser_stages;
  static struct menu bowser_fights;
  static struct menu secret_levels;
  static struct menu cap_levels;

  /* initialize menus */
  menu_init(&menu, MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);
  menu_init(&castle_lobby, MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);
  menu_init(&basement, MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);
  menu_init(&upstairs, MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);
  menu_init(&tippy, MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);
  menu_init(&bowser_stages, MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);
  menu_init(&bowser_fights, MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);
  menu_init(&secret_levels, MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);
  menu_init(&cap_levels, MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);

  /* populate warps top menu */
  int y = 0;
  menu.selector = menu_add_submenu(&menu, 0, ++y, NULL, "return");
  menu_add_submenu(&menu, 0, ++y, &castle_lobby, "castle lobby");
  menu_add_submenu(&menu, 0, ++y, &basement, "basement");
  menu_add_submenu(&menu, 0, ++y, &upstairs, "upstairs");
  menu_add_submenu(&menu, 0, ++y, &tippy, "tippy");
  menu_add_submenu(&menu, 0, ++y, &bowser_stages, "bowser stages");
  menu_add_submenu(&menu, 0, ++y, &bowser_fights, "bowser fights");
  menu_add_submenu(&menu, 0, ++y, &secret_levels, "secret levels");
  menu_add_submenu(&menu, 0, ++y, &cap_levels, "cap levels");

  // Populate castle lobby menu.
  y = 0;
  castle_lobby.selector = menu_add_submenu(&castle_lobby, 0, ++y, NULL, "return");
  menu_add_button(&castle_lobby, 0, ++y, "BOB", warp_to_bob_proc, NULL);
  menu_add_button(&castle_lobby, 0, ++y, "WF", warp_to_wf_proc, NULL);
  menu_add_button(&castle_lobby, 0, ++y, "JRB", warp_to_jrb_proc, NULL);
  menu_add_button(&castle_lobby, 0, ++y, "CCM", warp_to_ccm_proc, NULL);
  menu_add_button(&castle_lobby, 0, ++y, "BBH", warp_to_bbh_proc, NULL);

  // Populate basement menu
  y = 0;
  basement.selector = menu_add_submenu(&basement, 0, ++y, NULL, "return");
  menu_add_button(&basement, 0, ++y, "HMC", warp_to_hmc_proc, NULL);
  menu_add_button(&basement, 0, ++y, "LLL", warp_to_lll_proc, NULL);
  menu_add_button(&basement, 0, ++y, "SSL", warp_to_ssl_proc, NULL);
  menu_add_button(&basement, 0, ++y, "DDD", warp_to_ddd_proc, NULL);

  // Populate upstairs menu
  y = 0;
  upstairs.selector = menu_add_submenu(&upstairs, 0, ++y, NULL, "return");
  menu_add_button(&upstairs, 0, ++y, "SL", warp_to_sl_proc, NULL);
  menu_add_button(&upstairs, 0, ++y, "WDW", warp_to_wdw_proc, NULL);
  menu_add_button(&upstairs, 0, ++y, "TTM", warp_to_ttm_proc, NULL);
  menu_add_button(&upstairs, 0, ++y, "THI", warp_to_thi_proc, NULL);

  // Populate tippy
  y = 0;
  tippy.selector = menu_add_submenu(&tippy, 0, ++y, NULL, "return");
  menu_add_button(&tippy, 0, ++y, "TTC", warp_to_ttc_proc, NULL);
  menu_add_button(&tippy, 0, ++y, "RR", warp_to_rr_proc, NULL);

  // Populate Bowser stages menu.
  y = 0;
  bowser_stages.selector = menu_add_submenu(&bowser_stages, 0, ++y, NULL, "return");
  menu_add_button(&bowser_stages, 0, ++y, "BitDW", warp_to_bitdw_proc, NULL);
  menu_add_button(&bowser_stages, 0, ++y, "BitFS", warp_to_bitfs_proc, NULL);
  menu_add_button(&bowser_stages, 0, ++y, "BitS", warp_to_bits_proc, NULL);

  // Populate Bowser fights.
  y = 0;
  bowser_fights.selector = menu_add_submenu(&bowser_fights, 0, ++y, NULL, "return");
  menu_add_button(&bowser_fights, 0, ++y, "Bowser 1", warp_to_bowser_1_proc, NULL);
  menu_add_button(&bowser_fights, 0, ++y, "Bowser 2", warp_to_bowser_2_proc, NULL);
  menu_add_button(&bowser_fights, 0, ++y, "Bowser 3", warp_to_bowser_3_proc, NULL);

  // Populate secret stages.
  y = 0;
  secret_levels.selector = menu_add_submenu(&secret_levels, 0, ++y, NULL, "return");
  menu_add_button(&secret_levels, 0, ++y, "PSS", warp_to_pss_proc, NULL);
  menu_add_button(&secret_levels, 0, ++y, "SA", warp_to_sa_proc, NULL);
  menu_add_button(&secret_levels, 0, ++y, "WMOTR", warp_to_wmotr_proc, NULL);

  // Populate cap levels
  y = 0;
  cap_levels.selector = menu_add_submenu(&cap_levels, 0, ++y, NULL, "return");
  menu_add_button(&cap_levels, 0, ++y, "TotWC", warp_to_totwc_proc, NULL);
  menu_add_button(&cap_levels, 0, ++y, "CotMC", warp_to_cotmc_proc, NULL);
  menu_add_button(&cap_levels, 0, ++y, "VCutM", warp_to_vcutm_proc, NULL);

  return &menu;
}