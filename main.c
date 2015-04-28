#include "p24FJ64GA002.h"
#include "i2c.h"
#include "VL6180.h"
#include "timer.h"
#include "ledandpins.h"

int main(void)
{
    int range=0;

    initLED();
    initI2C();
    initSensor();
    startRangeFinding();

    while(1)
    {
        pollRange();
        range=getRange();
        clearInterrupts();
        if(range>50)
        {
            turnLEDOFF();
        }
        else if(range<=50)
            turnLEDON();
    }

    return 0;
}
