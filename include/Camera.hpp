#pragma once

#include "Vec.hpp"


class Camera {
public:
    Camera(int width, int height, float fov, const Vec3f& position, const Vec3f& lookAt, const Vec3f& up)
        : width(width), height(height), fov(fov), position(position), lookAt(lookAt), up(up) {
        updateBasis();
    }

    void setLookAt(const Vec3f& newLookAt) {
        lookAt = newLookAt;
        updateBasis();
    }

    void setBasis(const Vec3f& newRight, const Vec3f& newUp, const Vec3f& newForward) {
        right = newRight;
        up = newUp;
        forward = newForward;
    }

    Vec3f getPosition() const {
        return position;
    }

    Vec3f getRayDirection(float x, float y) const {
        float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        float halfFovTan = std::tan(glm::radians(fov) * 0.5f);

        float viewX = (2.0f * (x + 0.5f) / width - 1.0f) * aspectRatio * halfFovTan;
        float viewY = (2.0f * (y + 0.5f) / height - 1.0f) * halfFovTan;

        Vec3f rayDir = glm::normalize(viewX * right + viewY * up + forward);
        return rayDir;
    }

private:
    int width, height;
    float fov;
    Vec3f position, lookAt, up, right, forward;

    void updateBasis() {
        forward = glm::normalize(lookAt - position);
        right = glm::normalize(glm::cross(forward, up));
        up = glm::cross(right, forward);
    }
};
