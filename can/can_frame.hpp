// Copyright 2017 sitec systems GmbH. All rights reserved
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#pragma once

#include <string>
#include <vector>

#include <linux/can/raw.h>

namespace sitec {
namespace can {

//! Abstraction for CAN frame
class CanFrame {
 public:
  //! Constructur of CanFrame object
  //!
  //! @param id Contains the id of the can frame
  //! @param data std::vector which contains the payload of the can frame
  CanFrame(const unsigned int id, const std::vector<unsigned char>& data);

  //! Constructor of CanFrame object
  //!
  //! @param frame takes the c struct and copies the data from it to contruct the object
  CanFrame(struct can_frame* frame);
  
  //! Gets the ID of the CAN frame
  //!
  //! @return ID of the CAN frame
  unsigned int getId();
  
  //! Gets the std::vector with the data
  //!
  //! @return std::vector which contains the data of the CAN frame
  std::vector<unsigned char> getData();

  //! Create a can_frame struct from the CanFrame object.
  //!
  //! Please initialize the frame parameter first before using this function.
  //! @code{.cpp}
  //! struct can_frame frame;
  //! canFrame.getFrameStruct(&frame);
  //! @endcode
  //! @param frame pointer to a can_frame structure
  void getFrameStruct(struct can_frame* frame);

  //! Display the frame as std::string
  //!
  //! @return std::string object which contains the content of the frame
  std::string to_string();

 private:
  std::vector<unsigned char> data;
  unsigned int id;
};

}  // namespace can
}  // namespace sitec
