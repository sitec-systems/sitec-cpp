// Copyright 2017 sitec systems GmbH. All rights reserved
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "can.hpp"

#include <cstring>
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

#include "can_filter.hpp"
#include "can_frame.hpp"
#include "timeout.hpp"

using std::string;
using std::system_error;
using std::system_category;
using std::vector;
using std::runtime_error;

namespace sitec {
namespace can {

Can::Can(const char *networkInterface) {
  this->networkInterface = string(networkInterface);
}

Can::~Can() {
  if (sock != 0) {
    destroy();
  }
}

void Can::open() {
  if (sock != 0) {
    // already open
    return;
  }

  auto s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
  if (s <= 0) {
    throw system_error(EIO, system_category());
  }

  sock = s;

  struct ifreq ifr;
  strcpy(ifr.ifr_name, networkInterface.c_str());
  auto ret = ioctl(s, SIOCGIFINDEX, &ifr);
  if (ret < 0) {
    destroy();
    throw system_error(EIO, system_category());
  }

  struct sockaddr_can addr;
  addr.can_family = AF_CAN;
  addr.can_ifindex = ifr.ifr_ifindex;
  ret = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
  if (ret < 0) {
    destroy();
    throw system_error(EIO, system_category());
  }
}

void Can::destroy() {
  close(sock);
  sock = 0;
}

void Can::setFilter(const vector<CanFilter> &filter) {
  vector<struct can_filter> ll_filter;

  for (auto f : filter) {
    struct can_filter ll_f = {
        .can_id = f.getId(), .can_mask = f.getMask(),
    };
    ll_filter.push_back(ll_f);
  }

  auto ret = setsockopt(sock, SOL_CAN_RAW, CAN_RAW_FILTER, ll_filter.data(),
                        ll_filter.size() * sizeof(struct can_filter));
  if (ret < 0) {
    throw system_error(EIO, system_category());
  }
}

void Can::setFilter(const CanFilter &filter) {
  vector<CanFilter> filter_list{filter};
  setFilter(filter_list);
}

void Can::disableFilter() {
  CanFilter filter(0x7ff, 0);
  setFilter(filter);
}

CanFrame Can::receiveFrame() {
  if (sock == 0) {
    throw runtime_error("Socket is not open");
  }

  struct can_frame frame;
  auto ret = read(sock, &frame, sizeof(struct can_frame));
  if (ret < 0) {
    throw system_error(errno, system_category());
  }

  return CanFrame(&frame);
}

void Can::sendFrame(CanFrame &frame) {
  if (sock == 0) {
    throw runtime_error("Socket is not open");
  }

  struct can_frame ll_frame;
  frame.getFrameStruct(&ll_frame);

  auto ret = write(sock, &ll_frame, sizeof(struct can_frame));

  if (ret < 0) {
    if (errno == ETIMEDOUT) {
      throw system_error(ETIMEDOUT, system_category());
    } else {
      throw system_error(EIO, system_category());
    }
  }
}

void Can::setRecvTimeout(Timeout timeout) {
  auto tv = timeout.toTimevalStruct();
  auto ret = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv));
  if (ret != 0) {
    throw system_error(EIO, system_category());
  }
}

void Can::setSendTimeout(Timeout timeout) {
  auto tv = timeout.toTimevalStruct();
  auto ret = setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(tv));
  if (ret != 0) {
    throw system_error(EIO, system_category());
  }
}

}  // namespace can
}  // namespace sitec
