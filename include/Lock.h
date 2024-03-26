#pragma once
#include <Arduino.h>
#include <Servo.h>
#define LOCK_OPENED 180
#define LOCK_CLOSED 0

class Lock {
public:
  Lock(uint8_t pin) {
    state = State::CLOSED;
    servo.attach(pin);
  }
  ~Lock() = default;

  enum class State {
    CLOSED = 0,
    OPENED = 1,
  };

  void open() {
    if (state == State::OPENED) return;
    servo.write(LOCK_OPENED);
  }

  void close() {
    if (state == State::CLOSED) return;
    servo.write(LOCK_CLOSED);
  }

private:
  State state;
  Servo servo;
};