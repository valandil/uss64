Ideas for a future release:
 - [ ] Add support for SM64 Europe.
 - [ ] Add support for SM64 iQue.
 - [ ] Add support for SM64 DD.
 - [ ] Include a built-in patcher.
 - Practice
   - [ ] Infinite lives
   - [ ] Level reset (with camerafix)
     - [X] Reset level
     - [ ] Coins don't respawn, and I suspect enemies do not return to their original positions.
     - [ ] When resetting in castle, does not take into account last exit. Should
           map all warp nodes correctly.
   - [ ] Star Select (toggle between Tiny and Huge THI)
     - [ ] This doesn't work. Trying to initiate a warp to PSS and then immediately after
       another warp doesn't work. Must include some kind of delay, or figure out a way
       to instantly trigger star select.
   - [ ] Savestates
     - [ ] Double savestates (asm)
     - [ ] Something similar to memfiles in gz, with SD card writes
   - [X] Level Select (similar to warps in gz)
     - [ ] Not super stable. And some warps won't work depending on the current
       level. Should work out warp nodes.
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
   - [ ] Stop timer star grab, x-cam (show both).
 - HUD elements
   - [X] Lag counter, reset at level reset/star select/with L.
   - [ ] Lag as lives
   - [ ] Speed display
   - [ ] Speed as stars
   - [ ] Number of inputs per second
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
   - [ ] Wireframe viewer.
   - [X] Non-stop (toggle).
   - [X] Special triple jump toggle.
   - [ ] Save best times (with and without 100c star) to SD card ~~(or EEPROM)~~.
   - [ ] Detect if there is extra lag? (Not really possible unless you send the
         same DLs (+ uss64) and check the difference in lag).

Development:
 - [X] Check the textures that SM64 understands (fipps is ia4, for instance).
 - [X] Write a sane build pipeline (Makefile).
   - [X] Give $(BINDIR), $(ASMDIR) and $(PATCHDIR) as an argument to GenerateHooks in Makefile.
   - [X] Create a loop for all supported versions.
   - [X] Simplify directory structure of Makefile output, specifically the patch part.
 - [ ] Generate patches in different formats (~~xdelta~~, bps, ...).
 - [X] Capture input
   - ~~First try through Mario's object, but not sure that's active all the time.~~
   - Otherwise, capture the raw input data from the console.
 - [X] Spawn a simple menu. Just a return option.
 - [X] Code input display.
 - [X] Find more intelligent way to hook. Mario's behaviour is not run at every frame.
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
 - [X] ~~Read RAM entry point from the ROM itself instead of hard-coding it in the Python script.~~ Actually we never parse the ROM for other data, so might as well hard-code it.
 - [X] Check the address at which `SM64_gDisplayListHead` points before and after the
       `gfx_flush()` call to make sure that the DL is inserted.
 - [X] ~~Research the behaviour of the RDP for invalid DLs.~~
       There isn't much in the official docs. Maybe try to inject a known DL with
       deliberately wrong data and see what happens. Choose a second level DL
       to mess with a single layer (or try to).
