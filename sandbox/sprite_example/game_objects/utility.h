#pragma once

#include "app"

#include <glm/glm.hpp>

namespace cannon_game
{
    namespace utility
    {
        float get_current_window_aspect_ratio()
        {
            return float(selyan::Application::get()->getWindow()->getHeight()) /
            selyan::Application::get()->getWindow()->getWidth();
        }

        glm::mat4 calc_model_matrix(const Entity& entity)
        {
            auto modelMatrix = glm::identity<glm::mat4>();
            modelMatrix = glm::translate(modelMatrix, entity.position);
            modelMatrix = glm::rotate(modelMatrix, glm::radians(entity.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(entity.scale, entity.scale, 1));

            return modelMatrix;
        }
    }

}
