/*
 * BitHelper.cpp
 *
 *  Created on: 16.04.2016
 *      Author: johannes
 */

#include "BitHelper.h"

bool BitHelper::getBitAt(unsigned long data, int pos) {
	return (bool)((data >> pos) & 1);
}

bool BitHelper::getBitAt(byte data, int pos) {
	return getBitAt((unsigned long)data, pos);
}
