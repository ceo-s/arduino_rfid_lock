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
#include "LightMusic.h"

#define MY_LCD_RS   2
#define MY_LCD_E    3
#define MY_LCD_D4   4
#define MY_LCD_D5   5
#define MY_LCD_D6   6
#define MY_LCD_D7   7
#define SERVO_PIN_N 10
#define BUTTON_PIN A1
#define BUZZER_PIN A2
#define LED_PIN    A3

#define RST_PIN 9
#define CS_PIN 8

LightMusic lightMusic(BUZZER_PIN, LED_PIN);

OneButton button(BUTTON_PIN, true, true);

UIDStorage storage;

Servo servo;
Lock lock(servo);

LiquidCrystal lcd(MY_LCD_RS, MY_LCD_E, MY_LCD_D4, MY_LCD_D5, MY_LCD_D6, MY_LCD_D7);
Display disp(lcd);

MFRC522 mfrc(CS_PIN, RST_PIN);

Mecho mecho(mfrc, storage, lock, disp, lightMusic);


void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  servo.attach(SERVO_PIN_N);
  lock.close();
  disp.verificationRequest();

  Serial.begin(9600);
  SPI.begin();
  mfrc.PCD_Init();

  storage.init();
  mecho.setState(Mecho::State::VERIFYING);

  button.attachClick([]() {
    mecho.setState(Mecho::State::REGISTRATING);
  });

  button.attachDoubleClick([]() {
    mecho.setState(Mecho::State::DELETING);
  });

  button.attachLongPressStop([]() {
    mecho.setState(Mecho::State::OPENED);
  });
}

void loop() {
  if (mecho.getState() != Mecho::State::VERIFYING) button.tick();
  mecho.recieveSignal();
}