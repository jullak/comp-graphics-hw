#pragma once

#include <string>

#include <GL/glew.h>

namespace Shader {
    GLuint LoadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path);

    std::string ReadShaderFrom(const std::string& shader_path);

    void CompileAndCheck(GLuint id, const std::string& shader_code);
}
