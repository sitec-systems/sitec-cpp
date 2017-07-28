#include "can_frame.hpp"

#include <cstdio>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include <linux/can/raw.h>

namespace peripheral {
namespace can {

CanFrame::CanFrame(const unsigned int id,
                   const std::vector<unsigned char> &data)
    : id(id), data(data) {}

CanFrame::CanFrame(struct can_frame *frame) {
  id = frame->can_id;
  for (int i = 0; i < frame->can_dlc; i++) {
    data.push_back(frame->data[i]);
  }
}

unsigned int CanFrame::getId() { return id; }

std::vector<unsigned char> CanFrame::getData() { return data; }

void CanFrame::getFrameStruct(struct can_frame *frame) {
  frame->can_id = id;
  frame->can_dlc = data.size();

  for (int i = 0; i < data.size(); i++) {
    frame->data[i] = data[i];
  }
}

std::string CanFrame::to_string() {
  char buf[96];
  std::stringstream ss;

  sprintf(buf, "0x%x ", id);
  ss << buf << data.size() << " -> ";

  for (int i = 0; i < data.size(); i++) {
    sprintf(buf, "[0x%02x]", data[i]);
    ss << buf;
  }

  return ss.str();
}

} // namespace can
} // namespace peripheral
