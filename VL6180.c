/*
 Adley Gin
 ECE372
 VL6180.c
 */

#include "bitbang.h"
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
    //reset=1;
    if(reset==1)
    {
    // Mandatory : private registers
    setRegister(0x01,0x0207);
    setRegister(0x01,0x0208);
    setRegister(0x00,0x0096);
    setRegister(0xfd,0x0097);
    setRegister(0x00,0x00e3);
    setRegister(0x04,0x00e4);
    setRegister(0x02,0x00e5);
    setRegister(0x01,0x00e6);
    setRegister(0x03,0x00e7);
    setRegister(0x02,0x00f5);
    setRegister(0x05,0x00d9);
    setRegister(0xce,0x00db);
    setRegister(0x03,0x00dc);
    setRegister(0xf8,0x00dd);
    setRegister(0x00,0x009f);
    setRegister(0x3c,0x00a3);
    setRegister(0x00,0x00b7);
    setRegister(0x3c,0x00bb);
    setRegister(0x09,0x00b2);
    setRegister(0x09,0x00ca);
    setRegister(0x01,0x0198);
    setRegister(0x17,0x01b0);
    setRegister(0x00,0x01ad);
    setRegister(0x05,0x00ff);
    setRegister(0x05,0x0100);
    setRegister(0x05,0x0199);
    setRegister(0x1b,0x01a6);
    setRegister(0x3e,0x01ac);
    setRegister(0x1f,0x01a7);
    setRegister(0x00,0x0030);
    
    //setRegister(0x0004,SYSTEM_INTERRUPT_CONFIG_GPIO);
    //setRegister(0x0020,SYSTEM_MODE_GPIO1);    //maybe send 0x10 rather than 0x30
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

    I2C2CONbits.ACKDT=0;
    I2C2CONbits.SEN=1;  //start bit
    while(I2C2CONbits.SEN);
    delayUs(2);

    sendI2C(0x52);  //send 7-bit I2C address and write byte

    byte=(regAddress & 0xFF00) >> 8;    //send high byte of 16-bit index of register.
    sendI2C(byte);

    byte=(regAddress & 0x00FF);    //send lower byte of 16-bit index of register.
    sendI2C(byte);

    sendI2C(data);    //send 8-bits of data to register

    I2C2CONbits.PEN=1;    //stop
    reset_i2c_bus();

    return;
}

int getRegister(int regAddress)
{
    char byte;
    int c=0;

    I2C2CONbits.ACKDT=0;
    I2C2CONbits.SEN=1;
    while(I2C2CONbits.SEN);
    delayUs(2);
    
    sendI2C(0x52);    //write

    byte=(regAddress & 0xFF00) >> 8;    //send high byte of 16-bit index of register.
    sendI2C(byte);

    byte=(regAddress & 0x00FF);    //send lower byte of 16-bit index of register.
    sendI2C(byte);

    I2C2CONbits.RSEN=1;  //stop, start condition to start reading
    while(I2C2CONbits.RSEN==1);
    
    sendI2C(0x53);    //send 7-bit I2C address and read bit

    I2C2CONbits.RCEN=1;

    c=receiveI2C();

    I2C2CONbits.PEN=1;
    reset_i2c_bus();

    return c;

}