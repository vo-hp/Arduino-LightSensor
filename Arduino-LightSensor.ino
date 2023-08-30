#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <BH1750.h>
#include <Wire.h>

BH1750 lightMeter(0x23);
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int MAX_VALUE = 80;
const int MIN_VALUE = 0;
const int EXPECTED_VALUE = 40;
int ledValue = 0;
int ledPin = 6;
bool slowUpdate = false;
int preValue[4] = { 0, 0, 0, 0 };
void pushValue(int value) {
  preValue[0] = preValue[1];
  preValue[1] = preValue[2];
  preValue[2] = preValue[3];
  preValue[3] = value;
}
bool checkBlink() {
  return preValue[0] == 0 or preValue[1] == 0 or  preValue[2] == 0 or preValue[3] == 0;
}

void setup() {
  Serial.begin(115200);
  lcd.init();
  Wire.begin();
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("BH1750 Advanced begin"));
  } else {
    Serial.println(F("Error initialising BH1750"));
  }
}

void loop() {
  if (lightMeter.measurementReady()) {
    float lux = lightMeter.readLightLevel();
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.print(" lx    ");
    if (lux > EXPECTED_VALUE) {
      int minusValue = map(lux, 40, 200, 0, 62);
      if (minusValue > 10) {
        minusValue = 10;
      }
      if (slowUpdate) {
        minusValue = 1;
      }
      ledValue -= minusValue;
      // ledValue -= 1;
      Serial.print("  - " + String(minusValue) + "   ");
    }
    if (lux < EXPECTED_VALUE) {
      int plusValue = map(lux, 40, 0, 0, 62);
      if (plusValue > 10) {
        plusValue = 10;
      }
      if (slowUpdate) {
        plusValue = 1;
      }
      ledValue += plusValue;
      // ledValue += 1;
      Serial.print("  + " + String(plusValue) + "   ");
    }
    if (ledValue <= 0) {
      ledValue = 0;
    }
    if (ledValue >= 255) {
      ledValue = 255;
    }
    pushValue(ledValue);
    slowUpdate = checkBlink();
    analogWrite(ledPin, ledValue);
    Serial.println(ledValue);
  }
}
