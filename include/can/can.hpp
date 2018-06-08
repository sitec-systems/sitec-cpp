// Copyright 2017, 2018 sitec systems GmbH. All rights reserved
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#pragma once

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>

#include <errno.h>
#include <fcntl.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

namespace sitec {
namespace can {

//! Abstraction for CAN frame
class CanFrame {
 public:
  //! Constructur of CanFrame object
  //!
  //! @param id Contains the id of the can frame
  //! @param data std::vector which contains the payload of the can frame
  CanFrame(const canid_t id, const std::vector<uint8_t>& data,
           bool isRTR = false, bool isERR = false)
      : id(id), data(data), _isRTR(isRTR), _isERR(false) {
    if ((id & CAN_SFF_MASK) == id) {
      _isEFF = false;
    } else {
      _isEFF = true;
    }
  }

  //! Constructor of CanFrame object
  //!
  //! @param frame takes the c struct and copies the data from it to contruct
  //! the object
  CanFrame(struct can_frame* frame) {
    id = frame->can_id;
    if ((id & CAN_EFF_FLAG) == CAN_EFF_FLAG) {
      _isEFF = true;
      id &= ~(CAN_EFF_FLAG);
    } else {
      _isEFF = false;
    }

    if (id & CAN_ERR_FLAG) {
      _isERR = true;
      id &= ~(CAN_ERR_FLAG);
    } else {
      _isERR = false;
    }

    if (id & CAN_RTR_FLAG) {
      _isRTR = true;
      id &= ~(CAN_RTR_FLAG);
    } else {
      _isRTR = false;
    }

    for (int i = 0; i < frame->can_dlc; i++) {
      data.push_back(frame->data[i]);
    }
  }

  //! Gets the ID of the CAN frame
  //!
  //! @return ID of the CAN frame
  uint16_t getId() { return id; }

  //! Gets the std::vector with the data
  //!
  //! @return std::vector which contains the data of the CAN frame
  std::vector<uint8_t> getData() { return data; }

  //! Create a can_frame struct from the CanFrame object.
  //!
  //! Please initialize the frame parameter first before using this function.
  //! @code{.cpp}
  //! struct can_frame frame;
  //! canFrame.getFrameStruct(&frame);
  //! @endcode
  //! @param frame pointer to a can_frame structure
  void getFrameStruct(struct can_frame* frame) {
    frame->can_id = id;
    frame->can_dlc = data.size();

    for (int i = 0; i < data.size(); i++) {
      frame->data[i] = data[i];
    }
  }

  //! Display the frame as std::string
  //!
  //! @return std::string object which contains the content of the frame
  std::string toString() {
    char buf[96];
    std::stringstream ss;

    sprintf(buf, "0x%x ", id);
    ss << buf;
    if (_isEFF) {
      ss << " EFF ";
    }

    if (_isRTR) {
      ss << " RTR ";
    }

    if (_isERR) {
      ss << " ERR ";
    }

    ss << data.size() << " -> ";

    for (int i = 0; i < data.size(); i++) {
      sprintf(buf, "[0x%02x]", data[i]);
      ss << buf;
    }

    return ss.str();
  }

  bool isRTR() { return _isRTR; }

  bool isEFF() { return _isEFF; }

  bool isERR() { return _isERR; }

 private:
  std::vector<uint8_t> data;
  canid_t id;
  bool _isRTR;
  bool _isEFF;
  bool _isERR;
};

//! Abstraction for CAN filters
class CanFilter {
 public:
  //! Constructor of CanFilter class
  //!
  //! @param id ID which will be used for filter messages
  //! @param mask Bitmask which will be used for message filtering
  CanFilter(const unsigned int id, const unsigned int mask)
      : id(id), mask(mask) {}

  //! Gets the configured ID
  //!
  //! @return Returns the integer which represents the configured ID
  unsigned int getId() { return id; }

  //! Gets the configured bitmask
  //!
  //! @return Returns the configured bitmask
  unsigned int getMask() { return mask; }

 private:
  unsigned int id;
  unsigned int mask;
};

class Timeout {
 public:
  //! Constructor
  //!
  //! @param timeoutSec Timeout in seconds
  //! @param timeoutUsec Timout in microseconds
  Timeout(const uint32_t timeoutSec, const uint32_t timeoutUsec)
      : timeoutSec(timeoutSec), timeoutUsec(timeoutUsec) {}

  //! Converts the class into a struct timeval.
  //!
  //! @brief Memberfunction converts the class into a struct timeval. This
  //! is used by the most system calls to determine timeouts
  //! @return struct timeval
  struct timeval toTimevalStruct() {
    struct timeval tv;

    tv.tv_sec = timeoutSec;
    tv.tv_usec = timeoutUsec;

    return tv;
  }

 private:
  uint32_t timeoutSec;
  uint32_t timeoutUsec;
};

namespace {

const Timeout INIT_TIMEOUT(0, 0);

}  // namespace

//! Class which abstracts a CAN socket
class Can {
 public:
  //! Constructor for Can object.
  //!
  //! @param networkInterface Takes the name of the network interface (e.g.
  //! can0)
  Can(const char* networkInterface)
      : recvTimeout(INIT_TIMEOUT),
        sendTimeout(INIT_TIMEOUT),
        recvTimeoutEnable(false),
        sendTimeoutEnable(false) {
    this->networkInterface = std::string(networkInterface);
  }

