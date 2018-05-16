Development:
 - Finish Fast3D support in `gbi.h`:
   - [ ] `G_MW_FORCEMTX` and related macros.
   - [ ] Fix `g{,s}SPMatrix`.
   - [ ] Fix `g{,s}SPVertex`.
   - [ ] Fix `g{,s}SPViewport`.
   - [ ] Fix `g{,s}MoveWd`.
   - [ ] Add `g{,s}SPInsertMatrix` (low priority).
   - [ ] Check `g{,s}SPModifyVertex`.
   - [ ] Check whether the `__gsSP1{Triangle,Line3D}_w1` have any use for us,
         since the 2Triangles and Quadrangle stuff doesn't seem to be defined
         in Fast3D. Make sure that they aren't used in gz.
   - [ ] Fix `g{,s}SPCullDisplayList`.
   - [ ] Fix the `g{,s}SPBranch` macros, and `gSPLoadUCode` related macros.
         They aren't defined in Fast3D, so we should make sure that they aren't
         used in gz.
   - [ ] Fix `g{,s}SPNumLights`.
   - [ ] Fix `gSPLight`.
   - [ ] Fix `g{,s}SPLookAt{X,Y}`.
   - [ ] Fix `g{,s}SPopMatrix{,N}`. The N variant doesn't exist in Fast3D, so make
         sure it isn't used in gz.
   - [ ] Fix `g{,s}SPGeometryMode`.
   - [ ] Fix `g{,s}SPSetOtherMode`.


Ideas for a future release:
 - Practice
   - [ ] Infinite lives
   - [ ] Level reset (with camerafix)
   - [ ] Star Select (toggle between Tiny and Huge THI)
   - [ ] Savestates
     - [ ] Double savestates (asm)
     - [ ] Something similar to memfiles in gz, with SD card writes
   - [ ] Level Select (similar to warps in gz)
   - [ ] Pre-made practice files (customizable)
     - [ ] 120 Star
     - [ ] Up RTA 74 star
     - [ ] Up RTA 39 star
     - [ ] Up RTA 38 star
     - [ ] Tippy RTA 103 Star
     - [ ] Tippy RTA 58
 - Timer
   - [ ] Customize/remove TIME text.
   - [ ] Toggle reset timer at level reset/star select/with L.
   - [ ] Slide fix
   - [ ] Show timer in castle
   - [ ] Centiseconds (toggle)
   - [ ] Show timer always, star grab, x-cam (toggle).
   - [ ] Stop timer star grab, x-cam (toggle).
 - HUD elements
   - [ ] Lag counter, reset at level reset/star select/with L.
   - [ ] Lag as lives
   - [ ] Speed display
   - [ ] Speed as stars
 - Cosmetic
   - [ ] No music (toggle)
   - [ ] All Stars coloured (toggle, select color).
   - [ ] Custom dust colour
 - Misc.
   - [ ] Spawn MIPS in basement (toggle)
   - [ ] Spawn sub in DDD (toggle).
   - [ ] Spawn unpressed cap switches (toggle).
   - [ ] Spawn toad stars (toggle).
   - [ ] Spawn fat penguin (toggle).
   - [ ] TTC clock speed.
   - [ ] WDW water level.
   - [ ] Non-stop (toggle).
   - [ ] Save best times (with and without 100c star) to SD card (or EEPROM).
   - [ ] Detect if there is extra lag?