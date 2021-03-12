#include "buffers.h"

Buffers::VAOEnableScoped::VAOEnableScoped(GLuint id) : attribute_id_(id) {
    glEnableVertexAttribArray(attribute_id_);
}

Buffers::VAOEnableScoped::~VAOEnableScoped() {
    glDisableVertexAttribArray(attribute_id_);
}

GLuint Buffers::VAOEnableScoped::GetId() const {
    return attribute_id_;
}

Buffers::VBOBindScoped::VBOBindScoped(std::vector<GLfloat> &buffer)  {
    glGenBuffers(1, &vbo_id_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
    glBufferData(GL_ARRAY_BUFFER, buffer.size() * 4, buffer.data(), GL_STATIC_DRAW);
}

Buffers::VBOBindScoped::~VBOBindScoped() {
    glDeleteBuffers(1, &vbo_id_);
}

GLuint Buffers::VBOBindScoped::GetId() const {
    return vbo_id_;
}