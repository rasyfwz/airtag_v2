#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <cstdint>
#include <optional>
#include <string>

#include <termios.h>

class SerialPort {
public:
    SerialPort(const std::string& path, speed_t baud);
    ~SerialPort();

    SerialPort(const SerialPort&) = delete;
    SerialPort& operator=(const SerialPort&) = delete;

    std::optional<uint8_t> readByte();

private:
    int fd_ = -1;
};

#endif
