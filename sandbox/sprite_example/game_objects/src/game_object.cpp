#include "game_objects/game_object.h"

namespace cannon_game
{
    uint32_t GameObject::lastUniqueId = 0;

    GameObject::GameObject()
      : m_uniqueId(lastUniqueId++),
        m_position({ 0, 0 }),
        m_rotation(0),
        m_scale({ 1, 1 }),
        m_collision({ m_position }),
        m_dead(false)
    {
        update_model_matrix();
    }

    uint32_t GameObject::getUniqueId() const { return m_uniqueId; }

    void GameObject::setCollision(const CollisionCircle &collision) { m_collision = collision; }

    CollisionCircle GameObject::getCollision() const { return m_collision; }

    glm::mat4 GameObject::getModelMatrix() const { return m_modelMatrix; }

    void GameObject::setPosition(const glm::vec2 &position)
    {
        if (m_position == position)
            return;

        m_collision.center = position;
        m_position = position;
        update_model_matrix();
    }

    glm::vec2 GameObject::getPosition() const { return m_position; }

    void GameObject::setScale(const glm::vec2 &scale)
    {
        if (m_scale == scale)
            return;

        m_scale = scale;
        update_model_matrix();
    }

    glm::vec2 GameObject::getScale() const { return m_scale; }

    void GameObject::setRotation(float rotation)
    {
        if (m_rotation == rotation)
            return;

        m_rotation = rotation;
        update_model_matrix();
    }

    float GameObject::getRotation() const { return m_rotation; }

    void GameObject::die() { m_dead = true; }

    void GameObject::alive() { m_dead = false; }

    bool GameObject::isDie() const { return m_dead; }

    void GameObject::update_model_matrix()
    {
        m_modelMatrix = glm::identity<glm::mat4>();
        m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(m_position, 0));
        m_modelMatrix =
            glm::rotate(m_modelMatrix, glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(m_scale, 1));
    }
}