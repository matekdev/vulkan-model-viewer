#pragma once

#include "lve_model.hpp"

#include <memory>

namespace lve
{
    struct Transform2dComponent
    {
        glm::vec2 Translation{};
        glm::vec2 scale{1.f, 1.f};
        float rotation;

        glm::mat2 Mat2()
        {
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);
            glm::mat2 rotMatrix{{c, s}, {-s, c}};

            glm::mat2 scaleMat{{scale.x, .0f}, {.0f, scale.y}};
            return rotMatrix * scaleMat;
        };
    };

    class LveObject
    {
    public:
        using id_t = unsigned int;

        static LveObject CreateObject()
        {
            static id_t currentId = 0;
            return LveObject{currentId++};
        }

        LveObject(const LveObject &) = delete;
        LveObject &operator=(const LveObject &) = delete;
        LveObject(LveObject &&) = default;
        LveObject &operator=(LveObject &&) = default;

        id_t GetId() { return _id; }

        std::shared_ptr<LveModel> model{};
        glm::vec3 color{};
        Transform2dComponent transform2d{};

    private:
        LveObject(id_t objectId) : _id{objectId} {}

        id_t _id;
    };
}