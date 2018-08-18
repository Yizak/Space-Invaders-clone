#pragma once

#include "constants.h"
#include "entities.h"
#include "graphics.h"

using std::cout;
using std::endl;
using std::cin;
using std::vector;

const GLchar* vertex_shader_source = "#version 430 core\n"
	"layout (location = 0) in vec3 position;"
	"layout (location = 1) in vec3 color;"
	"layout (location = 2) in vec2 coords;"
	
	"uniform mat4 transform;"
	"uniform mat4 projection;"
	"uniform mat4 view;"
	
	"out vec3 vertexColour;"
	"out vec2 texCoords;"
	
	"void main()"
	"{"
	"gl_Position = projection * view * transform * vec4(position, 1.0f);"

	"vertexColour = color;"
	"texCoords = coords;"
	"}\n\0";
const GLchar* fragment_shader_source = "#version 430 core\n"
	"in vec3 vertexColor;"
	"in vec2 texCoords;"

	"out vec4 color;"

	"uniform sampler2D tex;"

	"void main()"
	"{"
	"color = texture(tex, 1.0 - texCoords);"
	"}\n\0";

const GLchar* cubemap_vertex_shader_source = "#version 430 core\n"
"layout (location = 0) in vec3 coords;"

"uniform mat4 projection;"
"uniform mat4 view;"

"out vec3 texCoords;"

"void main()"
"{"
"gl_Position = projection * view * vec4(coords, 1.0f);"

"texCoords = coords;"
"}\n\0";
const GLchar* cubemap_fragment_shader_source = "#version 430 core\n"
"in vec3 texCoords;"

"out vec4 color;"

"uniform samplerCube tex;"

"void main()"
"{"
"color = texture(tex, 1.0 - texCoords);"
"}\n\0";

SDL_Event event;
SDL_Window* window;
SDL_DisplayMode display;

vector<Barrier*> barriers;
vector<Invader*> invaders;
vector<Boundary*> boundaries;
vector<Bullet*> bullets;
vector<Missile*> missiles;
vector<Life*> livesUI;
//vector<Saucer*> saucers;
Player* player;
//Background* background;
Cubemap* cubemap;

GLuint VBO, VAO, EBO, vertex_shader, fragment_shader, shader_program;
GLuint cubemap_VBO, cubemap_VAO, cubemap_EBO, cubemap_vertex_shader, cubemap_fragment_shader, cubemap_shader_program;
std::array<bool, 7> buttons;
bool fullscreen = false, running = true, enemy_direction = 1, enemy_descend = false;
GLint width, height;
GLfloat viewpoint_x = 0.0f, viewpoint_y = 0.0f, viewpoint_z = -2.3f;
GLfloat view_direction_x = 0.0f, view_direction_y = 0.0f, view_direction_z = 0.0f;
float enemy_speed = 0.0008;
double delta_time, old_time;
const double FPS = 60.0;
char* path;

GLint gl_error();
GLint initialise(SDL_GLContext*);
GLint gl_draw_setup(GLfloat []);
void process_input();
void update();
void render();
double current_time();

