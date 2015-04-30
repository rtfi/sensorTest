/* 
 * File:   VL6180.h
 * Author: Adley
 *
 * Created on April 26, 2015, 4:23 PM
 */

#ifndef VL6180_H
#define	VL6180_H

int getRegister(int regAddress);
void setRegister(char data, int regAddress);

void initSensor();
void startRangeFinding();
int getRange();
void pollRange();
void clearInterrupts();

#endif	/* VL6180_H */

