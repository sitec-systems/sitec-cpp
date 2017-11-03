// Copyright 2017 sitec systems GmbH. All rights reserved
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "can_frame.hpp"

#include <cstdio>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include <linux/can/raw.h>

using std::string;
using std::stringstream;
using std::vector;

namespace sitec {
namespace can {

CanFrame::CanFrame(const unsigned int id,
                   const vector<unsigned char> &data)
    : id(id), data(data) {}

CanFrame::CanFrame(struct can_frame *frame) {
  id = frame->can_id;
  for (int i = 0; i < frame->can_dlc; i++) {
    data.push_back(frame->data[i]);
  }
}

unsigned int CanFrame::getId() { return id; }

vector<unsigned char> CanFrame::getData() { return data; }

void CanFrame::getFrameStruct(struct can_frame *frame) {
  frame->can_id = id;
  frame->can_dlc = data.size();

  for (int i = 0; i < data.size(); i++) {
    frame->data[i] = data[i];
  }
}

string CanFrame::to_string() {
  char buf[96];
  stringstream ss;

  sprintf(buf, "0x%x ", id);
  ss << buf << data.size() << " -> ";

  for (int i = 0; i < data.size(); i++) {
    sprintf(buf, "[0x%02x]", data[i]);
    ss << buf;
  }

  return ss.str();
}

} // namespace can
} // namespace sitec
