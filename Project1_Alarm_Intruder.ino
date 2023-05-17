#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial bluetoothSerial(11,10);

LiquidCrystal_I2C lcd(0x27, 16, 2);

int pirPinNumber = 2;
int pirState;
int isMotion = false;

int ledPinNumber = 3;
int buzzerPinNumber = 4;

void setup() {
  // put your setup code here, to run once:
  pinMode(pirPinNumber,INPUT);
  pinMode(ledPinNumber,OUTPUT);
  Serial.begin(9600);
  bluetoothSerial.begin(9600);
  lcd.init();

  Serial.println("Silent Alarm ");
  Serial.println("Initializing System");
}

void loop() {
  // put your main code here, to run repeatedly:
  pirState = digitalRead(pirPinNumber);
  if (pirState == HIGH && isMotion == false) {
      lcd.backlight();
      lcd.print("Motion Detected");
      delay(500);
      lcd.clear();
      // Will sent to phone using bluetooth
      sendBluetoothSetAlarmSignal();

      lcd.print("Alarm Signal is Sent");
      delay(1000);
      lcd.clear();
      lcd.noBacklight();

      isMotion = true;
  }

  if (pirState == LOW && isMotion == true ) {
    lcd.backlight();
    lcd.print("Clearing Alarm Signal");
    delay(500);
    // Will sent to phone using bluetooth
    sendBluetoothClearAlarmSignal();

    lcd.clear();
    lcd.print("Alarm Clear");
    delay(500);
    lcd.clear();
    lcd.noBacklight();

    isMotion = false;
  }


  digitalWrite(ledPinNumber,isMotion);
  digitalWrite(buzzerPinNumber,isMotion);
}

void sendBluetoothSetAlarmSignal(){
  bluetoothSerial.println("Intruder Alert");
}

void sendBluetoothClearAlarmSignal(){
  bluetoothSerial.println("No Intruder");
}
