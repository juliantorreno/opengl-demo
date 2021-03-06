#pragma once

#include <string>
#include <vector>

#include <tdpi/tdpi.h>

#include "camera.hpp"
#include "shader.hpp"

template <typename T> class renderer;

namespace ogld {
    class glsl_program {
    public:
        glsl_program(shader<shader_type::vertex> const& vs, shader<shader_type::fragment> const& fs, std::vector<std::string> const& attributes);

        glsl_program(glsl_program&& other) noexcept;

        glsl_program(glsl_program const&) = delete;
        glsl_program& operator=(glsl_program const&) = delete;

        ~glsl_program();

        operator GLuint() const noexcept;

        void bind() noexcept;
    private:
        friend class camera;
        template <typename T> friend class renderer;
        static glsl_program* current_instance;

        GLuint handle;
    };
}
