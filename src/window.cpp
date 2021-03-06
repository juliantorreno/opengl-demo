#include <array>
#include <memory>
#include <string>
#include <stdexcept>
#include <vector>

#include <tdpi/tdpi.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "window.hpp"

ogld::window* ogld::window::current_instance = nullptr;

ogld::px::px(unsigned int x_) noexcept : x(x_) {}

std::array<unsigned int, 2> ogld::px::operator*(ogld::px rh) { return {x, rh.x}; }
ogld::px operator""_px(unsigned long long x) { return x; }

void ogld::glfw_window_deleter::operator()(GLFWwindow* glfw_window) const noexcept { glfwDestroyWindow(glfw_window); }

ogld::window::window(std::string title, std::array<unsigned int, 2> size) : glfw_window(nullptr), aspect_ratio(size[0] / static_cast<float>(size[1])), glsl_program_(nullptr) {
    if (!glfwInit()) throw std::runtime_error("failed to initialize glfw.");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    glfw_window.reset(glfwCreateWindow(size[0], size[1], title.c_str(), nullptr, nullptr));
    if (!glfw_window.get()) throw std::runtime_error("failed to create window.");

    glfwMakeContextCurrent(glfw_window.get());
    glfwSetInputMode(glfw_window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!tdpiLoadGL()) throw std::runtime_error("failed to load OpenGL.");

    current_instance = this;

    shader<shader_type::vertex> vertex_shader("../opengl-demo/res/shaders/vertex_shader.glsl");
    shader<shader_type::fragment> fragment_shader("../opengl-demo/res/shaders/fragment_shader.glsl");

    glsl_program_.reset();
    glsl_program_ = std::make_unique<glsl_program>(vertex_shader, fragment_shader, std::vector<std::string>({"position", "normal", "texcoord"}));
    
    glsl_program_->bind();
}

void ogld::window::make_current() noexcept { glfwMakeContextCurrent(glfw_window.get()); }

void ogld::window::refresh() noexcept {
    glfwPollEvents();
    glfwSwapBuffers(glfw_window.get());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

ogld::window::operator GLFWwindow*() const noexcept { return glfw_window.get(); }
