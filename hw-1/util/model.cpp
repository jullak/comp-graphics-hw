#include "model.h"

void Model::AddTriangleWithColor(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec4 a_color, glm::vec4 b_color,
                        glm::vec4 c_color) {
    points_.push_back(a);
    points_.push_back(b);
    points_.push_back(c);

    colors_.push_back(a_color);
    colors_.push_back(b_color);
    colors_.push_back(c_color);
}

void Model::AddTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    points_.push_back(a);
    points_.push_back(b);
    points_.push_back(c);
}

const std::vector<glm::vec3>& Model::GetPoints() const {
    return points_;
}
const std::vector<glm::vec4>& Model::GetColors() const {
    return colors_;
}

void Model::PointsToBuffer(std::vector<GLfloat>& buffer) {
    pos_in_buffer_p_ = buffer.size() / 3;
    for (const auto& point : points_) {
        buffer.push_back(point.x);
        buffer.push_back(point.y);
        buffer.push_back(point.z);
    }
}

void Model::ColorsToBuffer(std::vector<GLfloat>& buffer, bool alpha_channel) {
    pos_in_buffer_c_ = buffer.size() / 3 + (alpha_channel ? 1 : 0);
    for (const auto& color : colors_) {
        buffer.push_back(color.r);
        buffer.push_back(color.g);
        buffer.push_back(color.b);

        if (alpha_channel) {
            buffer.push_back(color.a);
        }
    }
}

int32_t Model::TriangleCount() const {
    return points_.size() / 3;
}

size_t Model::GetPosPoint() const {
    return pos_in_buffer_p_;
}

size_t Model::GetColorPoint() const {
    return pos_in_buffer_c_;
}
