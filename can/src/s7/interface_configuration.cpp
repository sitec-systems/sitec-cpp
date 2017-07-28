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

namespace peripheral {
namespace can {

namespace {
const std::string CAN0_TERMINATOR("498");
const std::string CAN1_TERMINATOR("501");
const std::string CAN0_SLEEP("499");
const std::string CAN1_SLEEP("500");
const std::string CAN0_INTERFACE("can0");
const std::string CAN1_INTERFACE("can1");
const std::string GPIO_BASEPATH("/sys/class/gpio/gpio");
const std::string GPIO_EXPORT("/sys/class/gpio/export");
const std::string GPIO_UNEXPORT("/sys/class/gpio/unexport");

const bool isExported(std::string gpio) {
  std::stringstream ss;
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
const void exportGpio(std::string gpio) {
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
const void unexportGpio(std::string gpio) {
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

const void configureAsOutput(std::string gpio) {
  std::stringstream ss;
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
}

InterfaceConfiguration::InterfaceConfiguration(const char *ifaceName,
                                               const int bitrate,
                                               const bool terminator,
                                               const bool autoShutdown) {
  this->ifaceName = std::string(ifaceName);
  this->bitrate = bitrate;
  this->terminator = terminator;
  this->autoShutdown = autoShutdown;
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
  std::stringstream ss;
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
  std::stringstream ss;
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
}  // namespace peripheral
