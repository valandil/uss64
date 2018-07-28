Development:
 - Check the textures that SM64 understands (fipps is ia4, for instance).
 - Write a sane build pipeline (Makefile).
 - Generate patches in different formats (xdelta, bps, ...).
 - Finish Fast3D support in `gbi.h`:
   - [X] `G_MW_FORCEMTX` and related macros.
   - [X] Fix `g{,s}SPMatrix`.
   - [X] Fix `g{,s}SPVertex`.
   - [X] Fix `g{,s}SPViewport`.
   - [X] Fix `g{,s}MoveWd`.
   - [X] Add `g{,s}SPInsertMatrix` (low priority).
   - [X] Check `g{,s}SPModifyVertex`.
   - [X] Check whether the `__gsSP1{Triangle,Line3D}_w1` have any use for us,
         since the 2Triangles and Quadrangle stuff doesn't seem to be defined
         in Fast3D. Make sure that they aren't used in gz.
   - [X] Fix `g{,s}SPCullDisplayList`.
   - [X] Fix the `g{,s}SPBranch` macros, and `gSPLoadUCode` related macros.
         They aren't defined in Fast3D, so we should make sure that they aren't
         used in gz.
   - [X] Fix `g{,s}SPNumLights`.
   - [X] Fix `gSPLight`.
   - [X] Fix `g{,s}SPLookAt{X,Y}`.
   - [X] Fix `g{,s}SPopMatrix{,N}`. The N variant doesn't exist in Fast3D, so make
         sure it isn't used in gz.
   - [X] ~~Fix `g{,s}SPGeometryMode`~~. Check whether gsSPLoadGeometryMode has an equivalent in F3D.
   - [X] Fix `g{,s}SPSetOtherMode`.
 - [ ] Read RAM entry point from the ROM itself instead of hard-coding it in the Python script.
 - [X] Check the address at which `SM64_gDisplayListHead` points before and after the
       `gfx_flush()` call to make sure that the DL is inserted.
 - [ ] ~~Research the behaviour of the RDP for invalid DLs.~~
       There isn't much in the official docs. Maybe try to inject a known DL with
       deliberately wrong data and see what happens. Choose a second level DL
       to mess with a single layer (or try to).


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