#pragma once
#include <Arduino.h>
#include <MFRC522.h>
#include "Lock.h"
#include "Storage.h"
#include "Display.h"


class Mecho {
public:
  Mecho(MFRC522 &mfrc, BaseUIDStorage &storage, Lock &lock, Display &disp)
    :  mfrc(mfrc), storage(storage), lock(lock), disp(disp) {
      SPI.begin();
      mfrc.PCD_Init();
      mfrc.PCD_DumpVersionToSerial();
  }
  ~Mecho() = default;

  enum class State {
    VERIFYING = 0,
    REGISTRATING = 1,
    DELETING = -1,
  };

  void setState(State state) {
    if (this->state == state) return;
    this->state = state;

    switch (state) {
      case State::VERIFYING:
        disp.verificationRequest();

      case State::REGISTRATING:
        disp.registrationRequest();

      case State::DELETING:
        disp.deletionRequest();
    }
  }

  void recieveSignal() {
    if (!mfrc.PICC_IsNewCardPresent()) return;
    if (!mfrc.PICC_ReadCardSerial()) return;
    handle(mfrc.uid.uidByte, mfrc.uid.size);
  }

  void verify(byte uid[10], size_t size) {
    if (storage.UIDIsPresent(uid, size)) {
      lock.open();
      disp.verificationSuccess();
    } else {
      lock.close();
      disp.verificationFailure();
    }
    
  }

  void reg(byte uid[10], size_t size) {
    if (storage.saveUID(uid, size)) return;
    disp.registrationFailure();
  }

  void del(byte uid[10], size_t size) {
    if (storage.deleteUID(uid, size)) return;
    disp.deletionFailure();
  }

private:
  State state = State::VERIFYING;
  MFRC522 &mfrc;
  BaseUIDStorage &storage;
  Lock &lock;
  Display &disp;

  void handle(byte uid[10], size_t size) {
    switch (state) {
      case State::VERIFYING:
        return verify(uid, size);

      case State::REGISTRATING:
        return reg(uid, size);

      case State::DELETING:
        return del(uid, size);
    }
  }
};