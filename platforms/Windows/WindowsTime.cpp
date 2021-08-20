#include "core/time.h"

#include "GLFW/glfw3.h"

namespace selyan
{
    TimeStep TimeStep::getTime() { return TimeStep(glfwGetTime()); }

}
