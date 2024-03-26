#pragma once
#include <Arduino.h>
#include <MFRC522.h>
#include "Lock.h"
#include "Storage.h"
#include "Display.h"
#include "UID.h"

#define DELAY_TIME 2500

class Mecho {
public:
  Mecho(MFRC522 &mfrc, BaseUIDStorage &storage, Lock &lock, Display &disp)
    :  mfrc(mfrc), storage(storage), lock(lock), disp(disp) {}
  ~Mecho() = default;

  enum class State {
    VERIFYING = 0,
    OPENED,
    REGISTRATING,
    DELETING,
  };

  void setState(State state) {
    this->state = state;

    switch (state) {
      case State::VERIFYING:
        disp.verificationRequest();
        lock.close();
        break;

      case State::OPENED:
        disp.verificationSuccess();
        lock.open();
        break;

      case State::REGISTRATING:
        disp.registrationRequest();
        break;

      case State::DELETING:
        disp.deletionRequest();
        break;
    }
  }

  void recieveSignal() {
    if (!mfrc.PICC_IsNewCardPresent()) return;
    if (!mfrc.PICC_ReadCardSerial()) return;
    handle({mfrc.uid.size, mfrc.uid.uidByte});
  }

private:
  State state;
  MFRC522 &mfrc;
  BaseUIDStorage &storage;
  Lock &lock;
  Display &disp;

  void verify(UID uid) {
    if (storage.UIDIsPresent(uid)) {
      lock.open();
      disp.verificationSuccess();
      delay(DELAY_TIME);
      setState(State::OPENED);
    } else {
      lock.close();
      disp.verificationFailure();
      delay(DELAY_TIME);
      setState(State::VERIFYING);
    }
  }

  void close(UID uid) {
    if (storage.UIDIsPresent(uid)) {
      disp.closingSuccess(uid);
      delay(400);
      lock.close();
      delay(DELAY_TIME);
      setState(State::VERIFYING);
      return;
    }

    disp.closingFailure();
    delay(DELAY_TIME);
    setState(State::OPENED);
  }

  void reg(UID uid) {
    if (storage.saveUID(uid)) disp.registrationSuccess();
    else disp.registrationFailure();
    delay(DELAY_TIME);
    setState(State::OPENED);
  }

  void del(UID uid) {
    if (storage.deleteUID(uid)) disp.deletionSuccess();
    else disp.deletionFailure();
    delay(DELAY_TIME);
    setState(State::OPENED);
  }

  void handle(UID uid) {
    switch (state) {
      case State::VERIFYING:
        return verify(uid);
        break;

      case State::OPENED:
        return close(uid);
        break;

      case State::REGISTRATING:
        return reg(uid);
        break;

      case State::DELETING:
        return del(uid);
        break;
    }
  }
};