int main(int argc, char *argv[]) {
	// SDL initialise
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init Error: %s\n",
			SDL_GetError());
		return 1;
	}
	SDL_Log("SDL initialised OK!\n");

	// Window creation
	SDL_GetCurrentDisplayMode(0, &display);
	window = SDL_CreateWindow("Isaac Starkie - CGP2012M Assignment 1 - 14536235", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, display.w / 2, display.h / 2, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (window == nullptr) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
			"SDL_CreateWindow init error: %s\n", SDL_GetError());
		return 1;
	}
	SDL_GetWindowSize(window, &width, &height);

	// Set viewport
	glViewport(VIEWPORT_X, VIEWPORT_Y, width, height);

	// Set OpenGL context parameters
	GLint major = 4, minor = 3;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Checking OpenGL version
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
	SDL_Log("OpenGL %d.%d context\n", major, minor);

	// OpenGL Context creation
	SDL_GLContext context = SDL_GL_CreateContext(window);
	if (context == nullptr) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
			"SDL_GL_CreateContext init error: %s\n", SDL_GetError());
		return 1;
	}

	if (initialise(&context)) {
		return 1;
	}

	GLfloat vertices[] = {
		//Hexagon vertices
		// Positions		Colours				Texture coordinates

		// Front
		0.0f, 0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	0.5f, 1.0f,		// 1
		-0.5f, 0.25f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.75f,	// 2
		0.5f, 0.25f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.75f,	// 3
		-0.5f, 0.25f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.75f,	// 2
		0.5f, 0.25f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.75f,	// 3
		0.0f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	0.5f, 0.0f,		// 6
		-0.5f, 0.25f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.75f,	// 2
		-0.5f, -0.25f, 0.0f,0.0f, 1.0f, 0.0f,	0.0f, 0.25f,	// 4
		0.0f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	0.5f, 0.0f,		// 6
		0.5f, 0.25f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.75f,	// 3
		0.5f, -0.25f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.25f,	// 5
		0.0f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	0.5f, 0.0f,		// 6
		////////////////////////////////////////////////////////////
		// Back
		0.0f, 0.5f, -0.1f,	0.0f, 1.0f, 0.0f,	0.5f, 0.0f,		// 7
		-0.5f, 0.25f, -0.1f,0.0f, 1.0f, 0.0f,	1.0f, 0.25f,	// 8
		0.5f, 0.25f, -0.1f, 0.0f, 1.0f, 0.0f,	0.0f, 0.25f,	// 9
		-0.5f, 0.25f, -0.1f,0.0f, 1.0f, 0.0f,	1.0f, 0.75f,	// 8
		0.5f, 0.25f,-0.1f,	0.0f, 1.0f, 0.0f,	0.0f, 0.75f,	// 9
		0.0f, -0.5f, -0.1f,	0.0f, 1.0f, 0.0f,	0.5f, 0.0f,		// 12
		-0.5f, 0.25f, -0.1f,0.0f, 1.0f, 0.0f,	1.0f, 0.75f,	// 8
		-0.5f, -0.25f,-0.1f,0.0f, 1.0f, 0.0f,	1.0f, 0.25f,	// 10
		0.0f, -0.5f, -0.1f,	0.0f, 1.0f, 0.0f,	0.5f, 0.0f,		// 12
		0.5f, 0.25f, -0.1f,	0.0f, 1.0f, 0.0f,	0.0f, 0.75f,	// 9
		0.5f, -0.25f, -0.1f,0.0f, 1.0f, 0.0f,	0.0f, 0.25f,	// 11
		0.0f, -0.5f, -0.1f,	0.0f, 1.0f, 0.0f,	0.5f, 0.0f,		// 12
		/////////////////////////////////////////////////////////////
		// Sides
		0.0f, 0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,		// 1
		-0.5f, 0.25f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// 2
		0.0f, 0.5f, -0.1f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,		// 7
		-0.5f, 0.25f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// 2
		0.0f, 0.5f, -0.1f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,		// 7
		-0.5f, 0.25f, -0.1f,0.0f, 1.0f, 0.0f,	0.0f, 0.0f,		// 8
		0.0f, 0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,		// 1
		0.5f, 0.25f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,		// 3
		0.0f, 0.5f, -0.1f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,		// 7
		0.5f, 0.25f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,		// 3
		0.0f, 0.5f, -0.1f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,		// 7
		0.5f, 0.25f, -0.1f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// 9
		-0.5f, 0.25f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,		// 2
		-0.5f, -0.25f, 0.0f,0.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// 4
		-0.5f, 0.25f, -0.1f,0.0f, 1.0f, 0.0f,	0.0f, 1.0f,		// 8
		-0.5f, -0.25f, 0.0f,0.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// 4
		-0.5f, 0.25f, -0.1f,0.0f, 1.0f, 0.0f,	0.0f, 1.0f,		// 8
		-0.5f, -0.25f,-0.1f,0.0f, 1.0f, 0.0f,	0.0f, 0.0f,		// 10
		0.5f, 0.25f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,		// 3
		0.5f, -0.25f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,		// 5
		0.5f, 0.25f, -0.1f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,		// 9
		0.5f, -0.25f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,		// 5
		0.5f, 0.25f, -0.1f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,		// 9
		0.5f, -0.25f, -0.1f,0.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// 11
		-0.5f, -0.25f, 0.0f,0.0f, 1.0f, 0.0f,	1.0f, 1.0f,		// 4
		0.0f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// 6
		-0.5f, -0.25f,-0.1f,0.0f, 1.0f, 0.0f,	0.0f, 1.0f,		// 10
		0.0f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// 6
		-0.5f, -0.25f,-0.1f,0.0f, 1.0f, 0.0f,	0.0f, 1.0f,		// 10
		0.0f, -0.5f, -0.1f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,		// 12
		0.5f, -0.25f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,		// 5
		0.0f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,		// 6
		0.5f, -0.25f, -0.1f,0.0f, 1.0f, 0.0f,	1.0f, 1.0f,		// 11
		0.0f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,		// 6
		0.5f, -0.25f, -0.1f,0.0f, 1.0f, 0.0f,	1.0f, 1.0f,		// 11
		0.0f, -0.5f, -0.1f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// 12
	};

	GLfloat cubemap_vertices[]{
		0.5f, 0.5f, 0.0f,		// 0	Top Right
		0.5f, -0.5f, 0.0f,		// 1	Bottom Right
		-0.5f, -0.5f, 0.0f,		// 2	Bottom Left
		-0.5f, 0.5f, 0.0f,		// 3	Top Left
		0.5f, 0.5f, -0.5f,		// 4	Top Right	(facing camera)
		0.5f, -0.5f, -0.5f,		// 5	Bottom Right
		-0.5f, -0.5f, -0.5f,	// 6	Bottom Left
		-0.5f, 0.5f, -0.5f		// 7	Top Left
	};


	GLuint cubemap_indices[] = {
		// Front
		0, 1, 3,	// First Triangle
		1, 2, 3,	// Second Triangle
		// Left
		2, 3, 7,
		2, 6, 7,
		// Right
		0, 4, 5,
		0, 1, 5,
		// Bottom
		1, 2, 5,
		2, 5, 6,
		// Back
		4, 6, 7,
		4, 5, 6,
		// Top
		0, 4, 7,
		0, 3, 7
	};

	gl_draw_setup(vertices);

	// Bind VAO
	glBindVertexArray(VAO);

	// Send vertices to buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Send indices to buffer
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position vertex attribute setup
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Colour vertex attribute setup
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Texture coordinate vertex attribute setup
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(cubemap_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubemap_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubemap_vertices), cubemap_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubemap_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubemap_indices), cubemap_indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Unbind stuff for safety
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	//glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	player = new Player(shader_program, window, 0, 0.83, 0.0, 0.1, 0.08);
	//background = new Background("../assets/background.png", shader_program, window, 0, -1, 0.0, 1, 2);
	
	cubemap = new Cubemap("../assets/background.png", "../assets/background.png", cubemap_shader_program, window);

	for (GLint i = 0; i < 4; i++) {
		for (GLint j = 0; j < 10; j++) {
			for (GLint k = 0; k < 4; k++) {
				barriers.emplace_back(new Barrier(shader_program, window, -1.09 + (0.4 * (i + 1)) + (j / 50.0), 0.55 + (k / 25.0), 0.0, 0.02, 0.04));
			}
		}
	}
	for (GLint i = 0; i < 5; i++) {
		switch (i) {
		case 0:
		case 1:
			path = "../assets/invader1.png";
			break;
		case 2:
		case 3:
			path = "../assets/invader2.png";
			break;
		case 4:
			path = "../assets/invader3.png";
			break;
		}
		for (GLint j = 0; j < 11; j++) {
			invaders.emplace_back(new Invader(path, shader_program, window, -0.73 + (0.142 * j), -0.6 + (i * 0.142), 0.0, 0.102, 0.102));
		}
	}
	boundaries.emplace_back(new Boundary(shader_program, window, 1, -0.015, 0, 0.2, 2.15));
	boundaries.emplace_back(new Boundary(shader_program, window, -1, -0.015, 0, 0.2, 2.15));
	boundaries.emplace_back(new Boundary(shader_program, window, 0, 1, 0, 2, 0.24));
	boundaries.emplace_back(new Boundary(shader_program, window, 0, -1, 0, 2, 0.4));

	for (GLint i = 0; i < 3; i++) {
		livesUI.emplace_back(new Life(shader_program, window, -0.05 + (0.05 * i), 1.0, 0.05, 0.04, 0.06));
	}

	// building key map
	buttons[0] = false;		// left
	buttons[1] = false;		// right
	buttons[2] = false;		// shoot
	buttons[3] = false;		// fullscreen
	buttons[4] = false;		// view 1
	buttons[5] = false;		// view 2
	buttons[6] = false;		// view 3

	srand(time(NULL));

	old_time = current_time();

	// GAME LOOP
	while (running) {
		delta_time = current_time() - old_time;
		if (delta_time > 1000.0 / FPS) {

			process_input();
			update();
			render();

			old_time = current_time();

		}
		else {
			SDL_Delay(1);
		}
	}

	SDL_Log("Closing OpenGL...\n");

	SDL_GL_DeleteContext(context);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteVertexArrays(1, &cubemap_VAO);
	glDeleteBuffers(1, &cubemap_VBO);
	glDeleteBuffers(1, &cubemap_EBO);
	
	SDL_Log("Closing SDL...\n");
	delete player, cubemap;
	//delete background;

	for (GLint i = 0; i < barriers.size(); i++) {
		delete barriers[i];
	}
	for (GLint i = 0; i < invaders.size(); i++) {
		delete invaders[i];
	}
	for (GLint i = 0; i < boundaries.size(); i++) {
		delete boundaries[i];
	}
	for (GLint i = 0; i < bullets.size(); i++) {
		delete bullets[i];
	}
	for (GLint i = 0; i < missiles.size(); i++) {
		delete missiles[i];
	}
	for (GLint i = 0; i < livesUI.size(); i++) {
		delete livesUI[i];
	}
	/*for (GLint i = 0; i < saucers.size(); i++) {
		delete saucers[i];
	}*/
	SDL_Quit();

	return 0;
}

