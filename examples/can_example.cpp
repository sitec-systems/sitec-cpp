// Copyright 2017 sitec systems GmbH. All rights reserved
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <exception>
#include <future>
#include <iostream>
#include <system_error>

#include <can/can.hpp>
#include <can/can_filter.hpp>
#include <can/can_frame.hpp>
#include <can/interface_configuration.hpp>

using namespace sitec::can;
using namespace std;

namespace {
const CanFilter FILTER(0x123, 0xffff);
}

int main(int ac, char **av) {
  try {
    InterfaceConfiguration if0("can0", 500000, ENABLE_TERMINATOR);
    InterfaceConfiguration if1("can1", 500000, DONT_ENABLE_TERMINATOR);
    if0.up();
    if1.up();

    Can can0("can0");
    Can can1("can1");

    can0.open();
    can1.open();
    can1.setFilter(FILTER);
    can1.setRecvTimeout(Timeout(2, 0));

    CanFrame frame1(0x123, {0xc});
    CanFrame frame2(0x003, {0xfc, 0x0e});
    can0.sendFrame(frame1);
    cout << "Sending frame through can0 " << frame1.to_string() << '\n';
    can0.sendFrame(frame2);
    cout << "Sending frame through can0 " << frame2.to_string() << '\n';

    auto frame = can1.receiveFrame();
    cout << "Received frame on can1 " << frame.to_string() << '\n';

    can1.receiveFrame();  // This one times out

  } catch (system_error &e) {
    if (e.code() == errc::timed_out) {
      cout << "Receiving is timedout\n";
    } else {
      cout << e.what() << '\n';
    }
  }

  return 0;
}
