# IMU Orientation Visualizer

![demo](https://media1.giphy.com/media/v1.Y2lkPTc5MGI3NjExaDl3aDEwN203cmk2MXcxdXZ0cnRpeGU1c2N6cmplM21tNjB3MWVwNiZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/PORlWw46jU9TeE3flM/giphy.gif)

## Firmware
To install the firmware, follow the guidelines to install the ESP-IDF SDK.

The firmware architecture itself is simple, a super loop that uses I2C to communicate with the MPU6500. Adds preamble to send raw data through UART. Make sure to have a TTL-to-USB to use the RX and TX cables to send raw data. The port used to flash can be used for logs and debugging as well.

## Software
The software is created in C++. Simply run the Makefile with "make" in the host directory. In your terminal, run "./imu_monitor" followed with the USB port your TTL-to-USB cable is connected to.