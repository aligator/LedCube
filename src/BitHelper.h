/*
 * BitHelper.h
 *
 *  Created on: 16.04.2016
 *      Author: johannes
 */

#ifndef BITHELPER_H_
#define BITHELPER_H_

#include "Animation.h"

class BitHelper {
public:
	static bool getBitAt(unsigned long dword, int pos);
	static bool getBitAt(byte data, int pos);

};

#endif /* BITHELPER_H_ */
