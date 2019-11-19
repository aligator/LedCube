/*
 * EditMode.cpp
 *
 *  Created on: 30.03.2016
 *      Author: johannes
 */

#include "EditMode.h"

EditMode::EditMode(Cube *cube, Animation *animation) {
	activeLayer = -1;
	this->cube = cube;
	this->animation = animation;
	isInInitPhase = false;
	initPhaseCounter = 0;
	animationEdit = false;
}

EditMode::~EditMode() {

}

void EditMode::loop() {
	if (isInInitPhase) {
  		if (initPhaseCounter <= 1000) {
			if(initPhaseCounter == 0) {
				cube->toggleLayer(activeLayer);
			}
			initPhaseCounter++;
		} else {
			initPhaseCounter = 0;
			isInInitPhase = false;
			cube->toggleLayer(activeLayer);
		}
	}
}

void EditMode::incLayer() {
	int newLayer = activeLayer + 1;
	if (newLayer < cube->getMaxLayer()) {
		initLayer(newLayer);
	}
}

void EditMode::decLayer() {
	int newLayer = activeLayer - 1;

	if (newLayer >= 0) {
		initLayer(newLayer);
	}
}

void EditMode::initLayer(int layer) {
	if (!isInInitPhase) {
		isInInitPhase = true;
		initPhaseCounter = 0;
		activeLayer = layer;
	}
}

void EditMode::enable() {
	if (activeLayer < 0) {
		initLayer(0);
	}
}

void EditMode::disable() {
	if (activeLayer >= 0) {

		if (isInInitPhase) {
			cube->toggleLayer(activeLayer);
		}

		initPhaseCounter = 0;
		isInInitPhase = false;
		activeLayer = -1;

	}
}


void EditMode::processCommand(Commands command, int parameter) {
	Serial.println("process");
	switch (command) {
		case ENABLE:
			animationEdit = false;
			if (parameter == 1) {
				enable();
			} else {
				disable();
			}
			break;
		case ANIENABLE:
			animationEdit = true;
			if (parameter == 1) {
				enable();
				animation->loadFirstPicture();
			} else {
				disable();
			}
			break;
		case LOWER:
			decLayer();
			break;
		case HIGHER:
			incLayer();
			break;
		case TOGGLE:
			toggleLed(parameter);
			break;
		case ALL_LOW:
			setAll(LOW);
			break;
		case ALL_HIGH:
			setAll(HIGH);
			break;
		case ANINEXT:
			if (animationEdit) {
				animation->loadNextPicture();
			}
			break;
		case ANIPREV:
			if (animationEdit) {
				animation->loadPrevPicture();
			}
			break;
		case ANISAVE:
			if (animationEdit) {
				animation->save();
				processCommand(ANIENABLE, false);
				animation->enable(true);
			}
			break;
		case ANISPEED:
			if (animationEdit) {
				animation->setTime();
			}
			break;
		case ANIREMOVE:
			if (animationEdit) {
				animation->remove();
			}
			break;
		case ANINEW:
			if (animationEdit) {
				animation->add();
			}
			break;
	}
}

bool EditMode::isReady() {
	if (activeLayer != -1 && !isInInitPhase) {
		return true;
	}
	return false;
}

void EditMode::setAll(bool state) {
	if (isReady()) {
		cube->setLed(state);
		if (animationEdit) {
			animation->setLed(state);
		}
	}
}

void EditMode::toggleLed(int pos) {
	if (isReady()) {
		cube->toggleLed(activeLayer, pos);

		if (animationEdit) {
			animation->setLed(activeLayer, pos);
		}
	}

}

void EditMode::processCommand(Commands command, bool parameter) {
	if (parameter) {
		processCommand(command, 1);
	} else {
		processCommand(command, 0);
	}
}

void EditMode::processCommand(Commands command) {
	processCommand(command, -1);
}

bool EditMode::isEnabled() {
	return (activeLayer >= 0);
}

int EditMode::getActiveLayer() {
	return activeLayer;
}
