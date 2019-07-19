.set noat
.set noreorder
.set gp=64

.text

# -- In US/JP, $a2 contains the obj->respawnInfoType. In Shindou, it's v0.
# -- Cannot modify a1,a2 (JP/US).
# -- Cannot modify a1,v0,t6 (S).
.global respawn_objects_hook
respawn_objects_hook:
  lui   $t2, %hi(uss64)
  addiu $t2, $t2, %lo(uss64)
  lbu   $t3, 105($t2)
  li    $at, 1

  # -- Skip this function the function if command_reset was called, and reset the state.
  #beq   $t3, $at, .jumpBackToMainCode
  b      .jumpBackToMainCode
  nop
  #sb    $0 , 105($t2)

  # -- Otherwise, re-implement the function logic.
.checkType32:
  bne   $a2, $at, .checkType16
  nop

  lui   $t5, %hi(0x8029C704)
  addiu $t5, $t5, %lo(0x8029C704)
  jr    $t5
  nop

.checkType16:
  li    $at, 2
  bne   $a2, $at, .jumpBackToMainCode
  nop

  lui   $t5, %hi(0x8029C728)
  addiu $t5, $t5, %lo(0x8029C728)
  jr    $t5
  nop

.jumpBackToMainCode:
  lui   $t5, %hi(0x8029C754)
  addiu $t5, %lo(0x8029C754)
  jr    $t5
  nop

# How to use C structs in GAS
# https://stackoverflow.com/questions/22185848/how-to-re-use-c-structs-in-arm-assembly-in-a-maintainable-and-readable-way

# Make sure that ld does not discard the section with the asm function
# https://stackoverflow.com/questions/31521326/gc-sections-discards-used-data