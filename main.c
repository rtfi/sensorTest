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

    while(1)
    {
        //range=get_data(0x29,0x0016);
        //delayUs(1000);
        setRegister(0x01,0x0018);
        delayUs(5000);
        range=getRange();
        turnLEDON();
    }

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
