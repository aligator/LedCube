/*
 * EditMode.h
 *
 *  Created on: 30.03.2016
 *      Author: johannes
 */

#ifndef EDITMODE_H_
#define EDITMODE_H_
#include "Cube.h"
#include "Arduino.h"
#include "Animation.h"


class EditMode {
public:
	EditMode(Cube *cube, Animation *animation);
	virtual ~EditMode();

	enum Commands {ENABLE, LOWER, HIGHER, TOGGLE, ALL_LOW, ALL_HIGH, ANIENABLE, ANINEXT, ANIPREV, ANISPEED, ANISAVE, ANINEW, ANIREMOVE};

	void loop();
	void processCommand(Commands command, int parameter);
	void processCommand(Commands command, bool parameter);
	void processCommand(Commands command);
	bool isEnabled();
	int getActiveLayer();

private:
	void incLayer();
	void decLayer();
	void initLayer(int layer);
	void setAll(bool state);
	void toggleLed(int pos);
	bool isReady();
	void enable();
	void isAnimationEdit();
	void disable();

	Cube *cube;
	Animation *animation;
	int activeLayer; 	// -1 is deactivated
	bool isInInitPhase;
	bool animationEdit;
	int initPhaseCounter;
};

#endif /* EDITMODE_H_ */
