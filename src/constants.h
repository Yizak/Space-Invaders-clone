// CONSTANTS.H

#pragma once

#include <iostream>
#include <cstdlib>
#include <array>
#include <vector>
#include <chrono>

#include <GL/glew.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::vector;
using std::cout;
using std::endl;
using glm::mat4;
using glm::vec3;
using glm::vec4;

struct plot {
	GLdouble x;
	GLdouble y;
};

struct Rect {
	GLfloat x;
	GLfloat y;
	GLfloat w;
	GLfloat h;
};

const float VIEWPORT_X = 0, VIEWPORT_Y = 0;