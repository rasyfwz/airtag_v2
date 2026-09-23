#include "serial.hpp"

#include <stdexcept>

#include <fcntl.h>
#include <unistd.h>

SerialPort::SerialPort(const std::string& path, speed_t baud) {
    fd_ = open(path.c_str(), O_RDONLY | O_NOCTTY);
    if (fd_ < 0) {
        throw std::runtime_error("Failed to open serial port: " + path);
    }

    termios tty{};
    if (tcgetattr(fd_, &tty) != 0) {
        close(fd_);
        throw std::runtime_error("tcgetattr failed on " + path);
    }

    cfsetispeed(&tty, baud);
    cfsetospeed(&tty, baud);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~(PARENB | CSTOPB);

    tty.c_lflag = 0;
    tty.c_oflag = 0;
    tty.c_iflag &= ~(IXON | IXOFF | IXANY | ICRNL);

    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 0;

    if (tcsetattr(fd_, TCSANOW, &tty) != 0) {
        close(fd_);
        throw std::runtime_error("tcsetattr failed on " + path);
    }
}

SerialPort::~SerialPort() {
    if (fd_ >= 0) {
        close(fd_);
    }
}

std::optional<uint8_t> SerialPort::readByte() {
    uint8_t byte = 0;
    ssize_t n = read(fd_, &byte, 1);
    if (n <= 0) {
        return std::nullopt;
    }
    return byte;
}
