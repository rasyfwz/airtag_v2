#ifndef FUSION_HPP
#define FUSION_HPP

#include "parser.hpp"

// All angles in degrees. Roll/pitch are accelerometer+gyro fused; yaw is
// gyro-only (no magnetometer in this sensor) and will drift over time.
struct Orientation {
    double roll = 0.0;
    double pitch = 0.0;
    double yaw = 0.0;
};

// Complementary filter: blends the low-drift, high-noise accelerometer tilt
// estimate with the low-noise, drifting gyro integration.
class ComplementaryFilter {
public:
    explicit ComplementaryFilter(double alpha = 0.98);

    // gyro is expected in deg/s (matches ImuFrameParser::kGyroScale), dt in seconds.
    Orientation update(const Vec3& accel, const Vec3& gyro, double dt);

    const Orientation& orientation() const { return orientation_; }

private:
    double alpha_;
    Orientation orientation_;
    bool initialized_ = false;
};

#endif
