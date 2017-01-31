/*
 * Ir.h
 *
 *  Created on: 30.03.2016
 *      Author: johannes
 */

#ifndef IRCONNECT_H_
#define IRCONNECT_H_
#include "IRremote.h"
#include "Cube.h"
#include "EditMode.h"
#include "Animation.h"

class IrConnect {
public:
	IrConnect(int irSensor, EditMode *editMode, Cube *cube, Animation *animation);
	virtual ~IrConnect();

	void loop();


private:
	int ir;
	bool codeIsNew;
	IRrecv *irrecv;
	decode_results *results;
	decode_results *lastCode;
	EditMode *editMode;
	Cube *cube;
	Animation *animation;

	void processIr();
};

#endif /* IRCONNECT_H_ */
