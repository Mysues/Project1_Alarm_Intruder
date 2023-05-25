#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial bluetoothSerial(11, 10);

LiquidCrystal_I2C lcd(0x27, 16, 2);

int pirPinNumber = 2;
int pirState;
int isMotion = false;
long unsigned int prevIsMotion = false;

int ledPinNumber = 3;
int buzzerPinNumber = 5;

long unsigned int prevTime;
long unsigned int interval = 500;



void setup() {
  // put your setup code here, to run once:
  pinMode(pirPinNumber, INPUT);
  pinMode(ledPinNumber, OUTPUT);
  pinMode(buzzerPinNumber, OUTPUT);

  bluetoothSerial.begin(9600);

  lcd.init();
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("No Intruder");

  Serial.begin(9600);
  Serial.println("Silent Alarm ");
  Serial.println("Initializing System");
}

void loop() {
  // put your main code here, to run repeatedly:
  readingSensor();
  processingData();
}

void readingSensor() {
  pirState = digitalRead(pirPinNumber);

  if (pirState == HIGH && isMotion == false)
    isMotion = true;

  else if (pirState == LOW && isMotion == true )
    isMotion = false;
}

void processingData() {
  long unsigned int currentTime = millis();
  long unsigned int period = currentTime - prevTime;
  if (period > interval) {

    // Only Change When isMotion change compared to previous state
    if (isMotion != prevIsMotion) {
      //Control LED
      if (isMotion)
        digitalWrite(ledPinNumber, HIGH);
      else
        digitalWrite(ledPinNumber, LOW);

      //Control Piezo
      if (isMotion)
        tone(buzzerPinNumber, 900);
      else
        noTone(buzzerPinNumber);

      //Control LCD
      if (isMotion) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("No Intruder");
      }

      else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Intruder Detected!!!");
      }

      prevIsMotion = isMotion;
    }


    //BlueTooth Signal

    if (isMotion)
      sendBluetoothSetAlarmSignal();
    else
      sendBluetoothClearAlarmSignal();
    prevTime = currentTime;
  }

}

void sendBluetoothSetAlarmSignal() {
  bluetoothSerial.println("Motion");
  Serial.println("Motion");
}

void sendBluetoothClearAlarmSignal() {
  bluetoothSerial.println("!Motion");
  Serial.println("!Motion");
}
