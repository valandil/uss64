/// Taken from sm64_source.
#ifndef INPUT_USS64_H
#define INPUT_USS64_H

#include <stdint.h>

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
