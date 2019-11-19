/*
 * Dice.h
 *
 *  Created on: 11.06.2016
 *      Author: johannes
 */

#ifndef DICE_H_
#define DICE_H_

#include "Arduino.h"
#include "Cube.h"

class Dice {
public:
	static void roll(Cube *cube);
};



#endif /* DICE_H_ */
