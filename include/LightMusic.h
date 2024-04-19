#pragma once
#include <Arduino.h>

class LightMusic {
public:
  LightMusic(int buzzerPin, int ledPin) : buzzerPin(buzzerPin), ledPin(ledPin) {}
  ~LightMusic() = default;

  void doorSound() {
    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 1000);
    delay(500);
    noTone(buzzerPin);
    digitalWrite(ledPin, LOW);
  }

  void deniedSound() {
    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 1500);
    delay(500);
    noTone(buzzerPin);
    digitalWrite(ledPin, LOW);
  }

  void successSound() {
    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 2000);
    delay(500);
    noTone(buzzerPin);
    digitalWrite(ledPin, LOW);
  }

  void canceledSound() {
    digitalWrite(ledPin, HIGH);
    for (int i=0; i < 2; i++) {
      tone(buzzerPin, 2500);
      delay(250);
      noTone(buzzerPin);
    }
    digitalWrite(ledPin, LOW);
  }

private:
  int buzzerPin;
  int ledPin;
};