void process_input() {
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		running = false;
		break;
	case SDL_WINDOWEVENT:
		// handling window resize events
		if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
			SDL_GetWindowSize(window, &width, &height);
		}
		break;
	case SDL_KEYUP:
		SDL_FlushEvent(SDLK_LEFT);
		SDL_FlushEvent(SDLK_a);
		SDL_FlushEvent(SDLK_RIGHT);
		SDL_FlushEvent(SDLK_d);
		SDL_FlushEvent(SDLK_SPACE);
		SDL_FlushEvent(SDLK_w);
		SDL_FlushEvent(SDLK_f);
		SDL_FlushEvent(SDLK_1);
		SDL_FlushEvent(SDLK_2);
		SDL_FlushEvent(SDLK_3);
		buttons[0] = false;
		buttons[1] = false;
		buttons[2] = false;
		buttons[3] = false;
		buttons[4] = false;
		buttons[5] = false;
		buttons[6] = false;
		break;
	default:
		break;
	}

	if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
		switch (event.key.keysym.sym) {
		case SDLK_LEFT:
		case SDLK_a:
			buttons[0] = true;
			break;
		case SDLK_RIGHT:
		case SDLK_d:
			buttons[1] = true;
			break;
		case SDLK_SPACE:
		case SDLK_w:
			buttons[2] = true;
			break;
		case SDLK_f:
			buttons[3] = true;
			break;
		case SDLK_1:
			buttons[4] = true;
			break;
		case SDLK_2:
			buttons[5] = true;
			break;
		case SDLK_3:
			buttons[6] = true;
			break;
		case SDLK_ESCAPE:
			running = false;
			break;
		}
	}
}

