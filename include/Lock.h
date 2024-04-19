#pragma once
#include <Arduino.h>
#include <Servo.h>
#define LOCK_OPENED 180
#define LOCK_CLOSED 0

class Lock {
public:
  Lock(Servo &servo) : servo(servo) {
    state = State::CLOSED;
  }
  ~Lock() = default;

  enum class State {
    CLOSED = 0,
    OPENED = 1,
  };

  void open() {
    servo.write(LOCK_OPENED);
    state = State::OPENED;
  }

  void close() {
    servo.write(LOCK_CLOSED);
    state = State::CLOSED;
  }

private:
  Servo &servo;
  State state;
};