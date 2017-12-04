// Copyright 2017 sitec systems GmbH. All rights reserved
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#pragma once

#include <cstdint>

#include <sys/time.h>

namespace sitec {
namespace can {

//! Timeout abstraction used for send and receive timeout
class Timeout {
 public:
  //! Constructor
  //!
  //! @param timeoutSec Timeout in seconds
  //! @param timeoutUsec Timout in microseconds
  Timeout(const uint32_t timeoutSec, const uint32_t timeoutUsec);

  //! Converts the class into a struct timeval.
  //!
  //! @brief Memberfunction converts the class into a struct timeval. This
  //! is used by the most system calls to determine timeouts
  //! @return struct timeval
  struct timeval toTimevalStruct();

 private:
  uint32_t timeoutSec;
  uint32_t timeoutUsec;
};

}  // namespace can
}  // namespace sitec