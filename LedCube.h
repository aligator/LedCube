// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _Cube2_H_
#define _Cube2_H_
#include "Arduino.h"

#include "Cube.h"
#include "EditMode.h"
#include "IrConnect.h"
#include "EepromHandler.h"

int irSensor;

Cube *cube;
EditMode *editMode;
IrConnect *ir;
EepromHandler *eeprom;
Animation *animation;

#endif /* _Cube2_H_ */
