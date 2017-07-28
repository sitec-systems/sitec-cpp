#pragma once

#include <string>

#include "interface_state.hpp"

namespace peripheral {
namespace can {

class InterfaceConfiguration {
public:
  InterfaceConfiguration(const char *name, const int bitrate,
                         const bool terminator, const bool autoShutdown);
  ~InterfaceConfiguration();
  void up();
  void down();
  InterfaceState getState();
  void switchTerminator(const bool state);

private:
  void configureBitrate();
  void switchTransceiver(const bool state);

  std::string ifaceName;
  bool terminator;
  bool autoShutdown;
  int bitrate;
};

} // namespace can
} // namespace peripheral
