#include "fusion.hpp"

#include <cmath>

namespace {
constexpr double kRadToDeg = 180.0 / M_PI;
}

ComplementaryFilter::ComplementaryFilter(double alpha) : alpha_(alpha) {}

Orientation ComplementaryFilter::update(const Vec3& accel, const Vec3& gyro, double dt) {
    const double accelRoll = std::atan2(accel.y, accel.z) * kRadToDeg;
    const double accelPitch =
        std::atan2(-accel.x, std::sqrt(accel.y * accel.y + accel.z * accel.z)) * kRadToDeg;

    if (!initialized_) {
        orientation_.roll = accelRoll;
        orientation_.pitch = accelPitch;
        orientation_.yaw = 0.0;
        initialized_ = true;
        return orientation_;
    }

    const double gyroRoll = orientation_.roll + gyro.x * dt;
    const double gyroPitch = orientation_.pitch + gyro.y * dt;

    orientation_.roll = alpha_ * gyroRoll + (1.0 - alpha_) * accelRoll;
    orientation_.pitch = alpha_ * gyroPitch + (1.0 - alpha_) * accelPitch;
    orientation_.yaw += gyro.z * dt;

    return orientation_;
}
