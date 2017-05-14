/* Controller */

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* hardware platform libraries */
#include <mbed.h>

/* asteroids */
#include "model.h"
#include "asteroids.h"

/* Joystick 5-way switch
*/
////////VARIABLES////////////////////////////////
typedef enum {JLT, JRT, JUP, JDN, JCR} btnId_t;
static DigitalIn jsBtns[] = {P5_0, P5_4, P5_2, P5_1, P5_3}; // LFT, RGHT, UP, DWN, CTR
bool jsPrsdAndRlsd(btnId_t b);
bool joyStickUp;
bool joyStickDown;
bool joyStickRight;
bool joyStickLeft;
bool joyStickCenter;
bool toggle;
//////////////////////////////////////////////////////////
/////CHECKS AND TRANSMITS CURRENT POSITION OF JOYSTICK////
//////////////////////////////////////////////////////////
void controls(void)
{
    if (jsPrsdAndRlsd(JDN)) {
        joyStickDown = true;
        joyStickUp = false;
    }
    if (jsPrsdAndRlsd(JUP)) {
        joyStickUp = true;
        joyStickDown = false;
        joyStickRight = false;
        joyStickLeft = false;
    }
    if (jsPrsdAndRlsd(JRT)) {
        joyStickLeft = false;
        joyStickRight = true;
    }
    if (jsPrsdAndRlsd(JLT)) {
        joyStickLeft = true;
        joyStickRight = false;
    }
    if (jsPrsdAndRlsd(JCR)) {
        joyStickCenter = true;
    }
}
//////////////////////////////////////////////////////////
///////RECOGNISES IF JOYSTICK IS PRESSED//////////////////
//////////////////////////////////////////////////////////
bool jsPrsdAndRlsd(btnId_t b) {
    bool result = false;
    uint32_t state;
    static uint32_t savedState[4] = {1,1,1,1};
    state = jsBtns[b].read();
    if ((savedState[b] == 0) && (state == 1)) {
        result = true;
    }
    
    savedState[b] = state;
    return result;
}
