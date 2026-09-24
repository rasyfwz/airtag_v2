#include "fusion.hpp"
#include "parser.hpp"
#include "serial.hpp"
#include "vis.hpp"

#include <chrono>
#include <cstdio>
#include <mutex>
#include <thread>

namespace {

class SharedOrientation {
public:
    void set(const Orientation& o) {
        std::lock_guard<std::mutex> lock(mutex_);
        orientation_ = o;
    }

    Orientation get() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return orientation_;
    }

private:
    mutable std::mutex mutex_;
    Orientation orientation_;
};

// Runs forever on its own thread; the process exit on window-close reclaims it.
void readerLoop(SerialPort& port, SharedOrientation& shared) {
    ImuFrameParser parser;
    ComplementaryFilter filter;
    auto lastTime = std::chrono::steady_clock::now();

    while (true) {
        auto byte = port.readByte();
        if (!byte) {
            continue;
        }
        parser.feed(*byte);

        auto sample = parser.tryParseFrame();
        if (!sample) {
            continue;
        }

        const auto now = std::chrono::steady_clock::now();
        const double dt = std::chrono::duration<double>(now - lastTime).count();
        lastTime = now;

        const Orientation orientation = filter.update(sample->accel, sample->gyro, dt);
        shared.set(orientation);

        printf("Roll: %6.2f  Pitch: %6.2f  Yaw: %6.2f\n",
               orientation.roll, orientation.pitch, orientation.yaw);
    }
}

} // namespace

int main(int argc, char * argv[]) {
    SerialPort port(argv[1], B115200);
    printf("Port is open\n");

    SharedOrientation shared;
    std::thread reader(readerLoop, std::ref(port), std::ref(shared));
    reader.detach();

    Visualizer visualizer;
    while (visualizer.render(shared.get())) {
    }

    return 0;
}
