/*
 * Animation.h
 *
 *  Created on: 14.04.2016
 *      Author: johannes
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#define MAX 32
#define MAXANIMATION 8


#include "Arduino.h"
#include "EepromHandler.h"
#include "Cube.h"

class Animation {
public:
	Animation(EepromHandler *eeprom, Cube *cube);
	virtual ~Animation();
	unsigned long combineToDWord(word word1, word word2);
	unsigned long combineToDWord(byte byte1, byte byte2, byte byte3, byte byte4);
	bool isEnabled();
	void loadFromEEPROM(byte pos);
	void loop();
	void enable(bool enable);
	void loadPrev();
	void loadNext();
	void add();
	void remove();
	void loadPrevPicture();
	void loadFirstPicture();
	void loadNextPicture();
	void setLed(byte layer, byte pos);
	void setLed(bool state);
	void save();
	void setTime();
	void speedUp();
	void speedDown();
	void speedDefault();

private:
	void addPicture(unsigned long dword);
	bool getIsLast(byte data);
	bool getIsLast(unsigned long  data);
	unsigned int getTime(unsigned long dword);
	byte getRawTime(unsigned long dword);
	void setTime(unsigned long  time);
	void checkSpeed();

	EepromHandler *eeprom;
	Cube *cube;
	unsigned long leds[MAX];
	byte size;
	unsigned int timer;
	bool enabled;
	int current;
	int currentAnimation;
	bool timeDemoEnabled;
	int animationSpeed;
};

#endif /* ANIMATION_H_ */
