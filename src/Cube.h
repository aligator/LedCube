/*
 * Cube.h
 *
 *  Created on: 30.03.2016
 *      Author: johannes
 */

#ifndef CUBE_H_
#define CUBE_H_
#include "Arduino.h"

class Cube {
public:
	Cube();
	virtual ~Cube();

	void loop();
	void toggleLed(int layer, int pos);
	void toggleLayer(int layer);
	void setLed(int state);
	void setLed(int layer, int state);
	void setLed(int layer, int pos, int state);
	void setFromDWord(unsigned long dword);
	byte getMaxLayer();
	byte getMaxPerLayer();
	bool getLedState(int layer, int pos);

private:
	// byte positions[9] = {5, 6, 7,
						//	8, 9, 10,
						//	11, 12, 13};
	byte positions[9] = {11, 8, 5,
						 12, 9, 6,
						 13, 10, 7};
	byte layers[3] = {2, 3, 4};
	void processCube();
	void setLayer(int layer, int state);
	void setPos(int pos, int state);

	byte maxLayer = 3;
	byte maxPerLayer = 9;
	bool leds[3][9] = {	{	LOW, LOW, LOW,
							LOW, LOW, LOW,
							LOW, LOW, LOW},

						{	LOW, LOW, LOW,
							LOW, LOW, LOW,
							LOW, LOW, LOW},

						{	LOW, LOW, LOW,
							LOW, LOW, LOW,
							LOW, LOW, LOW},
						};
};

#endif /* CUBE_H_ */
