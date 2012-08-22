/*
 * BigDigitDriver.h
 *
 *  Created on: Jul 5, 2011
 *      Author: MichaelEdwards
 */

#ifndef BIGDIGITDRIVER_H_
#define BIGDIGITDRIVER_H_

//**************************************************************//
//  Name    : Femur Measurement Interactive
//  Author  : Cameron Browning, American Museum of Natural History Exhibit Media Dept.
//  Date    : 17 Mar, 2011
//  Modified: 28 Mar 2011
//  Version : 2.0
//  Notes   : Shifts out to Daisy Chained TPIC6C595s on
//          : The AMNH BDD 1.5 Ciruit boards
//****************************************************************

#include <Arduino.h>
#include <WProgram.h>

class BigDigitDriver {
public:
	//Pin connected to ST_CP of 74HC595
	int latchPin;
	//Pin connected to SH_CP of 74HC595
	int clockPin;
	////Pin connected to DS of 74HC595
	int dataPin;

	// is this the measure the dino femur, or visitor's femur?
	// the difference is that there is an empty digit on the
	// measure the visitor side. When reprogramming, swap
	// this value to program the other side.
	boolean isDino;

	int counter;

	int sliderPin;

	int sliderVal;
	int prevSliderVal;

	int lbPin;
	int kgPin;

	int lbVal;
	int kgVal;

	int state;

	boolean prevUse;

	long startWait;
	static const uint32_t TIMEOUT = 5000;

	float sLow;
	float sHigh;
	float dHigh;

	byte dataArray[11];
	int digitArray[9]; //= { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int prevDigits[9]; //= { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	byte decimalByte;
	boolean showDecimal;

	int cmSmoother[5];

	int cmDigits[3];

	int kgDigits[3];

	static const int HISTORYLENGTH = 10;
	static const int FORGIVENESS = 10;

	int sliderHistory[HISTORYLENGTH];

	float prevMassVal;
	float prevLenVal;

	int startDisplayMillis;
	int prevKgVal;
	int prevLbVal;

	int tempDigit;

	int lenVal;

	float lengthVal;
	float prevLengthVal;
	long outputVal;
	boolean useImperial;

	BigDigitDriver(
			int _latchPin,
			int _clockPin,
			int _dataPin,
			int _sliderPin,
			bool _isDino,
			bool _showDecimal
			);
	void updateButtons();
	void updateSlider();
	void measure();
	long calculateMass(boolean useImperial);
	void pushSliderVal(int sliderVal_in);
	boolean checkSlider();
	void startup();
	void setMass(long mass_in);
	void setLength(int length_in);
	void setAll(int val_in);
	void clearMass();
	void clearLength();
	void removeLeadingZeroes();
	void updateDisplays();
	void shiftOutSlow(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder,
			uint8_t val);
	void initHistory();
};

#endif /* BIGDIGITDRIVER_H_ */
