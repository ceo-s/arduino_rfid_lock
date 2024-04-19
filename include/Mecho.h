#pragma once
#include <Arduino.h>
#include <MFRC522.h>
#include "Lock.h"
#include "Storage.h"
#include "Display.h"
#include "UID.h"
#include "LightMusic.h"

#define DELAY_TIME 2500

class Mecho {
public:
  Mecho(MFRC522 &mfrc,
        BaseUIDStorage &storage,
        Lock &lock,
        Display &disp,
        LightMusic &lightMusic)
        : mfrc(mfrc),
          storage(storage),
          lock(lock),
          disp(disp),
          lightMusic(lightMusic) {}
  ~Mecho() = default;

  enum class State {
    VERIFYING = 0,
    OPENED,
    REGISTRATING,
    DELETING,
  };

  State getState() {
    return state;
  }

  void setState(State state) {
    State oldState = this->state;
    this->state = state;

    switch (state) {
      case State::VERIFYING:
        disp.verificationRequest();
        lock.close();
        break;

      case State::OPENED:
        if (oldState == State::REGISTRATING || oldState == State::DELETING) {
          lightMusic.canceledSound();
        }
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
  LightMusic &lightMusic;

  void verify(UID uid) {
    if (storage.UIDIsPresent(uid)) {
      lock.open();
      disp.verificationSuccess();
      lightMusic.doorSound();
      delay(DELAY_TIME);
      setState(State::OPENED);
    } else {
      lock.close();
      disp.verificationFailure();
      lightMusic.deniedSound();
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
    if (storage.saveUID(uid)) {
      disp.registrationSuccess();
      lightMusic.successSound();
    }
    else {
      disp.registrationFailure();
      lightMusic.deniedSound();
    }
    delay(DELAY_TIME);
    setState(State::OPENED);
  }

  void del(UID uid) {
    if (storage.deleteUID(uid)) {
      disp.deletionSuccess();
      lightMusic.successSound();
    }
    else {
      disp.deletionFailure();
      lightMusic.deniedSound();
    }
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