/*
 * BigDigitDriver.cpp
 *
 *  Created on: Jul 5, 2011
 *      Author: MichaelEdwards
 */

#include "BigDigitDriver.h"


#include <stdlib.h>

//**************************************************************//
//  Name    : Femur Measurement Interactive
//  Author  : Cameron Browning, American Museum of Natural History Exhibit Media Dept.
//  Date    : 17 Mar, 2011
//  Modified: 28 Mar 2011
//  Version : 2.0
//  Notes   : Shifts out to Daisy Chained TPIC6C595s on
//          : The AMNH BDD 1.5 Ciruit boards
//****************************************************************

BigDigitDriver::BigDigitDriver(
		int _latchPin,
		int _clockPin,
		int _dataPin,
		int _sliderPin,
		bool _isDino,
		bool _showDecimal
		) {
	//Pin connected to ST_CP of 74HC595
	latchPin = _latchPin;
	//Pin connected to SH_CP of 74HC595
	clockPin = _clockPin;
	////Pin connected to DS of 74HC595
	dataPin = _dataPin;

	// is this the measure the dino femur, or visitor's femur?
	// the difference is that there is an empty digit on the
	// measure the visitor side. When reprogramming, swap
	// this value to program the other side.
	isDino = _isDino;

	counter = 0;

	sliderPin = A0;

	sliderVal = 0;
	prevSliderVal = 0;

	lbPin = 2;
	kgPin = 3;

	lbVal = 0;
	kgVal = 0;

	state = -1;

	prevUse = true;

	startWait = 0;
//	uint32_t TIMEOUT = 5000;

	sLow = 0;
	sHigh = 0;
	dHigh = 0;

//	digitArray = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
//	prevDigits = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	decimalByte = B10000000;
	showDecimal = _showDecimal;

//	cmSmoother = { 0, 0, 0, 0, 0 };

	prevMassVal = 0;
	prevLenVal = 0;

	startDisplayMillis = 0;
	prevKgVal = HIGH;
	prevLbVal = HIGH;

	tempDigit = 0;

	lenVal = 0;

	if (!isDino) {
		sLow = 63;
		sHigh = 821;
		dHigh = 99;
	} else {
		sLow = 77;
		sHigh = 959;
		dHigh = 173;
	}

	Serial.begin(9600);
	//set pins to output so you can control the shift register
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);

	pinMode(lbPin, INPUT); // set pin to input
	pinMode(kgPin, INPUT); // set pin to input

	digitalWrite(lbPin, HIGH); // turn on pullup resistors
	digitalWrite(kgPin, HIGH); // turn on pullup resistors

	dataArray[1] = B00000101;
	dataArray[2] = B01011011;
	dataArray[3] = B01001111;
	dataArray[4] = B01100101;
	dataArray[5] = B01101110;
	dataArray[6] = B01111110;
	dataArray[7] = B00000111;
	dataArray[8] = B01111111;
	dataArray[9] = B01101111;
	dataArray[0] = B00111111;
	dataArray[10] = B00000000;

	initHistory();

	useImperial = false;

	//startup();
}

void BigDigitDriver::updateButtons() {
	kgVal = digitalRead(kgPin);
	lbVal = digitalRead(lbPin);
}

void BigDigitDriver::updateSlider() {
	sliderVal = analogRead(A0);
	//if(sliderVal!=sliderHistory[0]){
	pushSliderVal(sliderVal);
	// }
}

void BigDigitDriver::measure() {

	// lengthVal = sliderVal;


	lengthVal = ((float) sliderVal - sLow) / (sHigh - sLow) * dHigh;
	lengthVal = max(lengthVal,0);
	lengthVal = min(lengthVal,sHigh);

	lengthVal = 0.75 * lengthVal + 0.25 * prevLengthVal;
	Serial.print("SETTING MASS TO::: ");
	Serial.println((int) lengthVal, DEC);
	setLength((int) lengthVal);
	prevLengthVal = lengthVal;

}

long BigDigitDriver::calculateMass(boolean useImperial) {
	//calculates mass in kg
	outputVal = ((pow((long) lengthVal, 3.195)) * 0.0016);

	//calculates mass in lb
	if (useImperial == true) {

		outputVal = ((pow((long) lengthVal, 3.195)) * 0.00352739619);
		//setLength((long)lengthVal);
		//prevLengthVal = lengthVal;
	}
	// outputVal *= 10;
	return outputVal;
}

void BigDigitDriver::pushSliderVal(int sliderVal_in) {
	for (int i = (HISTORYLENGTH - 1); i > 0; i--) {
		sliderHistory[i] = sliderHistory[i - 1];
	}
	sliderHistory[0] = sliderVal_in;
}

