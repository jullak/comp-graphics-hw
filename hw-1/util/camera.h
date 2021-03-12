#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace CameraRotation {

    class IScenario {
    public:
        virtual glm::vec3 getStartPosition() const = 0;
        virtual glm::vec3 getNewPosition() = 0;
        virtual ~IScenario() {};
    };

    class Camera {
    public:
        Camera (const glm::mat4& proj, const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up, const glm::mat4& model, IScenario * scenario);

        void RebuildMVP();

        const glm::mat4& GetMVP() const;

    private:
        glm::vec3 center_; // tmp
        glm::vec3 up_; // tmp
        glm::mat4 projection_;
        glm::mat4 view_;
        glm::mat4 model_;
        glm::mat4 MVP_;
        IScenario * scenario_;
    };

}