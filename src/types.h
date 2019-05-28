#ifndef _TYPES_H_
#define _TYPES_H_

// This file contains various data types used in Super Mario 64 that don't yet
// have an appropriate header.

#include <n64.h>

#include "input.h"

typedef float Vec3f[3];
typedef int16_t Vec3s[3];

struct MarioState
{
    /*0x00*/ uint16_t unk00;
    /*0x02*/ uint16_t input;
    /*0x04*/ uint32_t flags;
    /*0x08*/ uint32_t particleFlags;
    /*0x0C*/ uint32_t action;
    /*0x10*/ uint32_t prevAction;
    /*0x14*/ uint32_t unk14;
    /*0x18*/ uint16_t actionState;
    /*0x1A*/ uint16_t actionTimer;
    /*0x1C*/ uint32_t actionArg;
    /*0x20*/ float intendedMag;
    /*0x24*/ int16_t intendedYaw;
    /*0x26*/ int16_t invincTimer;
    /*0x28*/ uint8_t framesSinceA;
    /*0x29*/ uint8_t framesSinceB;
    /*0x2A*/ uint8_t wallKickTimer;
    /*0x2B*/ uint8_t doubleJumpTimer;
    /*0x2C*/ Vec3s faceAngle;
    /*0x32*/ Vec3s angleVel;
    /*0x38*/ int16_t slideYaw;
    /*0x3A*/ int16_t twirlYaw;
    /*0x3C*/ Vec3f pos;
    /*0x48*/ Vec3f vel;
    /*0x54*/ float forwardVel;
    /*0x58*/ float slideVelX;
    /*0x5C*/ float slideVelZ;
    /*0x60*/ uint32_t *wall;
    /*0x64*/ uint32_t *ceil;
    /*0x68*/ uint32_t *floor;
    /*0x6C*/ float ceilHeight;
    /*0x70*/ float floorHeight;
    /*0x74*/ int16_t floorAngle;
    /*0x76*/ int16_t waterLevel;
    /*0x78*/ uint32_t *interactObj;
    /*0x7C*/ uint32_t *heldObj;
    /*0x80*/ uint32_t *usedObj;
    /*0x84*/ uint32_t *riddenObj;
    /*0x88*/ uint32_t *marioObj;
    /*0x8C*/ uint32_t *spawnInfo;
    /*0x90*/ uint32_t *area;
    /*0x94*/ uint32_t *unk94;
    /*0x98*/ uint32_t *unk98;
    /*0x9C*/ Controller *controller;
    /*0xA0*/ uint32_t *animation;
    /*0xA4*/ uint32_t collidedObjInteractTypes;
    /*0xA8*/ int16_t numCoins;
    /*0xAA*/ int16_t numStars;
    /*0xAC*/ int8_t numKeys; // Unused key mechanic
    /*0xAD*/ int8_t numLives;
    /*0xAE*/ int16_t health;
    /*0xB0*/ int16_t unkB0;
    /*0xB2*/ uint8_t hurtCounter;
    /*0xB3*/ uint8_t healCounter;
    /*0xB4*/ uint8_t squishTimer;
    /*0xB5*/ uint8_t fadeWarpOpacity;
    /*0xB6*/ uint16_t capTimer;
    /*0xB8*/ int16_t unkB8;
    /*0xBC*/ float peakHeight;
    /*0xC0*/ float quicksandDepth;
    /*0xC4*/ float unkC4;
};

#endif