boolean BigDigitDriver::checkSlider() {
	int loVal = 1024;
	int hiVal = 0;
	for (int i = 0; i < HISTORYLENGTH; i++) {
		if (sliderHistory[i] > hiVal)
			hiVal = sliderHistory[i];
		if (sliderHistory[i] < loVal)
			loVal = sliderHistory[i];
	}

	if ((hiVal - loVal) > FORGIVENESS) {
		/*
		 Serial.println("******************************************************************************************");
		 Serial.println("******************************* IN USE ***************************************************");
		 Serial.print("******************************   ");
		 Serial.print((hiVal-loVal),DEC);
		 Serial.println("   ****************************************");

		 for(int i=0;i<HISTORYLENGTH;i++){
		 Serial.print(sliderHistory[i],DEC);
		 if(i<HISTORYLENGTH-1){
		 Serial.print(",");
		 }
		 else {
		 Serial.println(".");
		 }

		 }

		 Serial.println("******************************************************************************************");
		 */
		startWait = millis();
		return true;
	} else {
		/*
		 Serial.println();
		 Serial.println(" --- --- --- NOT IN USE --- --- --- ");
		 Serial.print((hiVal-loVal),DEC);
		 Serial.println("   ****************************************");

		 for(int i=0;i<HISTORYLENGTH;i++){
		 Serial.print(sliderHistory[i],DEC);
		 if(i<HISTORYLENGTH-1){
		 Serial.print(",");
		 }
		 else {
		 Serial.println(".");
		 }

		 }

		 Serial.println("******************************************************************************************");
		 Serial.println();
		 */
		return false;
	}
}

void BigDigitDriver::startup() {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (j <= i) {
				digitArray[j] = 1;
			} else {
				digitArray[j] = 10;
			}
		}
		delay(100);
		updateDisplays();
	}
	setAll(8);
	updateDisplays();
	setAll(10);
	updateDisplays();
	delay(200);
	setAll(8);
	updateDisplays();
	delay(200);
	setAll(10);
	updateDisplays();
	delay(200);
}

void BigDigitDriver::setMass(long mass_in) {
	mass_in *= 10; // fix because in the gallery we drop the last digit from the boards
	mass_in = max(mass_in, 0);
	long factor = 10;
	long runningTotal = 0;
	long tempLong = 0;
	for (int i = 0; i < 6; i++) {
		tempLong = mass_in % factor;
		tempLong -= runningTotal;
		runningTotal += tempLong;
		factor *= 10;
		tempLong /= pow(10, i);
		digitArray[3 + 5 - i] = (int) tempLong;
	}

}

void BigDigitDriver::setLength(int length_in) {
	if (!isDino) {
		length_in *= 10;
	}
	length_in = max(length_in, 0);
	int factor = 10;
	int runningTotal = 0;
	for (int i = 0; i < 3; i++) {
		digitArray[2 - i] = length_in % factor;
		digitArray[2 - i] -= runningTotal;
		runningTotal += digitArray[2 - i];
		factor *= 10;
	}
	for (int i = 0; i < 3; i++) {
		digitArray[2 - i] /= pow(10, i);
	}
}

void BigDigitDriver::setAll(int val_in) {
	updateSlider();
	for (int i = 0; i < 9; i++) {
		digitArray[i] = val_in;
	}
	for (int i = 0; i < HISTORYLENGTH; i++) {
		sliderHistory[i] = sliderVal;
	}
}

void BigDigitDriver::clearMass() {
	for (int i = 3; i < 9; i++) {
		digitArray[i] = 10;
	}
}

void BigDigitDriver::clearLength() {

	for (int i = 0; i < 3; i++) {
		digitArray[i] = 10;
	}

}

void BigDigitDriver::removeLeadingZeroes() {

	int totalDigs = 9;
	if (isDino) {
		totalDigs = 8;
	} else {
		totalDigs = 7;
	}

	for (int i = 0; i < (totalDigs - 6); i++) {
		// loop through length
		if (digitArray[i] == 0) {
			digitArray[i] += 10;
		} else {
			break;
		}

	}

	for (int i = 3; i < 7; i++) {
		// loop through mass
		if (digitArray[i] == 0) {
			digitArray[i] += 10;
		} else {
			break;
		}
	}
}

void BigDigitDriver::updateDisplays() {

	// check for new info
	boolean isUpdated = false;
	for (int i = 0; i < 9; i++) {
		if (prevDigits[i] != digitArray[i]) {
			isUpdated = true;
			break;
		}
	}
	removeLeadingZeroes();

	if (isUpdated) {
		// Serial.println("UPDATING DISPLAY");

		//////////////////////////////////////////////////////
		// WRITE TO THE BDD BOARDS ///////////////////////////
		//////////////////////////////////////////////////////
		digitalWrite(latchPin, LOW);
		delay(1);

		// first size bytes (mass value)
		for (int i = 0; i < 9; i++) {
			shiftOut(dataPin, clockPin, MSBFIRST, dataArray[digitArray[i]]);
			Serial.print(digitArray[i]);
			Serial.print(" ");
			prevDigits[i] = digitArray[i];
		}
		Serial.println();
		digitalWrite(latchPin, HIGH);
		delay(1);
		//////////////////////////////////////////////////////

	}
}

void BigDigitDriver::shiftOutSlow(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder,
		uint8_t val) {
	for (int i = 0; i < 8; i++) {
		if (bitOrder == LSBFIRST) {
			digitalWrite(dataPin, !!(val & (1 << i)));
		} else {
			digitalWrite(dataPin, !!(val & (1 << (7 - i))));
		}
		delay(1);
		digitalWrite(clockPin, HIGH);
		delay(1);
		digitalWrite(clockPin, LOW);
		delay(1);
	}

}

void BigDigitDriver::initHistory() {
	for (int i = 0; i < HISTORYLENGTH; i++) {
		sliderHistory[i] = 0;
	}
}

