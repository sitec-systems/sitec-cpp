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

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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
const string CAN0_TERMINATOR("498");
const string CAN1_TERMINATOR("501");
const string CAN0_SLEEP("499");
const string CAN1_SLEEP("500");
const string CAN0_INTERFACE("can0");
const string CAN1_INTERFACE("can1");
const string GPIO_BASEPATH("/sys/class/gpio/gpio");
const string GPIO_EXPORT("/sys/class/gpio/export");
const string GPIO_UNEXPORT("/sys/class/gpio/unexport");

const bool isExported(string gpio) {
  stringstream ss;
  ss << GPIO_BASEPATH << gpio;

  struct stat buffer;
  auto ret = stat(ss.str().c_str(), &buffer);
  if (ret != 0) {
    return false;
  }

  return true;
}

/// @brief Export a GPIO to userspace via sysfs
///
/// @param gpio String with the number of the gpio, e. g. 499
const void exportGpio(string gpio) {
  if (isExported(gpio)) {
    return;
  }

  auto f = fopen(GPIO_EXPORT.c_str(), "w");
  if (!f) {
    throw std::runtime_error("Can't export GPIO");
  }

  auto ret = fprintf(f, "%s", gpio.c_str());
  if (ret < 0) {
    fclose(f);
    throw std::runtime_error("Can't export GPIO");
  }

  fclose(f);

  return;
}

/// @brief Unexport a GPIO from userspace via sysfs
///
/// @param gpio String with the number of the gpio, e. g. 499
const void unexportGpio(string gpio) {
  auto f = fopen(GPIO_UNEXPORT.c_str(), "w");
  if (!f) {
    throw std::runtime_error("Can't export GPIO");
  }

  auto ret = fprintf(f, "%s", gpio.c_str());
  if (ret < 0) {
    fclose(f);
    throw std::runtime_error("Can't export GPIO");
  }

  fclose(f);

  return;
}

const void configureAsOutput(string gpio) {
  stringstream ss;
  ss << GPIO_BASEPATH;
  ss << gpio;

  ss << "/direction";

  auto f = fopen(ss.str().c_str(), "w");
  if (!f) {
    throw std::runtime_error("Can't open direction file for " + gpio);
  }

  auto ret = fprintf(f, "out");
  if (ret < 0) {
    fclose(f);
    throw std::runtime_error("Can't change direction for GPIO " + gpio);
  }

  fclose(f);
}
} // namespace

InterfaceConfiguration::InterfaceConfiguration(const char *ifaceName,
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
    throw std::system_error(EIO, std::system_category());
  }

  if (ifaceName == CAN0_INTERFACE) {
    exportGpio(CAN0_SLEEP);
    exportGpio(CAN0_TERMINATOR);
    configureAsOutput(CAN0_SLEEP);
    configureAsOutput(CAN0_TERMINATOR);
  } else {
    exportGpio(CAN1_SLEEP);
    exportGpio(CAN1_TERMINATOR);
    configureAsOutput(CAN1_SLEEP);
    configureAsOutput(CAN1_TERMINATOR);
  }

  switchTerminator(terminator);
  switchTransceiver(true);
}

void InterfaceConfiguration::down() {
  auto ret = can_do_stop(ifaceName.c_str());
  if (ret != 0) {
    throw std::system_error(EIO, std::system_category());
  }

  switchTerminator(false);
  switchTransceiver(false);

  if (ifaceName == CAN0_INTERFACE) {
    unexportGpio(CAN0_SLEEP);
    unexportGpio(CAN0_TERMINATOR);
  } else {
    unexportGpio(CAN1_SLEEP);
    unexportGpio(CAN1_TERMINATOR);
  }
}

InterfaceState InterfaceConfiguration::getState() {
  int state = 0;

  auto ret = can_get_state(ifaceName.c_str(), &state);
  if (ret != 0) {
    throw std::system_error(EIO, std::system_category());
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
    throw std::system_error(EIO, std::system_category());
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
    throw std::runtime_error("Can't switch Terminator");
  }

  if (state) {
    fprintf(file, "1");
  } else {
    fprintf(file, "0");
  }

  fclose(file);
}

void InterfaceConfiguration::switchTransceiver(const bool state) {
  stringstream ss;
  ss << GPIO_BASEPATH;

  if (ifaceName == CAN0_INTERFACE) {
    ss << CAN0_SLEEP;
  } else {
    ss << CAN1_SLEEP;
  }

  ss << "/value";
  auto file = fopen(ss.str().c_str(), "w");

  if (file == nullptr) {
    throw std::runtime_error("Can't switch Terminator");
  }

  if (state) {
    fprintf(file, "1");
  } else {
    fprintf(file, "0");
  }

  fclose(file);
}

}  // namespace can
}  // namespace sitec
