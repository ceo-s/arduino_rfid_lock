#pragma once
#include <Arduino.h>
#include <LiquidCrystal.h>

class Display {
public:
  Display(LiquidCrystal &lcd) : lcd(lcd) {
    lcd.begin(16, 2, LCD_5x8DOTS);
  }
  ~Display() = default;

  void verificationRequest() {
    print("Attach a pass:", "");
  }

  void verificationSuccess() {
    print("Welcome!", "");
  }

  void verificationFailure() {
    print("Access denied!", "");
  }

  void registrationRequest() {
    print("Attach a pass:", "");
  }

  void registrationSuccess() {
    print("Welcome!", "");
  }

  void registrationFailure() {
    print("Access denied!", "");
  }

  void deletionRequest() {
    print("Attach a pass:", "");
  }

  void deletionSuccess() {
    print("Welcome!", "");
  }

  void deletionFailure() {
    print("Access denied!", "");
  }

private:
  LiquidCrystal &lcd;
  template<size_t S1, size_t S2>
  void print(const char (&str1)[S1], const char (&str2)[S2]) {
    lcd.clear();
    lcd.println(str1);
    lcd.println(str2);
  }
};