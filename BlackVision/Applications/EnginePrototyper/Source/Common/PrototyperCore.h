#pragma once

#include <string>

// Disable warning C4201: nonstandard extension used : nameless struct/union
#pragma warning(push)
#pragma warning(disable : 4201)

#include <glm/glm.hpp>

#pragma warning(pop)

#include <gl/glew.h>

#include "System/BasicTypes.h"

const std::string PROTOTYPES_SHADERS_ROOT = "Assets/Shaders/Prototypes/";