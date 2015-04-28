#include "p24FJ64GA002.h"
#include "i2c.h"
#include "VL6180.h"

int main(void)
{
    initI2C();
    initSensor();

    while(1)
    {
        
    }

    return 0;
}