void update() {
	// handling keyboard input
	if (buttons[0] && player->x > -0.83) {
		player->x -= player->speed;
		player->collider.x -= player->speed;

		view_direction_x += player->speed * -0.5f;
	}
	if (buttons[1] && player->x < 0.83) {
		player->x += player->speed;
		player->collider.x += player->speed;

		view_direction_x -= player->speed * -0.5f;
	}

	if (buttons[2]) {
		bullets.emplace_back(new Bullet(shader_program, window, player->x, player->y - 0.016, 0.0, 0.01, 0.03));
		buttons[2] = false;
	}

	if (buttons[3]) {
		if (!fullscreen) {
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
			SDL_ShowCursor(SDL_DISABLE);
			fullscreen = true;
		}
		else {
			SDL_SetWindowFullscreen(window, 0);
			SDL_SetWindowSize(window, display.w / 2, display.h / 2);
			SDL_ShowCursor(SDL_ENABLE);
			fullscreen = false;
			SDL_GetWindowSize(window, &width, &height);
		}
		buttons[3] = false;
	}

	if (buttons[4]) {
		viewpoint_x = -1.5f;
		viewpoint_y = 0.5f;
		viewpoint_z = -2.0f;
	}
	if (buttons[5]) {
		viewpoint_x = 0.0f;
		viewpoint_y = 0.0f;
		viewpoint_z = -2.3f;
	}
	if (buttons[6]) {
		viewpoint_x = 1.5f;
		viewpoint_y = -0.5f;
		viewpoint_z = -2.0f;
	}

	// handling player update
	player->update(invaders, livesUI, running);

	// handling invader updates
	for (GLint i = 0; i < invaders.size(); i++) {
		invaders[i]->update(running, enemy_direction, enemy_descend, enemy_speed);
		if (invaders[i]->get_create_missile(player->x)) {
			missiles.emplace_back(new Missile(shader_program, window, invaders[i]->x, invaders[i]->y + 0.052, 0.0, 0.01, 0.03));
		}
		if (invaders[i]->dying && invaders[i]->y > 1.5f) {
			vector<Invader*>::iterator iter = invaders.begin() + i;
			invaders.erase(iter);
		}
	}
	if (enemy_descend) {
		for (GLint i = 0; i < invaders.size(); i++) {
			invaders[i]->y += 0.04;
			invaders[i]->collider.y += 0.04;
		}
		enemy_descend = false;
	}

	// handling bullet updates
	for (GLint i = 0; i < bullets.size(); i++) {
		if (bullets[i]->update_get_delete_status(invaders, barriers) == 1) {
			vector<Bullet*>::iterator iter = bullets.begin() + i;
			bullets.erase(iter);
		}
	}

	// handling missile updates
	GLint temp = player->lives;

	for (GLint i = 0; i < missiles.size(); i++) {
		if (missiles[i]->update_get_delete_status(*player, barriers, player->lives) == 1) {
			vector<Missile*>::iterator iter = missiles.begin() + i;
			missiles.erase(iter);
		}
	}
	if (livesUI.size() && player->lives < temp) livesUI.pop_back();
	else if (player->lives == 0) running = false;

	// handling background update
	//background->update();

	// handing saucer updates
	/*for (GLint i = 0; i < saucers.size(); i++) {
		saucers[i]->update(player->x);
	}*/
}

