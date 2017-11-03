// Copyright 2017 sitec systems GmbH. All rights reserved
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#pragma once

#include <string>

#include "interface_state.hpp"

namespace sitec {
namespace can {

static const bool ENABLE_TERMINATOR = true;
static const bool DONT_ENABLE_TERMINATOR = false;

//! Abstraction for the system interface

//! Class abstracts the CAN interfaces of the system and makes it possible to configure this interfaces
//! from the application.
class InterfaceConfiguration {
public:
  //! Constructor.
  //!
  //! @param name Name of the CAN interface (e.g. can0)
  //! @param bitrate bitrate for the can interface
  //! @param terminator if true switch the terminator resistor if driving the interface up
  InterfaceConfiguration ( const char *name, const int bitrate,
                           const bool terminator );

  //! Destructor.
  //!
  //! Driving the interface down if leaving the scope of the InterfaceConfiguration
  ~InterfaceConfiguration();

  //! Driving the interface up.
  //!
  //! Configure bitrate for the CAN interface, switch the terminator if configured and
  //! driving the network interface up.
  //! @throws std::system_error if the bitrate can't be set properly or the application is
  //! unable to driver the network interface up
  void up();

  //! Driving the interface down
  //!
  //! Driving the network interface down
  //! @throws std::system_error if the network interface can't drive down properly
  void down();

  //! Get's the current state of the networking interface
  //!
  //! @return InterfaceState enum class which shows the state of the interface
  //! @throws std::system_error if the call to the system api fails
  InterfaceState getState();

  //! Switching the terminator
  //!
  //! @param state if true activate the terminator resistor, if false deactivate terminator resistor
  //! @throws std::runtime_error if the gpio file for the terminator resistor is not availible or not
  //! writable
  void switchTerminator ( const bool state );

private:
  void configureBitrate();
  void switchTransceiver ( const bool state );

  std::string ifaceName;
  bool terminator;
  int bitrate;
};

} // namespace can
} // namespace sitec
