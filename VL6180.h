/* 
 * File:   VL6180.h
 * Author: Adley
 *
 * Created on April 26, 2015, 4:23 PM
 */

#ifndef VL6180_H
#define	VL6180_H

void initSensor();
void startRangeFinding();
int getRange();
void pollRange();
void clearInterrupts();

void setRegister(char data, int regAddress);
int getRegister(int regAddress);

#endif	/* VL6180_H */

