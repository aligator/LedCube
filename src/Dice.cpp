#include "Dice.h"

void Dice::roll(Cube *cube) {
	 int rand = (int) random(1, 7);
	 unsigned long dword = 0;

	 switch (rand) {
	 	case 1:
	 		dword = 0x00400000;
	 		break;
	 	case 2:
			dword = 0x04040000;
			  break;
	 	case 3:
			dword = 0x04440000;
			break;
	 	case 4:
			dword = 0x05140000;
			break;
	 	case 5:
			dword = 0x05540000;
			break;
	 	case 6:
			dword = 0x05b40000;
			break;
	}

	cube->setFromDWord(dword);
}
