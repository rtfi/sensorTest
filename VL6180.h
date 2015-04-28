/* 
 * File:   VL6180.h
 * Author: Adley
 *
 * Created on April 26, 2015, 4:23 PM
 */

#ifndef VL6180_H
#define	VL6180_H

char getRegister(int regAddress);
void setRegister(char data, int regAddress);

void initSensor();
void startRangeFinding();


#endif	/* VL6180_H */