void render() {
	// Clear the colour and depth buffers
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render cubemap

	//glBindVertexArray(cubemap_VAO);
	//cubemap->render();

	// Render sprites

	// Bind vertex array object
	glBindVertexArray(VAO);

	/*background->render(	viewpoint_x, viewpoint_y, viewpoint_z,
						view_direction_x, view_direction_y, view_direction_z,
						width, height);
	for (GLint i = 0; i < saucers.size(); i++) {
		saucers[i]->render(	viewpoint_x, viewpoint_y, viewpoint_z,
							view_direction_x, view_direction_y, view_direction_z,
							width, height);
	}*/

	for (GLint i = 0; i < boundaries.size(); i++) {
		boundaries[i]->render(	viewpoint_x, viewpoint_y, viewpoint_z,
								view_direction_x, view_direction_y, view_direction_z,
								width, height);
	}
	for (GLint i = 0; i < bullets.size(); i++) {
		bullets[i]->render(	viewpoint_x, viewpoint_y, viewpoint_z,
							view_direction_x, view_direction_y, view_direction_z,
							width, height);
	}
	for (GLint i = 0; i < missiles.size(); i++) {
		missiles[i]->render(viewpoint_x, viewpoint_y, viewpoint_z,
							view_direction_x, view_direction_y, view_direction_z,
							width, height);
	}
	for (GLint i = 0; i < barriers.size(); i++) {
		barriers[i]->render(viewpoint_x, viewpoint_y, viewpoint_z,
							view_direction_x, view_direction_y, view_direction_z,
							width, height);
	}
	for (GLint i = 0; i < invaders.size(); i++) {
		invaders[i]->render(viewpoint_x, viewpoint_y, viewpoint_z,
							view_direction_x, view_direction_y, view_direction_z,
							width, height);
	}
	for (GLint i = 0; i < livesUI.size(); i++) {
		livesUI[i]->render(	viewpoint_x, viewpoint_y, viewpoint_z,
							view_direction_x, view_direction_y, view_direction_z,
							width, height);
	}

	player->render(	viewpoint_x, viewpoint_y, viewpoint_z,
					view_direction_x, view_direction_y, view_direction_z,
					width, height);

	// Unbind vertex array object
	glBindVertexArray(0);

	// Present the back buffer
	SDL_GL_SwapWindow(window);
}

