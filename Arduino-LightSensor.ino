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
      int minusValue = map(lux, 40, 200, 0, 125);
      ledValue -= minusValue;
      ledValue -= 1;
    }
    if (lux < EXPECTED_VALUE) {
      int plusValue = map(lux, 0, 40, 0, 62);
      ledValue += plusValue;
      ledValue += 1;
    }
    if ( ledValue <= 0) {
      ledValue = 0;
    }
    if ( ledValue >= 255) {
      ledValue = 255;
    }
    analogWrite(ledPin, ledValue);
    Serial.println(ledValue);
  }
}
