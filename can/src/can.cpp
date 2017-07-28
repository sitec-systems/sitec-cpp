#include "can.hpp"

#include <cstring>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>

#include <fcntl.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <libsocketcan.h>

#include "can_filter.hpp"
#include "can_frame.hpp"

namespace peripheral {
namespace can {

Can::Can(const char *networkInterface) {
  this->networkInterface = std::string(networkInterface);
}

Can::~Can() {
  if (sock != 0) {
    closeInterface();
  }
}

void Can::openInterface() {
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
    closeInterface();
    throw std::system_error(EIO, std::system_category());
  }

  struct sockaddr_can addr;
  addr.can_family = AF_CAN;
  addr.can_ifindex = ifr.ifr_ifindex;
  ret = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
  if (ret < 0) {
    closeInterface();
    throw std::system_error(EIO, std::system_category());
  }
}

void Can::configureInterface() {
  int interfaceState = 0;

  auto ret = can_get_state(networkInterface.c_str(), &interfaceState);
  if (ret != 0) {
    throw std::system_error(EIO, std::system_category());
  }

  if (interfaceState != CAN_STATE_STOPPED) {
    can_do_stop(networkInterface.c_str());
  }

  ret = can_set_bitrate(networkInterface.c_str(), bitrate);
  if (ret != 0) {
    throw std::system_error(EIO, std::system_category());
  }

  ret = can_do_start(networkInterface.c_str());
  if (ret != 0) {
    throw std::system_error(EIO, std::system_category());
  }
}

void Can::closeInterface() {
  close(sock);
  sock = 0;
}

void Can::setFilter(const std::vector<CanFilter> &filter) {
  std::vector<struct can_filter> ll_filter;

  for (auto f : filter) {
    struct can_filter ll_f = {
        .can_id = f.getId(), .can_mask = f.getMask(),
    };
    ll_filter.push_back(ll_f);
  }

  auto ret = setsockopt(sock, SOL_CAN_RAW, CAN_RAW_FILTER, ll_filter.data(),
                        ll_filter.size() * sizeof(struct can_filter));
  if (ret < 0) {
    throw std::system_error(EIO, std::system_category());
  }
}

void Can::setFilter(const CanFilter &filter) {
  std::vector<CanFilter> filter_list{filter};
  setFilter(filter_list);
}

void Can::disableFilter() {
  CanFilter filter(0x7ff, 0);
  setFilter(filter);
}

CanFrame Can::receiveFrame() {
  if (sock == 0) {
    throw std::runtime_error("Socket is not open");
  }

  struct can_frame frame;
  auto ret = read(sock, &frame, sizeof(struct can_frame));
  if (ret < 0) {
    throw std::system_error(EIO, std::system_category());
  }

  return CanFrame(&frame);
}

void Can::sendFrame(CanFrame &frame) {
  if (sock == 0) {
    throw std::runtime_error("Socket is not open");
  }

  struct can_frame ll_frame;
  frame.getFrameStruct(&ll_frame);

  auto ret = write(sock, &ll_frame, sizeof(struct can_frame));
  if (ret < 0) {
    throw std::system_error(EIO, std::system_category());
  }
}

} // namespace can
} // namespace peripheral
