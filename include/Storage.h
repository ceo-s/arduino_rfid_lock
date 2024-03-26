#pragma once
#include <Arduino.h>
#include <EEPROM.h>
#include "UID.h"

#define START_ADDR 10
#define RECORD_SIZE 11 // 1 byte for size + 10 bytes for uid data
#define STORAGE_CAPACITY 30

class BaseUIDStorage {
public:
  virtual bool saveUID(UID uid) = 0;
  virtual bool deleteUID(UID uid) = 0;
  virtual bool UIDIsPresent(UID uid) = 0;
  virtual size_t size() = 0;
  virtual size_t capacity() = 0;
};

class UIDStorage : public BaseUIDStorage {
public:
  UIDStorage() {

  }
  ~UIDStorage() = default;

  bool saveUID(UID uid) {
    if (size_ == capacity_) return false;
    if (UIDIsPresent(uid)) return true;
    int addr = getOffset();

    EEPROM.update(addr++, uid.size);
    for (size_t i=0; i <= uid.size; i++) {
      EEPROM.update(addr + i, uid.data[i]);
    }

    size_++;
    return true;
  }

  bool deleteUID(UID uid) {
    if (size_ <= 1) return false;
    int uidAddr = findUID(uid);
    if (uidAddr == -1) return true;

    int lastRecordAddr = getOffset() - RECORD_SIZE;
    for (int i=0; i < 11; i++) {
      EEPROM.update(uidAddr + i, EEPROM.read(lastRecordAddr + i));
    }
    size_--;

    return true;
  }

  bool UIDIsPresent(UID uid) {
    if (findUID(uid) == -1) return false;
    return true;
  }

  size_t size() {
    return size_;
  }

  size_t capacity() {
    return capacity_;
  }

private:
  size_t size_ = 0;
  const size_t capacity_ = 6;

  int getOffset() {
    return size_ * RECORD_SIZE + START_ADDR;
  }

  int findUID(UID uid) {
    // returns addr if uid present and -1 if not
    Serial.print("Comparing.\nStorage size == ");
    Serial.print(size_);
    Serial.print('\n');
    for (int addr=START_ADDR; addr < getOffset(); addr += RECORD_SIZE) {
      Serial.println("Comparing\n");
      printRecord(addr);
      Serial.println("");
      printUID(uid);
      if (uidcmp(addr, uid)) return addr;
    }
    Serial.println("Not found");
    return -1;
  }

  bool uidcmp(int addr, UID uid) {
    if (EEPROM.read(addr) != uid.size) return false;

    addr++;
    for (size_t i=0; i < uid.size; i++) {
      if (EEPROM.read(addr + i) != uid.data[i]) return false;
    }

    return true;
  }

  void printRecord(int addr) {
    Serial.println("Record:");
    Serial.print("  Size:    ");
    size_t size = EEPROM.read(addr);
    Serial.print(size);
    Serial.print('\n');
    Serial.print("  Data:    ");

    for (size_t i=1; i <= size; i++) {
      Serial.print(EEPROM.read(addr + i), HEX);
      Serial.print("  ");
    }

    Serial.print('\n');
  }
};