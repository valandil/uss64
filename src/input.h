/// Taken from sm64_source.
#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

#define A_BUTTON                (1 << 15) // 0x8000
#define B_BUTTON                (1 << 14) // 0x4000
#define Z_TRIG                  (1 << 13) // 0x2000
#define START_BUTTON            (1 << 12) // 0x1000
#define U_JPAD                  (1 << 11) // 0x0800
#define D_JPAD                  (1 << 10) // 0x0400
#define L_JPAD                  (1 << 9)  // 0x0200
#define R_JPAD                  (1 << 8)  // 0x0100
#define DUMMY_1                 (1 << 7)  // 0x0080 (unused)
#define DUMMY_2                 (1 << 6)  // 0x0040 (unused)
#define L_TRIG                  (1 << 5)  // 0x0020
#define R_TRIG                  (1 << 4)  // 0x0010
#define U_CBUTTONS              (1 << 3)  // 0x0008
#define D_CBUTTONS              (1 << 2)  // 0x0004
#define L_CBUTTONS              (1 << 1)  // 0x0002
#define R_CBUTTONS              (1 << 0)  // 0x0001


typedef struct {
    /*0x00*/ uint16_t      type;            /* Controller Type */
    /*0x02*/ uint8_t       status;          /* Controller status */
    /*0x03*/ uint8_t       errno;
} OSContStatus;

typedef struct {
    /*0x00*/ uint16_t      button;
    /*0x02*/ int8_t        rawStickX;       /* -80 <= stick_x <= 80 */
    /*0x03*/ int8_t        rawStickY;       /* -80 <= stick_y <= 80 */
    /*0x04*/ uint8_t       errno;
} OSContPad;

typedef struct
{
    /*0x00*/ int16_t       rawStickX;       //
    /*0x02*/ int16_t       rawStickY;       //
    /*0x04*/ float         stickX;          // [-64, 64] positive is right
    /*0x08*/ float         stickY;          // [-64, 64] positive is up
    /*0x0C*/ float         stickMag;        // distance from center [0, 64]
    /*0x10*/ uint16_t      buttonDown;
    /*0x12*/ uint16_t      buttonPressed;
    /*0x14*/ OSContStatus  *statusData;
    /*0x18*/ OSContPad     *controllerData;
} Controller;

#endif // INPUT_H
