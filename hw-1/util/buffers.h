#pragma once

#include <vector>

#include <GL/glew.h>

namespace Buffers {

    class VAOEnableScoped {
    public:
        VAOEnableScoped(GLuint id);
        ~VAOEnableScoped();

        GLuint GetId() const;
    private:
        GLuint attribute_id_;
    };

    class VBOBindScoped {
    public:
        VBOBindScoped(std::vector<GLfloat>& buffer);
        ~VBOBindScoped();

        GLuint GetId() const;
    private:
        GLuint vbo_id_;
    };
};