double current_time() {
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 1000000.0;
}

GLint gl_error() {
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		cout << error;
		return 1;
	}

	return 0;
}

GLint initialise(SDL_GLContext* context) {
	// GLEW initialise
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		cout << "Failed to initialize GLEW" << std::endl;
		return 1;
	}

	// SDL_Image initialise
	GLint imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_image init error: %s\n",
			IMG_GetError());
		return 1;
	}
	SDL_Log("SDL_image initialised OK!\n");

	return 0;
}

GLint gl_draw_setup(GLfloat vertices[]) {
	GLint success;
	GLchar infoLog[512];

	// Compile vertex shaders
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
		cout << "ERROR:\n" << infoLog << endl;
	}
	cubemap_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(cubemap_vertex_shader, 1, &cubemap_vertex_shader_source, NULL);
	glCompileShader(cubemap_vertex_shader);
	glGetShaderiv(cubemap_vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(cubemap_vertex_shader, 512, NULL, infoLog);
		cout << "ERROR:\n" << infoLog << endl;
	}

	// Compile fragment shaders
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
		cout << "ERROR:\n" << infoLog << endl;
	}
	cubemap_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(cubemap_fragment_shader, 1, &cubemap_fragment_shader_source, NULL);
	glCompileShader(cubemap_fragment_shader);
	glGetShaderiv(cubemap_fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(cubemap_fragment_shader, 512, NULL, infoLog);
		cout << "ERROR:\n" << infoLog << endl;
	}

	// Create shader programs
	shader_program = glCreateProgram();

	cubemap_shader_program = glCreateProgram();

	// Link shaders via the shader programs
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
		cout << infoLog << endl;
	}
	glAttachShader(cubemap_shader_program, cubemap_vertex_shader);
	glAttachShader(cubemap_shader_program, cubemap_fragment_shader);
	glLinkProgram(cubemap_shader_program);
	glGetProgramiv(cubemap_shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(cubemap_shader_program, 512, NULL, infoLog);
		cout << infoLog << endl;
	}

	// Delete redundant shaders
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	glDeleteShader(cubemap_vertex_shader);
	glDeleteShader(cubemap_fragment_shader);

	// Create vertex buffer object, vertex array object
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	glGenBuffers(1, &cubemap_VBO);
	glGenVertexArrays(1, &cubemap_VAO);
	glGenBuffers(1, &cubemap_EBO);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return 1;
}