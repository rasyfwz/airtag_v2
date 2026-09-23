#ifndef VIS_HPP
#define VIS_HPP

#include "fusion.hpp"

struct GLFWwindow;

// Renders a fixed-position block whose rotation reflects the IMU's current
// roll/pitch/yaw. There is no translational motion.
class Visualizer {
public:
    Visualizer(int width = 800, int height = 600, const char* title = "IMU Orientation");
    ~Visualizer();

    Visualizer(const Visualizer&) = delete;
    Visualizer& operator=(const Visualizer&) = delete;

    // Draws one frame for the given orientation. Returns false once the
    // window has been asked to close.
    bool render(const Orientation& orientation);

private:
    void drawAxes() const;
    void drawBlock() const;

    GLFWwindow* window_ = nullptr;
};

#endif
