//#include <WProgram.h>
#include "BigDigitDriver.h"
#include <stdlib.h>

extern "C" void __cxa_pure_virtual()
{
  cli();
  for (;;);
}

__extension__ typedef int __guard __attribute__((mode (__DI__)));

void * operator new(size_t size)
{
  return malloc(size);
}

void operator delete(void * ptr)
{
  free(ptr);
}

void * operator new[](size_t size)
{
    return malloc(size);
}

void operator delete[](void * ptr)
{
	if (ptr)
		free(ptr);
}

int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);}
void __cxa_guard_release (__guard *g) {*(char *)g = 1;}
void __cxa_guard_abort (__guard *) {}

BigDigitDriver bdd(4,5,6,A0,false,true);

int main(void) {
	init();

	setup();

	for (;;)
		loop();

	return 0;
}

//**************************************************************//
//  Name    : Femur Measurement Interactive
//  Author  : Cameron Browning, American Museum of Natural History Exhibit Media Dept.
//  Date    : 17 Mar, 2011
//  Modified: 28 Mar 2011
//  Version : 2.0
//  Notes   : Shifts out to Daisy Chained TPIC6C595s on
//          : The AMNH BDD 1.5 Ciruit boards
//****************************************************************

void setup() {
	bdd.startup();
}

void loop() {

	digitalWrite(13,HIGH);
	delay(200);
	digitalWrite(13,LOW);
	delay(200);
	digitalWrite(13,HIGH);
	delay(100);
	digitalWrite(13,LOW);
	delay(100);

//	bdd.updateSlider();
//	bdd.updateButtons();
//
//	switch (bdd.state) {
//	case -1:
//		// startup animation
//		bdd.startup();
//		bdd.state = 0;
//		break;
//	case 0:
//		// sleep state
//		if (bdd.kgVal == LOW) {
//			bdd.state = 2;
//		}
//		if (bdd.lbVal == LOW) {
//			bdd.state = 3;
//		}
//
//		bdd.updateDisplays();
//
//		if (bdd.checkSlider()) {
//			Serial.println("TRUE");
//			bdd.state = 1;
//		} else {
//			Serial.println("FALSE");
//			bdd.setAll(10);
//			delay(50);
//		}
//
//		break;
//	case 1:
//		// measuring...
//		bdd.updateSlider();
//		bdd.checkSlider();
//		bdd.measure();
//		bdd.updateDisplays();
//		if ((millis() - bdd.startWait) > bdd.TIMEOUT) {
//			bdd.state = 0;
//		}
//		// display live measurement value
//
//		if (bdd.kgVal == LOW) {
//			bdd.state = 2;
//		}
//		if (bdd.lbVal == LOW) {
//			bdd.state = 3;
//		}
//
//		break;
//	case 2:
//		bdd.updateSlider();
//		bdd.checkSlider();
//		bdd.measure();
//		bdd.updateButtons();
//		bdd.updateDisplays();
//
//		Serial.println("measuring");
//		//  setMass(calculateMass());
//		bdd.setMass(bdd.calculateMass(false));
//		Serial.print("THIS IS MASS------>>>>>>");
//		Serial.println(bdd.calculateMass(false), DEC);
//		Serial.println("CALCULATING MASS");
//
//		bdd.updateDisplays();
//		/*
//		 Serial.println();
//		 Serial.println("*************************************************************************************");
//		 Serial.println("*********************              UPDATING DISPLAY              ********************");
//		 Serial.println("*************************************************************************************");
//		 Serial.println();
//		 */
//		delay(5000);
//		bdd.clearMass();
//		bdd.state = 1;
//		break;
//	case 3:
//		bdd.updateSlider();
//		bdd.checkSlider();
//		bdd.updateButtons();
//		bdd.measure();
//		bdd.updateDisplays();
//
//		bdd.setMass(bdd.calculateMass(true));
//		Serial.print("THIS IS MASS------>>>>>>");
//		Serial.println(bdd.calculateMass(true), DEC);
//		Serial.println("CALCULATING MASS");
//
//		//updateDisplays();
//		bdd.updateDisplays();
//		delay(5000);
//		bdd.clearMass();
//		bdd.state = 1;
//		break;
//	}

	/*
	 clearLength();
	 clearMass();


	 if(digitalRead(kgPin)==LOW){
	 setLength(123);
	 }
	 if(digitalRead(lbPin)==LOW){
	 setMass(analogRead(A0));
	 }

	 */
	// setLength(123);
	// setMass(45678);
	// updateDisplays();


}

