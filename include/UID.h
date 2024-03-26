#pragma once
#include <Arduino.h>

struct UID {
  size_t size;
  byte *data;
};

void printUID(UID uid) {
  Serial.println("UID:");
  Serial.print("  Size:    ");
  Serial.print(uid.size);
  Serial.print('\n');
  Serial.print("  Data:    ");

  for (size_t i=0; i < uid.size; i++) {
    Serial.print(uid.data[i], HEX);
    Serial.print("  ");
  }

  Serial.print('\n');
}