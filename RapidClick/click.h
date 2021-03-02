#pragma once

#include "common.h"

class Clicker {
  PIMPL

public:
  Clicker();
  ~Clicker();

  void setActivate(int);
  void setOutput(int);
  void pause();
  void enableToggle();
  void enableHold();

  bool isEnabled();
};