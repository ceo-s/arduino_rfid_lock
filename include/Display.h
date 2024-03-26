#pragma once
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "UID.h"

class Display {
public:
  Display(LiquidCrystal &lcd) : lcd(lcd) {
    lcd.begin(16, 2, LCD_5x8DOTS);
  }
  ~Display() = default;

  void verificationRequest() {
    print("Verifying:", "Attach a pass");
  }

  void verificationSuccess() {
    print("Success:", "Lock is open");
  }

  void verificationFailure() {
    print("Access denied:", "Try again");
  }

  void registrationRequest() {
    print("Registrating:", "Attach a pass");
  }

  void registrationSuccess() {
    print("Succeess:", "Registered card");
  }

  void registrationFailure() {
    print("Failure:", "Storage is full");
  }

  void deletionRequest() {
    print("Deleting:", "Attach a pass");
  }

  void deletionSuccess() {
    print("Success:", "Deleted card");
  }

  void deletionFailure() {
    print("Failure:", "Card is last");
  }

  void closingSuccess(UID uid) {
    print("Success:", "Door is closed");
  }

  void closingFailure() {
    print("Failure:", "Card is bad");
  }

private:
  LiquidCrystal &lcd;
  template<size_t S1, size_t S2>
  void print(const char (&str1)[S1], const char (&str2)[S2]) {
    lcd.clear();
    lcd.print(str1);
    lcd.setCursor(0, 1);
    lcd.print(str2);
  }
};
