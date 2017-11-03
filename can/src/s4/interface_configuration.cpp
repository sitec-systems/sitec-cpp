// Copyright 2017 sitec systems GmbH. All rights reserved
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "../interface_configuration.hpp"

#include <cstdio>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <system_error>

#include <libsocketcan.h>

#include "../interface_state.hpp"

using std::runtime_error;
using std::system_error;
using std::string;
using std::system_category;
using std::stringstream;

namespace sitec {
namespace can {

namespace {
const string CAN0_TERMINATOR("500");
const string CAN1_TERMINATOR("502");
const string CAN0_INTERFACE("can0");
const string CAN1_INTERFACE("can1");
const string GPIO_BASEPATH("/sys/class/gpio/gpio");
const char CAN0_TRANSCEIVER[] = "/sys/bus/spi/drivers/tja1145/spi2.0/modectrl";
const char CAN0_ENABLE[] = "normal";
const char CAN0_DISABLE[] = "standby";
}

InterfaceConfiguration::InterfaceConfiguration(const char* ifaceName,
                                               const int bitrate,
                                               const bool terminator) {
  this->ifaceName = string(ifaceName);
  this->bitrate = bitrate;
  this->terminator = terminator;
}

InterfaceConfiguration::~InterfaceConfiguration() { down(); }

void InterfaceConfiguration::up() {
  auto state = getState();
  if (state != InterfaceState::stopped) {
    down();
  }

  configureBitrate();

  auto ret = can_do_start(ifaceName.c_str());
  if (ret != 0) {
    throw system_error(EIO, system_category());
  }

  switchTerminator(terminator);
  switchTransceiver(true);
}

void InterfaceConfiguration::down() {
  auto ret = can_do_stop(ifaceName.c_str());
  if (ret != 0) {
    throw system_error(EIO, system_category());
  }

  switchTerminator(false);
  switchTransceiver(false);
}

InterfaceState InterfaceConfiguration::getState() {
  int state = 0;

  auto ret = can_get_state(ifaceName.c_str(), &state);
  if (ret != 0) {
    throw system_error(EIO, system_category());
  }

  switch (state) {
    case CAN_STATE_STOPPED:
      return InterfaceState::stopped;
    case CAN_STATE_ERROR_ACTIVE:
      return InterfaceState::errorActive;
    case CAN_STATE_ERROR_WARNING:
      return InterfaceState::errorWarning;
    case CAN_STATE_ERROR_PASSIVE:
      return InterfaceState::errorPassive;
    case CAN_STATE_BUS_OFF:
      return InterfaceState::busOff;
    case CAN_STATE_SLEEPING:
      return InterfaceState::sleeping;
  }

  // Normally you should never reach this point
  return InterfaceState::stopped;
}

void InterfaceConfiguration::configureBitrate() {
  if (can_set_bitrate(ifaceName.c_str(), bitrate) != 0) {
    throw system_error(EIO, system_category());
  }
}

void InterfaceConfiguration::switchTerminator(const bool state) {
  stringstream ss;
  ss << GPIO_BASEPATH;

  if (ifaceName == CAN0_INTERFACE) {
    ss << CAN0_TERMINATOR;
  } else {
    ss << CAN1_TERMINATOR;
  }

  ss << "/value";
  auto file = fopen(ss.str().c_str(), "w");

  if (file == nullptr) {
    throw runtime_error("Can't switch Terminator");
  }

  if (state) {
    fprintf(file, "1");
  } else {
    fprintf(file, "0");
  }

  fclose(file);
}

void InterfaceConfiguration::switchTransceiver(const bool state) {
  if (ifaceName != CAN0_INTERFACE) {
    return;
  }

  auto file = fopen(CAN0_TRANSCEIVER, "w");
  if (file == nullptr) {
    throw runtime_error("Can't switch CAN0 transceiver");
  }

  if (state) {
    fprintf(file, CAN0_ENABLE);
  } else {
    fprintf(file, CAN0_DISABLE);
  }

  fclose(file);
}
}  // namespace can
}  // namespace sitec
