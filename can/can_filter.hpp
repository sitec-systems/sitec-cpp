// Copyright 2017 sitec systems GmbH. All rights reserved
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#pragma once

namespace sitec {
namespace can {

//! Abstraction for CAN filters
class CanFilter {
 public:
  //! Constructor of CanFilter class
  //!
  //! @param id ID which will be used for filter messages
  //! @param mask Bitmask which will be used for message filtering
  CanFilter(const unsigned int id, const unsigned int mask);

  //! Gets the configured ID
  //!
  //! @return Returns the integer which represents the configured ID
  unsigned int getId();

  //! Gets the configured bitmask
  //!
  //! @return Returns the configured bitmask
  unsigned int getMask();

 private:
  unsigned int id;
  unsigned int mask;
};

}  // namespace can
}  // namespace sitec
