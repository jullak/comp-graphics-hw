#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class WindowInitializer {
public:
    WindowInitializer();

    ~WindowInitializer();

    WindowInitializer(const WindowInitializer& i) = delete;
    WindowInitializer& operator= (const WindowInitializer& i) = delete;

    GLFWwindow * GetWindowPtr() const {
        return window_;
    }

private:
    GLFWwindow * window_;
    GLuint vertex_array_attr_id_{0};

    void WindowHint_();
};

