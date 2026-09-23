#include "vis.hpp"

#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include <stdexcept>

Visualizer::Visualizer(int width, int height, const char* title) {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window_) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
}

Visualizer::~Visualizer() {
    if (window_) {
        glfwDestroyWindow(window_);
    }
    glfwTerminate();
}

bool Visualizer::render(const Orientation& orientation) {
    if (glfwWindowShouldClose(window_)) {
        return false;
    }

    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(window_, &width, &height);
    if (height == 0) {
        height = 1;
    }
    glViewport(0, 0, width, height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, static_cast<double>(width) / height, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(3.0, 2.5, 3.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    drawAxes();

    // Block's local +X is "forward" (roll axis), +Z is "right" (pitch axis),
    // world +Y is "up" (yaw axis). No translation is ever applied.
    glPushMatrix();
    glRotated(orientation.yaw, 0.0, 1.0, 0.0);
    glRotated(orientation.pitch, 0.0, 0.0, 1.0);
    glRotated(orientation.roll, 1.0, 0.0, 0.0);
    drawBlock();
    glPopMatrix();

    glfwSwapBuffers(window_);
    glfwPollEvents();

    return true;
}

void Visualizer::drawAxes() const {
    constexpr double kLength = 2.0;

    glBegin(GL_LINES);
    glColor3d(1.0, 0.2, 0.2);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(kLength, 0.0, 0.0);

    glColor3d(0.2, 1.0, 0.2);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, kLength, 0.0);

    glColor3d(0.2, 0.2, 1.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, kLength);
    glEnd();
}

void Visualizer::drawBlock() const {
    constexpr double kHalfX = 0.75;
    constexpr double kHalfY = 0.15;
    constexpr double kHalfZ = 0.5;

    glBegin(GL_QUADS);

    // +X face (front)
    glColor3d(0.90, 0.45, 0.10);
    glVertex3d(kHalfX, -kHalfY, -kHalfZ);
    glVertex3d(kHalfX, kHalfY, -kHalfZ);
    glVertex3d(kHalfX, kHalfY, kHalfZ);
    glVertex3d(kHalfX, -kHalfY, kHalfZ);

    // -X face (back)
    glColor3d(0.55, 0.25, 0.05);
    glVertex3d(-kHalfX, -kHalfY, kHalfZ);
    glVertex3d(-kHalfX, kHalfY, kHalfZ);
    glVertex3d(-kHalfX, kHalfY, -kHalfZ);
    glVertex3d(-kHalfX, -kHalfY, -kHalfZ);

    // +Y face (top)
    glColor3d(0.85, 0.85, 0.85);
    glVertex3d(-kHalfX, kHalfY, -kHalfZ);
    glVertex3d(kHalfX, kHalfY, -kHalfZ);
    glVertex3d(kHalfX, kHalfY, kHalfZ);
    glVertex3d(-kHalfX, kHalfY, kHalfZ);

    // -Y face (bottom)
    glColor3d(0.25, 0.25, 0.25);
    glVertex3d(-kHalfX, -kHalfY, kHalfZ);
    glVertex3d(kHalfX, -kHalfY, kHalfZ);
    glVertex3d(kHalfX, -kHalfY, -kHalfZ);
    glVertex3d(-kHalfX, -kHalfY, -kHalfZ);

    // +Z face (right)
    glColor3d(0.20, 0.40, 0.85);
    glVertex3d(-kHalfX, -kHalfY, kHalfZ);
    glVertex3d(-kHalfX, kHalfY, kHalfZ);
    glVertex3d(kHalfX, kHalfY, kHalfZ);
    glVertex3d(kHalfX, -kHalfY, kHalfZ);

    // -Z face (left)
    glColor3d(0.20, 0.65, 0.30);
    glVertex3d(kHalfX, -kHalfY, -kHalfZ);
    glVertex3d(kHalfX, kHalfY, -kHalfZ);
    glVertex3d(-kHalfX, kHalfY, -kHalfZ);
    glVertex3d(-kHalfX, -kHalfY, -kHalfZ);

    glEnd();
}
