#pragma once

#include <string>
#include <vector>

#include <linux/can/raw.h>

namespace peripheral {
namespace can {

class CanFrame {
 public:
  CanFrame(const unsigned int id, const std::vector<unsigned char>& data);
  CanFrame(struct can_frame* frame);
  unsigned int getId();
  std::vector<unsigned char> getData();
  void getFrameStruct(struct can_frame* frame);
  std::string to_string();

 private:
  std::vector<unsigned char> data;
  unsigned int id;
};

}  // namespace can
}  // namespace peripheral
