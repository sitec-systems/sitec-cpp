// Copyright 2017, 2018 sitec systems GmbH. All rights reserved
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#pragma once

#include <string>
#include <system_error>

#include <cerrno>
#include <cstdint>
#include <cstdio>

#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

namespace sitec {
namespace serial {

class Serial {
 public:
  Serial(std::string devFile) : _devFile(devFile), _fd(0) {}

  ~Serial() {
    if (isOpen()) {
      closeInterface();
    }
  }

  bool isOpen() { return _fd != 0; }

  void openInterface(int baudrate) {
    auto ret = open(_devFile.c_str(), O_RDWR | O_NOCTTY, O_NDELAY);
    if (ret == -1) {
      throw std::system_error(EIO, std::generic_category(), "Open interface");
    }

    if (!isatty(ret)) {
      throw std::system_error(EINVAL, std::generic_category(),
                              "Open Interface device file");
    }

    _fd = ret;

    struct termios ttyConfig;

    if (tcgetattr(_fd, &ttyConfig) < 0) {
      throw std::system_error(EIO, std::generic_category(),
                              "Grep interface configuration");
    }

    setInputProcessingConfiguration(&ttyConfig);
    setOutputProcessingConfiguration(&ttyConfig);
    setLineProcessingConfiguration(&ttyConfig);
    setCharacterProcessingConfiguration(&ttyConfig);
    setBaudrate(&ttyConfig, baudrate);

    if (tcsetattr(_fd, TCSAFLUSH, &ttyConfig) < 0) {
      throw std::system_error(EIO, std::generic_category(),
                              "Setting parameters for serial interface");
    }
  }

  size_t readBuffer(uint8_t *buffer, size_t len, long timeoutSec,
                    long timeoutUSec) {
    struct timeval tv;
    tv.tv_sec = timeoutSec;
    tv.tv_usec = timeoutUSec;

    return readBuffer(buffer, len, &tv);
  }

  size_t readBuffer(uint8_t *buffer, size_t len) {
    return readBuffer(buffer, len, nullptr);
  }

  size_t writeBuffer(uint8_t *buffer, size_t len, long timeoutSec,
                     long timeoutUSec) {
    struct timeval tv;
    tv.tv_sec = timeoutSec;
    tv.tv_usec = timeoutUSec;

    return writeBuffer(buffer, len, &tv);
  }

  size_t writeBuffer(uint8_t *buffer, size_t len) {
    return writeBuffer(buffer, len, nullptr);
  }

 private:
  void closeInterface() {
    close(_fd);
    _fd = 0;
  }

  void setInputProcessingConfiguration(struct termios *ttyConfig) {
    // Input Flags -- Turn off input processing
    //
    // convert break to null byte, no CR to NL translation,
    // no NL to CR translation, don't mark parity errors or breaks
    // no input parity check, don't strip high bit off,
    // no XON/XOFF software flow control
    ttyConfig->c_iflag &=
        ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);
  }

  void setOutputProcessingConfiguration(struct termios *ttyConfig) {
    // Output flags - Turn off output processing
    //
    // no CR to NL translation, no NL to CR-NL translation,
    // no NL to CR translation, no column 0 CR suppression,
    // no Ctrl-D suppression, no fill characters, no case mapping,
    // no local output processing
    ttyConfig->c_oflag &=
        ~(OCRNL | ONLCR | ONLRET | ONOCR | OFILL | OLCUC | OPOST);
  }

  void setLineProcessingConfiguration(struct termios *ttyConfig) {
    // No line processing
    //
    // echo off, echo newline off, canonical mode off,
    // extended input processing off, signal chars off
    ttyConfig->c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
  }

  void setCharacterProcessingConfiguration(struct termios *ttyConfig) {
    // Turn off character processing
    //
    // clear current char size mask, no parity checking,
    // no output processing, force 8 bit input
    //
    ttyConfig->c_cflag &= ~(CSIZE | PARENB);
    ttyConfig->c_cflag |= CS8;
  }

  void setBaudrate(struct termios *ttyConfig, int baudrate) {
    speed_t speed;

    switch (baudrate) {
      case 9600:
        speed = B9600;
        break;
      case 115200:
        speed = B115200;
        break;
      case 230400:
        speed = B230400;
        break;

      default:
        throw std::system_error(EINVAL, std::generic_category(),
                                "Set Baudrate");
    }

    if (cfsetispeed(ttyConfig, speed) < 0 ||
        cfsetospeed(ttyConfig, speed) < 0) {
      throw std::system_error(EIO, std::generic_category(), "Set Baudrate");
    }
  }

  size_t readBuffer(uint8_t *buffer, size_t len, struct timeval *tv) {
    fd_set in;
    FD_ZERO(&in);
    FD_SET(_fd, &in);

    auto ret = select(_fd + 1, &in, nullptr, nullptr, tv);
    if (ret == -1) {
      throw std::system_error(errno, std::generic_category(),
                              "Read buffer select");
    }

    if (FD_ISSET(_fd, &in)) {
      auto rbytes = read(_fd, buffer, len);
      if (rbytes < 0) {
        throw std::system_error(errno, std::generic_category(), "Read buffer");
      }

      return rbytes;
    } else {
      return 0;
    }
  }

  size_t writeBuffer(uint8_t *buffer, size_t len, struct timeval *tv) {
    fd_set out;
    FD_ZERO(&out);
    FD_SET(_fd, &out);

    auto ret = select(_fd + 1, nullptr, &out, nullptr, tv);
    if (ret == -1) {
      throw std::system_error(errno, std::generic_category(),
                              "Write buffer select");
    }

    if (FD_ISSET(_fd, &out)) {
      auto wbytes = write(_fd, buffer, len);
      if (wbytes < 0) {
        throw std::system_error(errno, std::generic_category(), "Write buffer");
      }

      return wbytes;
    } else {
      return 0;
    }
  }

  std::string _devFile;
  int _fd;
};

}  // namespace serial
}  // namespace sitec