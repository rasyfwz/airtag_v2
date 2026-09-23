#ifndef PARSER_HPP
#define PARSER_HPP

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

struct Vec3 {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

struct ImuSample {
    Vec3 accel;
    Vec3 gyro;
};

// Extracts <preamble><accel 6B><gyro 6B> frames out of a raw serial byte
// stream. See device/main/imu.c for the on-wire format this expects.
class ImuFrameParser {
public:
    static constexpr uint8_t kPreamble[2] = {0xAA, 0x55};
    static constexpr size_t kAccelBytes = 6;
    static constexpr size_t kGyroBytes = 6;
    static constexpr double kAccelScale = 16384.0;  // LSB/g, default +-2g range
    static constexpr double kGyroScale = 131.0;     // LSB/(deg/s), default +-250dps range

    void feed(uint8_t byte);
    std::optional<ImuSample> tryParseFrame();

private:
    std::optional<size_t> findPreamble(size_t from) const;
    static Vec3 extractVec3(const uint8_t* buf, double scale);

    std::vector<uint8_t> buffer_;
};

#endif
