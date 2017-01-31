// Do not remove the include below
#include "LedCube.h"
#include "Arduino.h"

//Erster Start:
//Damit im Speicher die Animationen am anfang leer sind
//und nicht 64 (leere) Bilder enthalten: 1x "w" per Seriell senden
//
//Tastenbelegung:
//EQ: Editiermodus starten. Ist der Animationsmodus aktiv, wird der Animations-editiermodus gestartet.
//	ist der Animationsmodes nicht gestartet, wird der Standbild-Edit-Modus gestartet
//CH: Animationsmodus starten
//Prev / Next:	Im Animationsmodus: Nächste/Vorherige Animation
//				Im Ani-Edit-Modus: Nächstes/Vorheriges Bild laden
//CH- /CH+: Im Ani-Edit_modus: letztes Bild entfernen / neuse Bild hintendrann hängen
//			- es wird nicht sofort zu dem neuen Bild gewechselt.
//			- Falls das Aktuelle Bild gelöscht wird, wird automatisch zum ersten gewechselt.
//			- Erstes Bild kann nicht gelöscht werden
//Play/Pause: Im Ani-Edit-Mode: Animation komplett Speichern, Ani-Edit-Mode beenden und Animation abspielen
//200+: Im Ani-Edit-Mode: Zeigedauer des aktuellen Bildes ändern (Zeitdauer wird durch invertieren des kompletten Würfels demonstriert)
//		8 Verschiedene Zeiten möglich
//
//
//Folgende Befehle gelten für beide Edit-Modes
//Vol- / Vol+: aktuell editierbare Eben erhöhen/verringern
//1 - 9: LED in aktueller Ebene invertieren
//0: Alle LEDs aus
//100+: Alle Leds an
//EQ: Edit-Modus beenden. im Ani-Edit-Mode wird nichts gespeichert!!!!

void setup() {
	//Serial.begin(9600);
	noInterrupts();

	irSensor = A0;
	cube = new Cube();
	eeprom = new EepromHandler();
	animation = new Animation(eeprom, cube);
	editMode = new EditMode(cube, animation);
	ir = new IrConnect(irSensor, editMode, cube, animation);


}

void loop() {
	ir->loop();
	editMode->loop();
	cube->loop();
	eeprom->loop();
	animation->loop();
}
