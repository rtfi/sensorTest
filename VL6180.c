#include "i2c.h"
#include "p24FJ64GA002.h"
#include "VL6180.h"

#define I2CADDRESS 0x0029
#define SYS_INTERRUPT_CONFIG_GPIO 0x0014
#define SYSRANGE_START 0x0018
#define SYSTEM_MODE_GPIO1 0x0011
#define READOUT_AVERAGING_SAMPLE_PERIOD 0x010A
#define SYSRANGE_MAX_CONVERGENCE_TIME 0x001C
#define SYSRANGE_INTERMEASUREMENT_PERIOD 0x001B

void initSensor()
{
    setRegister(0x0004,SYS_INTERRUPT_CONFIG_GPIO);
    setRegister(0x0030,SYSTEM_MODE_GPIO1);    //maybe send 0x10 rather than 0x30
    setRegister(0x0030,READOUT_AVERAGING_SAMPLE_PERIOD);
    setRegister(0x0032,SYSRANGE_MAX_CONVERGENCE_TIME);
    setRegister(0x0000,SYSRANGE_INTERMEASUREMENT_PERIOD);   //10ms between
    
}

void startRangeFinding()
{
    setRegister(0x0003,SYSRANGE_START);

}

void setRegister(char data, int regAddress)
{
    char byte;

    
    //start bit
    I2C1CONbits.SEN=1;

    //send address and write byte
    sendI2C(0x52);

    //send high byte of 16-bit index of register.
    byte=(regAddress & 0xFF00) >> 8;
    sendI2C(byte);

    //send lower byte of 16-bit index of register.
    byte=(regAddress & 0x00FF);
    sendI2C(byte);

    //send 8-bits of data to register
    sendI2C(data);

    //stop
    I2C1CONbits.PEN=1;
    
}

char getRegister(int regAddress)
{
    char byte;
    char c='c';

    //start bit
    I2C1CONbits.SEN=1;

    //write
    sendI2C(0x52);

    //send high byte of 16-bit index of register.
    byte=(regAddress & 0xFF00) >> 8;
    sendI2C(byte);

    //send lower byte of 16-bit index of register.
    byte=(regAddress & 0x00FF);
    sendI2C(byte);

    //stop, start
    I2C1CONbits.PEN=1;
    I2C1CONbits.SEN=1;

    //read
    sendI2C(0x53);

    c=receiveI2C();

    I2C1CONbits.PEN=1;

    return c;

}