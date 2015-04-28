/*
 Adley Gin
 ECE372
 VL6180.c
 */

#include "i2c.h"
#include "p24FJ64GA002.h"
#include "VL6180.h"
#include "timer.h"

#define I2CADDRESS 0x0029
#define READOUT_AVERAGING_SAMPLE_PERIOD 0x010A
#define RESULT_INTERRUPT_STATUS_GPIO 0x004F
#define RESULT_RANGE_VAL 0x0062
#define SYSRANGE_START 0x0018
#define SYSRANGE_MAX_CONVERGENCE_TIME 0x001C
#define SYSRANGE_INTERMEASUREMENT_PERIOD 0x001B
#define SYSTEM_FRESH_OUT_OF_RESET 0x0016
#define SYSTEM_INTERRUPT_CONFIG_GPIO 0x0014
#define SYSTEM_INTERRUPT_CLEAR 0x0015
#define SYSTEM_MODE_GPIO1 0x0011


#define SYSTEM_FRESH_OUT_OF_RESET 0x0016
#define SYSTEM_INTERRUPT_CLEAR 0x0015

void initSensor()
{
    char reset;
    reset=getRegister(SYSTEM_FRESH_OUT_OF_RESET);

    if(reset==1)
    {

    setRegister(0x0004,SYSTEM_INTERRUPT_CONFIG_GPIO);
    setRegister(0x0020,SYSTEM_MODE_GPIO1);    //maybe send 0x10 rather than 0x30
    setRegister(0x0030,READOUT_AVERAGING_SAMPLE_PERIOD);    //4.3ms
    setRegister(0x0032,SYSRANGE_MAX_CONVERGENCE_TIME);  // 50ms
    setRegister(0x0006,SYSRANGE_INTERMEASUREMENT_PERIOD);   //70ms

    setRegister(0x07,SYSTEM_FRESH_OUT_OF_RESET);    //clear reset flag.
    }
    return;
}

void startRangeFinding()
{
    setRegister(0x0003,SYSRANGE_START);
    
    return;
}

int getRange()
{
    int range;

    range=getRegister(RESULT_RANGE_VAL);

    return range;
}

void pollRange()
{
    char status;
    char range_status;

    status=getRegister(RESULT_INTERRUPT_STATUS_GPIO);   //raw status value in hex from 8-bit register
    range_status=status & 0x07; // this gets us the lower three bits we want to look at.

    //0x04 means that there is a new sample ready.
    while(range_status!=0x04)
    {
        status=getRegister(RESULT_INTERRUPT_STATUS_GPIO);
        range_status=status & 0x07;
        delayUs(1000);  //wait 1ms before checking for status again.
    }
    return;
}

void clearInterrupts()
{
    setRegister(0x07,SYSTEM_INTERRUPT_CLEAR);

    return;
}

void setRegister(char data, int regAddress)
{
    char byte;

    I2C1CONbits.SEN=1;  //start bit

    sendI2C(0x52);  //send 7-bit I2C address and write byte

    byte=(regAddress & 0xFF00) >> 8;    //send high byte of 16-bit index of register.
    sendI2C(byte);

    byte=(regAddress & 0x00FF);    //send lower byte of 16-bit index of register.
    sendI2C(byte);

    sendI2C(data);    //send 8-bits of data to register

    I2C1CONbits.PEN=1;    //stop

    return;
}

char getRegister(int regAddress)
{
    char byte;
    char c='c';

    I2C1CONbits.SEN=1;    //start bit

    sendI2C(0x52);    //write

    byte=(regAddress & 0xFF00) >> 8;    //send high byte of 16-bit index of register.
    sendI2C(byte);

    byte=(regAddress & 0x00FF);    //send lower byte of 16-bit index of register.
    sendI2C(byte);

    I2C1CONbits.RSEN=1;  //stop, start condition to start reading
    
    sendI2C(0x53);    //send 7-bit I2C address and read bit

    I2C1CONbits.RCEN=1;

    c=receiveI2C();

    I2C1CONbits.PEN=1;

    return c;

}