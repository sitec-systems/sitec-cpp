#include <exception>
#include <future>
#include <iostream>

#include <can.hpp>
#include <can_filter.hpp>
#include <can_frame.hpp>
#include <interface_configuration.hpp>

using namespace peripheral::can;
using namespace std;

namespace {
const CanFilter FILTER(0x123, 0xffff);
}

int main(int ac, char **av) {
  try {
    InterfaceConfiguration if0("can2", 500000, true, true);
    InterfaceConfiguration if1("can1", 500000, false, true);
    if0.up();
    if1.up();

    Can can0("can0");
    Can can1("can1");

    can0.openInterface();
    can1.openInterface();
    can1.setFilter(FILTER);

    CanFrame frame1(0x123, {0xc});
    CanFrame frame2(0x003, {0xfc, 0x0e});
    can0.sendFrame(frame1);
    cout << "Sending frame through can0 " << frame1.to_string() << '\n';
    can0.sendFrame(frame2);
    cout << "Sending frame through can0 " << frame2.to_string() << '\n';

    auto frame = can1.receiveFrame();
    cout << "Received frame on can1 " << frame.to_string() << '\n';

  } catch (exception &e) {
    cout << e.what() << '\n';
  }

  return 0;
}
