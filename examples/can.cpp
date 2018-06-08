// Copyright (c) 2018 sitec systems GmbH

#include <cstdio>
#include <exception>

#include <can/can.hpp>

using namespace std;
using namespace sitec::can;

void traffic() {
  Can can("can0");
  CanFrame frame(0x123, {});
  try {
    can.open();
    can.setSendTimeout(Timeout(0, 1000));
    can.enableSendTimeout();

    can.sendFrame(frame);
    for (int i = 0; i < 100; i++) {
      can.sendFrame(frame);
    }
  } catch (exception &ex) {
    fprintf(stderr, "Fatal: %s\n", ex.what());
  }
}

int main(int ac, char **av) {
  Can can("can0");

  can.open();
  can.disableRecvTimeout();

  for (;;) {
    auto f = can.receiveFrame();

    printf("%s\n", f.toString().c_str());
  }

  return 0;
}