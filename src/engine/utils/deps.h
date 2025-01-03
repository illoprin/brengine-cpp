#pragma once

// Standart C includes
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

// Standart CPP includes
#include <random>
#include <vector>
#include <iostream>
#include <string>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

// Math and linal
#include <math.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

// OpenGL includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

// STB image
#include <stb_image_write.h>
#include <stb_image.h>

// Misc
#include "defs.h"