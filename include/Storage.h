#pragma once
#include <Arduino.h>
#include <EEPROM.h>
#define STORAGE_CAPACITY 6

class BaseUIDStorage {
public:
  virtual bool saveUID(byte uid[10], size_t size) = 0;
  virtual bool deleteUID(byte uid[10], size_t size) = 0;
  virtual bool UIDIsPresent(byte uid[10], size_t size) = 0;
  virtual size_t size() = 0;
  virtual size_t capacity() = 0;
};

class UIDStorage : public BaseUIDStorage {
public:
  UIDStorage() {

  }
  ~UIDStorage() = default;

  virtual bool saveUID(byte uid[10], size_t size) {
    if (size_ == capacity_) return false;
  }

  virtual bool deleteUID(byte uid[10], size_t size) {
    if (size <= 1) return false;
  }

  virtual bool UIDIsPresent(byte uid[10], size_t size) {

  }

  virtual size_t size() {
    return size_;
  }

  virtual size_t capacity() {
    return capacity_;
  }

private:
  size_t size_ = 0;
  const size_t capacity_ = 6;
};