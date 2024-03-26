#include <Arduino.h>
#include <EEPROM.h>
#include <Servo.h>
#include <LiquidCrystal.h>
#include <MFRC522.h>
#include <OneButton.h>
#include <Lock.h>
#include "Display.h"
#include "Storage.h"
#include "Mecho.h"

#define MY_LCD_RS 2
#define MY_LCD_E 3
#define MY_LCD_D4 4
#define MY_LCD_D5 5
#define MY_LCD_D6 6
#define MY_LCD_D7 7
#define SERVO_PIN_N 10
#define BUTTON_PIN A1

#define RST_PIN 9
#define CS_PIN 8

OneButton button(BUTTON_PIN, true, true);

UIDStorage storage;

Servo servo;
Lock lock(servo);

LiquidCrystal lcd(MY_LCD_RS, MY_LCD_E, MY_LCD_D4, MY_LCD_D5, MY_LCD_D6, MY_LCD_D7);
Display disp(lcd);

MFRC522 mfrc(CS_PIN, RST_PIN);

Mecho mecho(mfrc, storage, lock, disp);


void setup() {
  Serial.begin(9600);
  servo.attach(SERVO_PIN_N);
  lock.close();
  disp.verificationRequest();

  SPI.begin();
  mfrc.PCD_Init();
  mfrc.PCD_DumpVersionToSerial();

  mecho.setState(Mecho::State::OPENED);

  button.attachClick([]() {
    mecho.setState(Mecho::State::REGISTRATING);
    Serial.println("clk");
  });

  button.attachDoubleClick([]() {
    mecho.setState(Mecho::State::DELETING);
    Serial.println("dclk");
  });

  button.attachLongPressStop([]() {
    mecho.setState(Mecho::State::OPENED);
    Serial.println("hold");
  });
}

void loop() {
  // Serial.println("Hello mf");
  button.tick();
  mecho.recieveSignal();
}