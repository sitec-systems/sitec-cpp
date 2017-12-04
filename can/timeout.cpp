// Copyright 2017 sitec systems GmbH. All rights reserved
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "timeout.hpp"

#include <cstdint>

#include <sys/time.h>

using namespace std;

namespace sitec {
namespace can {

Timeout::Timeout(const uint32_t timeoutSec, const uint32_t timeoutUsec)
    : timeoutSec(timeoutSec), timeoutUsec(timeoutUsec) {}

struct timeval Timeout::toTimevalStruct() {
  struct timeval tv;

  tv.tv_sec = timeoutSec;
  tv.tv_usec = timeoutUsec;

  return tv;
}

}  // namespace can
}  // namespace sitec