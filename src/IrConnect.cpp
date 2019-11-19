/*
 * Ir.cpp
 *
 *  Created on: 30.03.2016
 *      Author: johannes
 */

#include "IrConnect.h"
#include "Dice.h"

IrConnect::IrConnect(int irSensor, EditMode *editMode, Cube *cube, Animation *animation) {
	ir = irSensor;
	irrecv = new IRrecv(ir);
	codeIsNew = false;
	results = new decode_results();
	lastCode = new decode_results();
	this->editMode = editMode;
	this->cube = cube;
	this->animation = animation;
	irrecv->enableIRIn();
}

IrConnect::~IrConnect() {
	delete irrecv;
	delete results;
	delete lastCode;
}

void IrConnect::loop() {

	if (irrecv->decode(results)) {
		if (results->value != 4294967295u) { // Wenn Taste gedrückt gehalten wird, kommt dieser code
			lastCode = results;
			codeIsNew = true;

			Serial.println(lastCode->value);
		}

		irrecv->resume();
	}


	if (codeIsNew) {
		processIr();

	}

	codeIsNew = false;
}

void IrConnect::processIr() {
	switch (lastCode->value) {
		case 16748655: // EQ
			if (animation->isEnabled()) {
				editMode->processCommand(EditMode::ANIENABLE, true);
				animation->enable(false);
			} else if (!animation->isEnabled() && !editMode->isEnabled()) {
				editMode->processCommand(EditMode::ENABLE, true);
			} else {
				editMode->processCommand(EditMode::ENABLE, false);
			}
			break;
		case 16769055: // -
			if (animation->isEnabled()) {
				animation->speedDown();
			} else {
				editMode->processCommand(EditMode::LOWER);
			}
			break;
		case 16754775: // +
			if (animation->isEnabled()) {
				animation->speedUp();
			} else {
				editMode->processCommand(EditMode::HIGHER);
			}

			break;
		case 16738455: // 0
			if (animation->isEnabled()) {
				animation->speedDefault();
			} else {
				editMode->processCommand(EditMode::ALL_LOW);
			}
			break;
		case 16750695: // 100 +
			editMode->processCommand(EditMode::ALL_HIGH);
			break;
		case 16724175: // 1
			if (!animation->isEnabled() && !editMode->isEnabled()) {
				Dice::roll(cube);
			} else {
				editMode->processCommand(EditMode::TOGGLE, 0);
			}
			break;
		case 16718055: // 2
			editMode->processCommand(EditMode::TOGGLE, 1);
			break;
		case 16743045: // 3
			editMode->processCommand(EditMode::TOGGLE, 2);
			break;
		case 16716015: // 4
			editMode->processCommand(EditMode::TOGGLE, 3);
			break;
		case 16726215: // 5
			editMode->processCommand(EditMode::TOGGLE, 4);
			break;
		case 16734885: // 6
			editMode->processCommand(EditMode::TOGGLE, 5);
			break;
		case 16728765: // 7
			editMode->processCommand(EditMode::TOGGLE, 6);
			break;
		case 16730805: // 8
			editMode->processCommand(EditMode::TOGGLE, 7);
			break;
		case 16732845: // 9
			editMode->processCommand(EditMode::TOGGLE, 8);
			break;
		case 16736925: // CH
			if (!editMode->isEnabled() && !animation->isEnabled()) {
				animation->enable(true);
			} else {
				animation->enable(false);
			}

			// max 32Bits: 4294967295
			//Animation *animation = new Animation();
			//animation->decodeDWord(0xfa, 0xfe, 0xff, 0xff);

			break;
		case 16753245: // CH -
			if (editMode->isEnabled()) {
				editMode->processCommand(EditMode::ANIREMOVE);
			}
			break;
		case 16769565: // CH +
			if (editMode->isEnabled()) {
				editMode->processCommand(EditMode::ANINEW);
			}
			break;
		case 16720605: // prev
			if (animation->isEnabled()) {
				animation->loadPrev();
			} else if (editMode->isEnabled()) {
				editMode->processCommand(EditMode::ANIPREV);
			}

			break;
		case 16712445: // next
			if (animation->isEnabled()) {
				animation->loadNext();
			} else if (editMode->isEnabled()) {
				editMode->processCommand(EditMode::ANINEXT);
			}
			break;
		case 16761405: // Play
			if (editMode->isEnabled()) {
				editMode->processCommand(EditMode::ANISAVE);
			}
			break;
		case 16756815: // 200 +
			if (editMode->isEnabled()) {
				editMode->processCommand(EditMode::ANISPEED);
			}
			break;
	}
}