  //! Destructor for Can object.
  //!
  //! Closing the open socket to clean up the system
  ~Can() {
    if (sock != 0) {
      destroy();
    }
  }

  //! Open the socket for communication
  //!
  //! @throws std::system_error if the socket can't be open properly.
  void open() {
    if (sock != 0) {
      // already open
      return;
    }

    auto s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (s <= 0) {
      throw std::system_error(EIO, std::system_category());
    }

    sock = s;

    struct ifreq ifr;
    strcpy(ifr.ifr_name, networkInterface.c_str());
    auto ret = ioctl(s, SIOCGIFINDEX, &ifr);
    if (ret < 0) {
      destroy();
      throw std::system_error(EIO, std::system_category());
    }

    struct sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    ret = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (ret < 0) {
      destroy();
      throw std::system_error(EIO, std::system_category());
    }
  }

  //! Setting multiple filters for the socket
  //!
  //! @param filter std::vector object which contains CanFilter objects
  //! @throws std::system_error if the filter can't be set.
  void setFilter(const std::vector<CanFilter>& filter) {
    std::vector<struct can_filter> ll_filter;

    for (auto f : filter) {
      struct can_filter ll_f = {
          .can_id = f.getId(),
          .can_mask = f.getMask(),
      };
      ll_filter.push_back(ll_f);
    }

    auto ret = setsockopt(sock, SOL_CAN_RAW, CAN_RAW_FILTER, ll_filter.data(),
                          ll_filter.size() * sizeof(struct can_filter));
    if (ret < 0) {
      throw std::system_error(EIO, std::system_category());
    }
  }

  void setFilter(const CanFilter& filter) {
    std::vector<CanFilter> filter_list{filter};
    setFilter(filter_list);
  }

  void disableFilter() {
    CanFilter filter(0x7ff, 0);
    setFilter(filter);
  }

  CanFrame receiveFrame() {
    if (sock == 0) {
      throw std::runtime_error("Socket is not open");
    }

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(sock, &fds);

    auto ret = doRecvSelect(&fds);
    if (ret == -1) {
      throw std::system_error(errno, std::system_category());
    } else if (ret == 0) {
      throw std::system_error(ETIMEDOUT, std::system_category());
    }

    struct can_frame frame;
    ret = read(sock, &frame, sizeof(struct can_frame));
    if (ret < 0) {
      throw std::system_error(errno, std::system_category());
    }

    return CanFrame(&frame);
  }

  void sendFrame(CanFrame& frame) {
    if (sock == 0) {
      throw std::system_error(EINVAL, std::generic_category(), "Send Frame");
    }

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(sock, &fds);

    auto ret = doSendSelect(&fds);
    if (ret == -1) {
      throw std::system_error(errno, std::generic_category(),
                              "Send Frame Select");
    } else if (ret == 0) {
      throw std::system_error(ETIMEDOUT, std::generic_category(),
                              "Send Frame Select");
    }

    struct can_frame ll_frame;
    frame.getFrameStruct(&ll_frame);

    ret = write(sock, &ll_frame, sizeof(struct can_frame));

    if (ret < 0) {
      if (errno == ETIMEDOUT) {
        throw std::system_error(ETIMEDOUT, std::system_category(),
                                "Send Frame Write");
      } else {
        throw std::system_error(errno, std::system_category(),
                                "Send Frame Write");
      }
    }
  }

  void setRecvTimeout(Timeout timeout) { recvTimeout = timeout; }

  void setSendTimeout(Timeout timeout) { sendTimeout = timeout; }

  void disableRecvTimeout() { recvTimeoutEnable = false; }

  void enableRecvTimeout() { recvTimeoutEnable = true; }

  void enableSendTimeout() { sendTimeoutEnable = true; }

  void disableSendTimeout() { sendTimeoutEnable = false; }

  void destroy() {
    close(sock);
    sock = 0;
  }

 private:
  int doRecvSelect(fd_set* fds) {
    if (recvTimeoutEnable) {
      auto tv = recvTimeout.toTimevalStruct();
      return select(sock + 1, fds, nullptr, nullptr, &tv);
    }

    return select(sock + 1, fds, nullptr, nullptr, nullptr);
  }

  int doSendSelect(fd_set* fds) {
    if (sendTimeoutEnable) {
      auto tv = sendTimeout.toTimevalStruct();
      return select(sock + 1, nullptr, fds, nullptr, &tv);
    }

    return select(sock + 1, nullptr, fds, nullptr, nullptr);
  }

  std::string networkInterface;
  unsigned int bitrate;
  int sock = 0;
  Timeout recvTimeout;
  Timeout sendTimeout;
  bool recvTimeoutEnable;
  bool sendTimeoutEnable;
};  // namespace can

}  // namespace can
}  // namespace sitec
