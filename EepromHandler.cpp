/*
r * EepromHandler.cpp
 *
 *  Created on: 14.04.2016
 *      Author: johannes
 */

#include "EepromHandler.h"
#include "EEPROM.h"

EepromHandler::EepromHandler() {

}

EepromHandler::~EepromHandler() {

}

int freeRam() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void EepromHandler::loop() {
	 if (Serial.available() > 0) {
		 byte incoming = Serial.read();

		 Serial.println(incoming);

		 switch (incoming) {
			case 67: // C
				eraseComplete();
				break;
			case 114: // r
				readComplete();
				break;
			case 119: // w
				for (int i = 0; i < 8; ++i) {
					int start = 128 * i;
					write(start, 0b10000000, true);
				}
				//write(0, 0b00011111, true);

				break;
			case 69: // E
				erase(0);
			case 113: // q
				Serial.println(freeRam());
				break;
			default:
				break;
		}

	 }
}

void EepromHandler::eraseComplete() {
	Serial.println("Erasing complete EEPROM");

	for (uint16_t i = 0 ; i < EEPROM.length() ; i++) {
		EEPROM.write(i, 0);
	}
	Serial.println("EEPROM completely erased");
}

byte EepromHandler::readByte(int pos) {

	return EEPROM.read(pos);

}

void EepromHandler::erase(uint16_t from, uint16_t to) {
	if (from >= 0 && from < EEPROM.length() && to >= 0 && to < EEPROM.length()) {
		Serial.println("Erasing EEPROM");
		Serial.print("From: ");
		Serial.println(from);
		Serial.print("To: ");
		Serial.println(to);

		for (uint16_t i = from ; i <= to ; i++) {
			EEPROM.write(i, 0);
		}
		Serial.println("EEPROM erased");
	}
}

void EepromHandler::erase(uint16_t addr) {
	Serial.println("Erasing EEPROM");
	Serial.print("Adress:");
	Serial.println(addr);

	EEPROM.write(addr, 0);

	Serial.println("EEPROM erased");
}

void EepromHandler::readComplete() {
	Serial.println("EEPROM-Dump:");

	byte value;

	for (uint16_t i = 0 ; i < EEPROM.length() ; i++) {
		value = EEPROM.read(i);
		Serial.print(i);
		Serial.print("\t");
		Serial.print(value, DEC);
		Serial.println();
	}
	Serial.println("EEPROM-Dump end");
}

void EepromHandler::write(uint16_t addr, byte data, bool chek) {
	Serial.println("Write to EEPROM");
	Serial.print("Adress: ");
	Serial.print(addr);
	Serial.print("\nData: ");
	Serial.print(data);
	Serial.println();
	bool ok = true;

	if (chek) {  // only write if not already correct data
		if (readByte(addr) == data) {
			ok = false;
		}
	}


	if (ok) {
		EEPROM.write(addr, data);
	} else {
		Serial.println("Data not written. It was already there.");
	}


	Serial.println("Write to EEPROM end");
}

uint16_t EepromHandler::getMax() {
	return EEPROM.length();
}
