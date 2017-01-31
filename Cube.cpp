/*
 * Cube.cpp
 *
 *  Created on: 30.03.2016
 *      Author: johannes
 */

#include "Cube.h"
#include "BitHelper.h"

Cube::Cube() {
	for (int i = 0; i < maxPerLayer; ++i) {
		pinMode(positions[i], OUTPUT);
	}
	for (int i = 0; i < maxLayer; ++i) {
		pinMode(layers[i], OUTPUT);
	}
}

Cube::~Cube() {

}

void Cube::loop() {
	processCube();
}

bool Cube::getLedState(int layer, int pos) {
	if (layer >= 0 && layer < getMaxLayer() && pos >= 0 && pos < getMaxPerLayer()) {
		return leds[layer][pos];
	}
	return false;
}

void Cube::toggleLed(int layer, int pos) {
	if (leds[layer][pos] == HIGH) {
		leds[layer][pos] = LOW;
	} else if (leds[layer][pos] == LOW) {
		leds[layer][pos] = HIGH;
	}
}

void Cube::toggleLayer(int layer) {
	for (int i=0; i < maxPerLayer; i++) {
		toggleLed(layer, i);
	}
}

void Cube::setLayer(int layer, int state) {
	digitalWrite(layers[layer], state);
}

void Cube::setLed(int state) {
	for (int i = 0; i < maxLayer; ++i) {
		setLed(i, state);
	}
}

void Cube::setLed(int layer, int pos, int state) {
	leds[layer][pos] = state;
}

void Cube::setLed(int layer, int state) {
	for (int i = 0; i < maxPerLayer; ++i) {
		setLed(layer, i, state);
	}
}

void Cube::setPos(int pos, int state) {
	digitalWrite(positions[pos], state);
}

byte Cube::getMaxLayer() {
	return maxLayer;
}

byte Cube::getMaxPerLayer() {
	return maxPerLayer;
}

void Cube::processCube() {
	for (int i = 0; i < maxLayer; ++i) {
		setLayer(i, HIGH);

		for (int j = 0; j < maxPerLayer; ++j) {
			setPos(j, leds[i][j]);
		}
	//	delay(time);
		for (int j = 0; j < maxPerLayer; ++j) {
			setPos(j, LOW);
		}
		setLayer(i, LOW);
	}
}

void Cube::setFromDWord(unsigned long dword) {
	int layer = -1;
	int i;
	dword = dword & 134217727; // remove most Left five Bits
	for (i = 0; i < 32; ++i) {
		byte pos = i % 9;
		if (pos == 0) {
			layer++;
		}
		if (layer < maxLayer) {
			leds[layer][pos] = BitHelper::getBitAt(dword, i);
		}
	}

}
