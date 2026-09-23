#include "parser.hpp"

void ImuFrameParser::feed(uint8_t byte) {
    buffer_.push_back(byte);
}

std::optional<ImuSample> ImuFrameParser::tryParseFrame() {
    auto start = findPreamble(0);
    if (!start) {
        return std::nullopt;
    }

    auto end = findPreamble(*start + 2);
    if (!end) {
        return std::nullopt;
    }

    const size_t accelStart = *start + 2;
    const size_t gyroStart = accelStart + kAccelBytes;
    const size_t frameEnd = gyroStart + kGyroBytes;

    if (buffer_.size() < frameEnd) {
        return std::nullopt;
    }

    ImuSample sample;
    sample.accel = extractVec3(&buffer_[accelStart], kAccelScale);
    sample.gyro = extractVec3(&buffer_[gyroStart], kGyroScale);

    buffer_.erase(buffer_.begin(), buffer_.begin() + static_cast<long>(*end));

    return sample;
}

std::optional<size_t> ImuFrameParser::findPreamble(size_t from) const {
    if (from >= buffer_.size()) {
        return std::nullopt;
    }
    for (size_t i = from; i + 1 < buffer_.size(); ++i) {
        if (buffer_[i] == kPreamble[0] && buffer_[i + 1] == kPreamble[1]) {
            return i;
        }
    }
    return std::nullopt;
}

Vec3 ImuFrameParser::extractVec3(const uint8_t* buf, double scale) {
    Vec3 v;
    v.x = static_cast<int16_t>((buf[0] << 8) | buf[1]) / scale;
    v.y = static_cast<int16_t>((buf[2] << 8) | buf[3]) / scale;
    v.z = static_cast<int16_t>((buf[4] << 8) | buf[5]) / scale;
    return v;
}
