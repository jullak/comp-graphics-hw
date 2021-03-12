#include "camera.h"

CameraRotation::Camera::Camera(const glm::mat4 &proj, const glm::vec3 &eye, const glm::vec3 &center,
                               const glm::vec3 &up, const glm::mat4 &model, IScenario *scenario)
        : center_(center)
        , up_(up)
        , projection_(proj)
        , model_(model)
        , scenario_(scenario) {
    view_ = glm::lookAt(scenario_->getStartPosition(), center_, up_);
    MVP_ = projection_ * view_ * model_;
}

void CameraRotation::Camera::RebuildMVP() {
    view_ = glm::lookAt(scenario_->getNewPosition(), center_, up_);
    MVP_ = projection_ * view_ * model_;
}

const glm::mat4& CameraRotation::Camera::GetMVP() const {
    return MVP_;
}