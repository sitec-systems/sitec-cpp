#include <cstdint>
#include <cstdio>

#include <serial/serial.hpp>

using namespace sitec::serial;
using namespace std;

int main(int ac, char** av) {
  Serial serial("/dev/ttyACM0");
  serial.openInterface(115200);

  uint8_t cmd[] = "ATI\r\n";
  uint8_t resp[100];

  serial.writeBuffer(cmd, sizeof(cmd), 5, 0);
  auto rbytes = serial.readBuffer(resp, 100);
  resp[rbytes] = '\0';

  printf("Response\n%s", resp);

  return 0;
}