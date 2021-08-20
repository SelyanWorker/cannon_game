#pragma once

#include "event.h"

namespace selyan
{
    class MouseKeyReleaseEvent : public Event
    {
    public:
        MouseKeyReleaseEvent(uint32_t buttonCode) : m_buttonCode(buttonCode) {}
        ~MouseKeyReleaseEvent() = default;

        static EventType GetStaticType() { return EventType::MouseKeyRelease; }
        EventType getType() { return EventType::MouseKeyRelease; }
        uint32_t GetKeyCode() const { return m_buttonCode; }

    private:
        uint32_t m_buttonCode;
    };

    class MouseKeyPressEvent : public Event
    {
    public:
        MouseKeyPressEvent(uint32_t buttonCode) : m_buttonCode(buttonCode) {}
        ~MouseKeyPressEvent() = default;

        static EventType GetStaticType() { return EventType::MouseKeyPressed; }
        EventType getType() { return EventType::MouseKeyPressed; }
        uint32_t GetKeyCode() const { return m_buttonCode; }

    private:
        uint32_t m_buttonCode;
    };

    class MouseMoveEvent : public Event
    {
    public:
        MouseMoveEvent(float xPos, float yPos) : m_xPos(xPos), m_yPos(yPos) {}
        ~MouseMoveEvent() = default;

        static EventType GetStaticType() { return EventType::MouseMoved; }
        EventType getType() { return EventType::MouseMoved; }

        float GetX() const { return m_xPos; }
        float GetY() const { return m_yPos; }

    private:
        float m_xPos;
        float m_yPos;
    };

    class MouseScrollEvent : public Event
    {
    public:
        MouseScrollEvent(float xPos, float yPos) : m_xOffset(xPos), m_yOffset(yPos) {}
        ~MouseScrollEvent() = default;

        static EventType GetStaticType() { return EventType::MouseScroll; }
        EventType getType() { return EventType::MouseScroll; }

        float GetX() const { return m_xOffset; }
        float GeTy() const { return m_yOffset; }

    private:
        float m_xOffset;
        float m_yOffset;
    };
}
