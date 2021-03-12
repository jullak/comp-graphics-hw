#include "window_initializer.h"

WindowInitializer::WindowInitializer() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        throw std::runtime_error("Failed to initialize GLFW\n");
    }

    WindowHint_();
    window_ = glfwCreateWindow(1024, 768, "app", NULL, NULL);
    glfwMakeContextCurrent(window_);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        throw std::runtime_error("Failed to initialize GLFW\n");
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window_, GLFW_STICKY_KEYS, GL_TRUE);

    glGenVertexArrays(1, &vertex_array_attr_id_);
    glBindVertexArray(vertex_array_attr_id_);

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
}

WindowInitializer::~WindowInitializer()  {
    glDeleteVertexArrays(1, &vertex_array_attr_id_);
    glfwTerminate();
}

void WindowInitializer::WindowHint_() {
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}