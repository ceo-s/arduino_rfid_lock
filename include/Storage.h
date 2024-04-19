#pragma once
#include <Arduino.h>
#include <EEPROM.h>
#include "UID.h"

#define SIZE_ADDR 9
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
  UIDStorage() {}
  ~UIDStorage() {
    EEPROM.update(SIZE_ADDR, size_);
  }

  void init() {
    size_ = EEPROM.read(SIZE_ADDR);
  }

  bool saveUID(UID uid) {
    if (size_ == capacity_) return false;
    if (UIDIsPresent(uid)) return true;
    int addr = getOffset();

    EEPROM.update(addr++, uid.size);
    for (size_t i=0; i <= uid.size; i++) {
      EEPROM.update(addr + i, uid.data[i]);
    }

    updateSize(size_ + 1);
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
    updateSize(size_ - 1);

    return true;
  }

  bool UIDIsPresent(UID uid) {
    if (findUID(uid) == -1) return false;
    return true;
  }

  inline size_t size() {
    return size_;
  }

  inline size_t capacity() {
    return capacity_;
  }

private:
  size_t size_ = 0;
  const size_t capacity_ = STORAGE_CAPACITY;

  int getOffset() {
    return size_ * RECORD_SIZE + START_ADDR;
  }

  int findUID(UID uid) {
    // returns addr if uid present and -1 if not
    for (int addr=START_ADDR; addr < getOffset(); addr += RECORD_SIZE) {
      if (uidcmp(addr, uid)) return addr;
    }
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

  void updateSize(size_t newSize) {
    size_ = newSize;
    EEPROM.update(SIZE_ADDR, size_);
  }

  // void printRecord(int addr) {
  //   Serial.println("Record:");
  //   Serial.print("  Size:    ");
  //   size_t size = EEPROM.read(addr);
  //   Serial.print(size);
  //   Serial.print('\n');
  //   Serial.print("  Data:    ");

  //   for (size_t i=1; i <= size; i++) {
  //     Serial.print(EEPROM.read(addr + i), HEX);
  //     Serial.print("  ");
  //   }

  //   Serial.print('\n');
  // }
};