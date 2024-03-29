/*
 * Animation.cpp
 *
 *  Created on: 14.04.2016
 *      Author: johannes
 */

#include "Animation.h"
#include "BitHelper.h"


Animation::Animation(EepromHandler *eeprom, Cube *cube) {
	this->eeprom = eeprom;
	this->cube = cube;
	this->size = 0;
	this->enabled = false;
	this->timer = 0;
	this->current = 0;
	this->currentAnimation = 0;
	this->timeDemoEnabled = false;
	speedDefault();
}

Animation::~Animation() {

}

unsigned long Animation::combineToDWord(word word1, word word2) {
	return ((unsigned long)word2 | ((unsigned long)word1 << 16));
}

unsigned long Animation::combineToDWord(byte byte1, byte byte2, byte byte3, byte byte4) {
	return combineToDWord((unsigned long)byte2 | ((unsigned long)byte1  << 8), (unsigned long)byte4 | ((unsigned long)byte3 << 8));
}

void Animation::addPicture(unsigned long dword) {
	if (size + 1 <= MAX) {
		leds[size] = dword;
		size++;
	}
}

bool Animation::getIsLast(byte data) {
	return BitHelper::getBitAt(data, 7);
}

bool Animation::getIsLast(unsigned long  data) {
	return BitHelper::getBitAt(data, 31);
}

unsigned int Animation::getTime(unsigned long dword) {
	return (getRawTime(dword) + 1) * animationSpeed;
}

byte Animation::getRawTime(unsigned long dword) {
	return (byte)(((dword >> 27) & 7)); // get time lie it is written into the EEPROM 
}

bool Animation::isEnabled() {
	return enabled;
}

void Animation::enable(bool enable) {
	enabled = enable;
	if (enable) {
		// load current Animation into RAM
		loadFromEEPROM(currentAnimation);
	}
}


void Animation::loadFromEEPROM(byte pos) {

	currentAnimation = pos;
	size = 0;
	current = 0;

	// MAX * 4  = Maximum size of one complete animation with MAX Pictures
	unsigned int start = pos * MAX * 4; // start of the animation based on pos
	if (start < eeprom->getMax()) {
		byte bytes[4];
		bool end = false;

		while (!end) {
		
			// read each byte of one Picture
			for (int i = 0; i < 4; ++i) {
				bytes[i] = eeprom->readByte(start + i);
			}

			addPicture(combineToDWord(bytes[0], bytes[1], bytes[2], bytes[3]));
			
			// increment start to get to the next Picture
			start += 4;

			
			// if it is the last Picture, or exceeds MAX (or ROM-size) --> stop reading
			if (getIsLast(bytes[0]) || start >= eeprom->getMax() || size >= MAX) {
				end = true;
			}
		}
	}
}

// load Prev animation
void Animation::loadPrev() {
	currentAnimation--;
	if (currentAnimation < 0) {
		currentAnimation = MAXANIMATION -1;
	}

	loadFromEEPROM(currentAnimation);
}

// load Next animation
void Animation::loadNext() {
	currentAnimation++;
	if (currentAnimation > MAXANIMATION -1) {
		currentAnimation = 0;
	}

	loadFromEEPROM(currentAnimation);
}

void Animation::loadPrevPicture() {
	current--;
	if (current < 0) {
		current = size - 1;
	}
	cube->setFromDWord(leds[current]);

}

void Animation::loadFirstPicture() {
	current = 0;
	cube->setFromDWord(leds[current]);
}

void Animation::loadNextPicture() {
	current++;
	if (current >= size) {
		current = 0;
	}

	cube->setFromDWord(leds[current]);
}

void Animation::add() {
	addPicture(0);
}

void Animation::remove() {
	if (size > 1) {
		size--;
	}

	if (current >= size) {
		loadFirstPicture();
	}

}

void Animation::setTime(unsigned long time) {
	time = time & 0b111; // mask only three bits
	time = time << 27; // move them to the right place

	leds[current] = leds[current] & 3355443199; // clear the three bits
	leds[current] = leds[current] | time; // set the bits

	if (!isEnabled() && !timeDemoEnabled) {
		timer = 0;
		timeDemoEnabled = true;
	}
}

void Animation::setTime() {
	unsigned long time = (unsigned long)getRawTime(leds[current]);

	time++;

	if (time > 0b111) {
		time = 0;
	}
	setTime(time);
}

// Toggle One Led
void Animation::setLed(byte layer, byte pos) {
	if (layer >= 0 && layer < cube->getMaxLayer() && pos >= 0 && pos < cube->getMaxPerLayer()) { // check layer and pos

		// (layer * cube->getMaxPerLayer()) + pos) --> layer, pos to position of bit in dword-Format
		int posInDword = ((layer * cube->getMaxPerLayer()) + pos);
		leds[current] ^= (-cube->getLedState(layer, pos) ^ leds[current]) & (1L << posInDword); // toggle LED
	}

}

// Set all Leds
void Animation::setLed(bool state) {
	if (state) {
		leds[current] = leds[current] | 0x7FFFFFF; //set all on
	} else {
		leds[current] = leds[current] & 0x70000000; //set all off
	}
}

void Animation::speedUp() {
	animationSpeed -= 10;
	checkSpeed();
}

void Animation::speedDown() {
	animationSpeed += 10;
	checkSpeed();
}

void Animation::speedDefault() {
	animationSpeed = 200;
}

void Animation::checkSpeed() {
	if (animationSpeed < 50) {
		animationSpeed = 50;
	}

	if (animationSpeed > 500) {
		animationSpeed = 500;
	}
}

void Animation::save() {
	// for each Picture in Animation
	for (int i = 0; i < size; ++i) {
		unsigned long data = leds[i];
	
		// set the MSB to one if it is the Last Picture
		if (i == size - 1) {
			data = data | 0x80000000;
		} else {
			data = data & 0x7FFFFFFF;
		}

		// ((Current animation) * (MAX size of one animation)) + (current Picture * (size of one Picture))
		unsigned int start = (currentAnimation * MAX * 4) + i * 4; 
		int count = 0;
		for (int j = 3; j >= 0; --j) {
			byte dataByte = data >> j*8; // get 1 Byte of the Dword
			eeprom->write(start + count, dataByte, true); // write this Byte

			count++;
		}
	}
}

void Animation::loop() {
	// do Animation only when Animation-Mode is on
	if (enabled) {
		if (timer == 0) {
			cube->setFromDWord(leds[current]);
			timer++;
		} else if (timer > getTime(leds[current])) {
			timer = 0;
			if (current+1 < size) {
				current++;
			} else {
				current = 0;
			}

		} else {
			timer++;
		}
	} else if (timeDemoEnabled) { // do timeDemo
		if (timer == 0) {
			for (int i = 0; i < cube->getMaxLayer(); ++i) {
				cube->toggleLayer(i);
			}

			timer++;
		} else if (timer > getTime(leds[current])) {
			timer = 0;

			for (int i = 0; i < cube->getMaxLayer(); ++i) {
				cube->toggleLayer(i);
			}
			timeDemoEnabled = false;
		} else {
			timer++;
		}
	}

}
