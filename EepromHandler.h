/*
 * EepromHandler.h
 *
 *  Created on: 14.04.2016
 *      Author: johannes
 */

#ifndef EEPROMHANDLER_H_
#define EEPROMHANDLER_H_
#include "Arduino.h"

class EepromHandler {
public:
	EepromHandler();
	virtual ~EepromHandler();
	void loop();
	uint16_t getMax();
	byte readByte(int pos);
	void write(uint16_t addr, byte data, bool check);

private:
	void eraseComplete();
	void readComplete();
	void erase(uint16_t addr);
	void erase(uint16_t from, uint16_t to);

};

#endif /* EEPROMHANDLER_H_ */
