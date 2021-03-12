#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

class Model {
public:
    Model() = default;

    void AddTriangleWithColor(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec4 a_color, glm::vec4 b_color, glm::vec4 c_color);
    void AddTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);
    //add with texture?

    const std::vector<glm::vec3>& GetPoints() const;
    const std::vector<glm::vec4>& GetColors() const;
    size_t GetPosPoint() const;
    size_t GetColorPoint() const;

    void PointsToBuffer(std::vector<GLfloat>& buffer);
    void ColorsToBuffer(std::vector<GLfloat>& buffer, bool alpha_channel = false);

    int32_t TriangleCount() const;

private:
    std::vector<glm::vec3> points_;
    std::vector<glm::vec4> colors_;
    size_t pos_in_buffer_p_;
    size_t pos_in_buffer_c_;
};


