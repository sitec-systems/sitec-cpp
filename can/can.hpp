// Copyright 2017, 2018 sitec systems GmbH. All rights reserved
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#pragma once

#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

#include "can_filter.hpp"
#include "can_frame.hpp"
#include "timeout.hpp"

namespace sitec {
namespace can {

//! Class which abstracts a CAN socket
class Can {
 public:
  //! Constructor for Can object.
  //!
  //! @param networkInterface Takes the name of the network interface (e.g.
  //! can0)
  Can(const char* networkInterface);

  //! Destructor for Can object.
  //!
  //! Closing the open socket to clean up the system
  ~Can();

  //! Open the socket for communication
  //!
  //! @throws std::system_error if the socket can't be open properly.
  void open();

  //! Setting multiple filters for the socket
  //!
  //! @param filter std::vector object which contains CanFilter objects
  //! @throws std::system_error if the filter can't be set.
  void setFilter(const std::vector<CanFilter>& filter);
  void setFilter(const CanFilter& filter);
  void setRecvTimeout(Timeout timeout);
  void setSendTimeout(Timeout timeout);
  void disableRecvTimeout();
  void enableRecvTimeout();
  void disableFilter();
  CanFrame receiveFrame();
  void sendFrame(CanFrame& frame);
  void destroy();

 private:
  std::string networkInterface;
  unsigned int bitrate;
  int sock = 0;
  Timeout recvTimeout;
  Timeout sendTimeout;
  bool recvTimeoutEnable;

  int doRecvSelect(fd_set* fds);
};

}  // namespace can
}  // namespace sitec
