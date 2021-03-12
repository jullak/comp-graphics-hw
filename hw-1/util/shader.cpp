#include "shader.h"

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

#include <GL/glew.h>


std::string Shader::ReadShaderFrom(const std::string& shader_path) {
    std::string shader_code;
    std::ifstream shader_stream(shader_path, std::ios::in);
    if (shader_stream.is_open()){
        std::stringstream sstr;
        sstr << shader_stream.rdbuf();
        shader_code = sstr.str();
        shader_stream.close();
    } else {
        std::cout << "Impossible to open " << shader_path << ". Are you in the right directory ? Don't forget to read the FAQ !" << std::endl;
    }
    return shader_code;
}

void Shader::CompileAndCheck(GLuint id, const std::string& shader_code) {
    GLint result = GL_FALSE;
    int32_t info_log_length;

    char const * source_pointer = shader_code.c_str();
    glShaderSource(id, 1, &source_pointer, NULL);
    glCompileShader(id);

    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0) {
        std::vector<char> error_message(info_log_length + 1);
        glGetShaderInfoLog(id, info_log_length, NULL, &error_message[0]);
        printf("%s\n", &error_message[0]);
    }
}

GLuint Shader::LoadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path){
    // Create the shaders
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertex_shader_code = ReadShaderFrom(vertex_file_path);
    std::string fragment_shader_code = ReadShaderFrom(fragment_file_path);


    std::cout << "Compiling shader : " << vertex_file_path << std::endl;
    CompileAndCheck(vertex_shader_id, vertex_shader_code);
    std::cout << "Compiling shader : " << fragment_file_path << std::endl;
    CompileAndCheck(fragment_shader_id, fragment_shader_code);

    std::cout << "Linking shaders" << std::endl;

    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);

    // Check the program
    GLint result = GL_FALSE;
    int32_t info_log_length;
    glGetProgramiv(program_id, GL_LINK_STATUS, &result);
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0){
        std::vector<char> error_message(info_log_length + 1);
        glGetProgramInfoLog(program_id, info_log_length, NULL, &error_message[0]);
        printf("%s\n", &error_message[0]);
    }

    glDetachShader(program_id, vertex_shader_id);
    glDetachShader(program_id, fragment_shader_id);

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return program_id;
}


