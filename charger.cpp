// Do not remove the include below
#include "charger.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SDL_Arduino_INA3221.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define OLED_ADDRESS   0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
SDL_Arduino_INA3221 ina3221;

void setup() {
	Serial.begin(115200);
	ina3221.begin(INA3221_CONFIG_SETCONFIG, 10, 10, 10);
	display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
	display.setRotation(2);
	display.cp437(true);         // Use full 256 char 'Code Page 437' font
	display.setTextColor(SSD1306_WHITE); // Draw white text
	display.setTextSize(1);      // Normal 1:1 pixel scale
}

void loop() {

	float totalVoltage = 0;
	float totalCurrent = 0;
	float totalPower = 0;

	display.clearDisplay();

	for (int ch = 1; ch < 4; ch++) {
		float busVoltage = 0;
		float current = 0;
		float power = 0;

		busVoltage = ina3221.getBusVoltage_V(ch);
		current = ina3221.getCurrent_mA(ch) / 1000;
		power = current * busVoltage;
//
//		Serial.print(ch);
//		Serial.print(" ");
//		Serial.print(busVoltage);
//		Serial.print(" ");
//		Serial.print(ina3221.getCurrent_mA(ch));
//		Serial.print(" ");
//		Serial.println(ina3221.getShuntVoltage_mV(ch));

		totalVoltage = busVoltage;
		totalCurrent += current;

		int x = 10 + (40 * (ch - 1));

		display.setCursor(x, 30);
		display.print(current, 1);
		display.print("A");

		display.setCursor(x, 40);
		display.print(power, 1);
		display.print("W");

	}

	display.setCursor(15, 8);
	totalPower = totalCurrent * totalVoltage;

	display.print(totalVoltage, 1);
	display.print("V ");
	display.print(totalCurrent, 1);
	display.print("A ");
	display.print(totalPower, 1);
	display.print("W ");

	display.drawLine(0, 20, 128, 20, SSD1306_WHITE);
	display.drawLine(40, 20, 40, 64, SSD1306_WHITE);
	display.drawLine(80, 20, 80, 64, SSD1306_WHITE);

	display.display();

	delay(100);
}
