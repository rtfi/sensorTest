/*Adley Gin
 ECE 372
 i2c.c
 */
 

#include "p24FJ64GA002.h"
#include "i2c.h"

#define WRITE 0
#define READ 1
#define OUTPUT 0
#define INPUT 1

void initI2C()
{
    //might not be necessary, but we're using pins 17 and 18 as the I2C clock and data pins
    //TRISBbits.TRISB8=OUTPUT;
    //TRISBbits.TRISB9=OUTPUT;

    //No need to set any AD1PCFG bits to digital, because pins 17 and 18 are not analog pins.

    //I2C1CONbits.I2CEN=1;    //Enable I2C. Use I2C1CONbits because pins 17 and 18 are SDA1 and SCL1.
    //IFS1bits.MI2C1IF=0;
    //I2C1BRG = 38;   //I got this value form the I2C baud rate table in the PIC24 datasheet.

    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0; // probbably not necessary
    AD1PCFGbits.PCFG4 = 1;
    AD1PCFGbits.PCFG5 = 1;

    I2C2CONbits.I2CEN = 1;
    IFS3bits.MI2C2IF = 0;
    I2C2BRG = 10000;
}

void sendI2C(char data)
{
    int testAck=0;
    //master will raise interrupt flag once start condition is complete.
   
    while(I2C2CONbits.SEN==1);
    //IFS3bits.MI2C2IF=0;
    
    //load data into transmit register
    I2C2TRN = data;

    //wait while transmitting data
    while(I2C2STATbits.TRSTAT == 1);

    //ACKSTAT = 0 means acknowledged.
    if(I2C2STATbits.ACKSTAT==0);
    {
        testAck=1;
    }
    
    //while(IFS3bits.MI2C2IF == 0);
    //IFS3bits.MI2C2IF=0;
}

int receiveI2C()
{
    int c;

    //while interrupt flag for master is down
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF=0;

    
    //wait until i2c receiver buffer is full
    while(!I2C2STATbits.RBF);
    //I2C2STATbits.RBF=0;

    c=I2C2RCV;

    I2C2CONbits.ACKDT = 0; //0 ACK or 1 for NACK
    I2C2CONbits.ACKEN = 1; //enable acknowledge

    //while(IFS1bits.MI2C1IF == 0);
    //IFS1bits.MI2C1IF=0;
   
    return c;
    
}