// GRAPHICS.CPP

#pragma once
#include "constants.h"
#include "graphics.h"

Sprite::Sprite() {
	// default sprite constructor
}

Sprite::Sprite(char* path, SDL_Window* window, GLuint shader_program_, GLfloat x_, GLfloat y_, GLfloat z_, GLfloat w_, GLfloat h_) {
	shader_program = shader_program_;
	x = x_;
	y = y_;
	z = z_;
	w = w_;
	h = h_;
	collider.x = x;
	collider.y = y;
	collider.w = w;
	collider.h = h;

	surface = IMG_Load(path);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Sprite::update() {
	//
}

void Sprite::render(GLfloat viewpoint_x, GLfloat viewpoint_y, GLfloat viewpoint_z, GLfloat view_direction_x, GLfloat view_direction_y, GLfloat view_direction_z, GLint screen_width, GLint screen_height) {
	glUseProgram(shader_program);

	glm::mat4 projection = glm::perspective(45.0f, ((float)screen_width / 2.0f) / (float)screen_height, 0.1f, 10.0f);
	//glm::mat4 view = glm::translate(mat4(), glm::vec3(viewpoint_x, viewpoint_y, viewpoint_z));
	glm::mat4 view =	glm::lookAt(glm::vec3(viewpoint_x, viewpoint_y, -viewpoint_z),
						glm::vec3(view_direction_x, view_direction_y, view_direction_z),
						glm::vec3(0.0f, 1.0f, 0.0f));			
	glm::mat4 trans;
	trans = glm::translate(trans, glm::vec3(x, -y, z));
	trans = glm::scale(trans, glm::vec3(w, h, 1.0f));
	trans = glm::rotate(trans, rotation, glm::vec3(1.0f, 0.0f, 1.0f));

	GLint projection_location = glGetUniformLocation(shader_program, "projection");
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

	GLint view_location = glGetUniformLocation(shader_program, "view");
	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

	GLint transform_location = glGetUniformLocation(shader_program, "transform");
	glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(trans));

	glViewport(VIEWPORT_X, VIEWPORT_Y, screen_width, screen_height);

	// Bind texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Draw triangles
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 60);
}

bool Sprite::is_colliding(Rect a, Rect b) {
	a.x -= a.w;
	a.y -= a.h;
	b.x -= b.w;
	b.y -= b.h;
	a.w *= 2;
	a.h *= 2;
	b.w *= 2;
	b.h *= 2;

	bool collision = true;

	if (a.y + a.h <= b.y) {
		collision = false;
	}
	else if (a.y >= b.y + b.h)
	{
		collision = false;
	}
	else if (a.x + a.w <= b.x)
	{
		collision = false;
	}
	else if (a.x >= b.x + b.w)
	{
		collision = false;
	}

	return collision;
}

Barrier::Barrier(GLuint shader_program_, SDL_Window* window, GLfloat x_, GLfloat y_, GLfloat z_, GLfloat w_, GLfloat h_) {
	shader_program = shader_program_;
	x = x_;
	y = y_;
	z = z_;
	w = w_;
	h = h_;
	collider.x = x;
	collider.y = y;
	collider.w = w;
	collider.h = h;

	surface = IMG_Load("../assets/green_square.png");

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Barrier::~Barrier() {
	SDL_FreeSurface(surface);
}

Boundary::Boundary(GLuint shader_program_, SDL_Window* window, GLfloat x_, GLfloat y_, GLfloat z_, GLfloat w_, GLfloat h_) {
	shader_program = shader_program_;
	x = x_;
	y = y_;
	z = z_;
	w = w_;
	h = h_;
	collider.x = x;
	collider.y = y;
	collider.w = w;
	collider.h = h;

	surface = IMG_Load("../assets/dark_green_square.png");

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Boundary::~Boundary() {
	SDL_FreeSurface(surface);
}

Background::Background(char* path, GLuint shader_program_, SDL_Window* window, GLfloat x_, GLfloat y_, GLfloat z_, GLfloat w_, GLfloat h_) {
	shader_program = shader_program_;
	x = x_;
	y = y_;
	z = z_;
	w = w_;
	h = h_;
	collider.x = x;
	collider.y = y;
	collider.w = w;
	collider.h = h;

	surface = IMG_Load(path);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Background::~Background() {
	SDL_FreeSurface(surface);
}

void Background::update() {
	if (y < 1) y += 0.001;
	else y = -1;
}

Cubemap::Cubemap(char* path1, char* path2, GLuint shader_program_, SDL_Window* window) {
	shader_program = shader_program_;

	file_paths.emplace_back(path1);	// Right
	file_paths.emplace_back(path1);	// Left
	file_paths.emplace_back(path1);	// Top
	file_paths.emplace_back(path2);	// Bottom
	file_paths.emplace_back(path1);	// Back
	file_paths.emplace_back(path1);	// Front

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	for (GLuint i = 0; i < file_paths.size(); i++)
	{
		surface = IMG_Load(file_paths[i]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
		surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

Cubemap::~Cubemap() {
	SDL_FreeSurface(surface);
}

void Cubemap::render() {
	glDepthMask(GL_FALSE);
	glUseProgram(shader_program);

	/*glm::mat4 projection = glm::perspective(45.0f, ((float)screen_width / 2.0f) / (float)screen_height, 0.1f, 10.0f);
	//glm::mat4 view = glm::translate(mat4(), glm::vec3(viewpoint_x, viewpoint_y, viewpoint_z));
	glm::mat4 view = glm::lookAt(glm::vec3(viewpoint_x, viewpoint_y, -viewpoint_z),
		glm::vec3(view_direction_x, view_direction_y, view_direction_z),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 trans;
	trans = glm::translate(trans, glm::vec3(x, -y, z));
	trans = glm::scale(trans, glm::vec3(w, h, 1.0f));
	trans = glm::rotate(trans, rotation, glm::vec3(1.0f, 0.0f, 1.0f));

	GLint projection_location = glGetUniformLocation(shader_program, "projection");
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

	GLint view_location = glGetUniformLocation(shader_program, "view");
	glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

	GLint transform_location = glGetUniformLocation(shader_program, "transform");
	glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(trans));

	glViewport(VIEWPORT_X, VIEWPORT_Y, screen_width, screen_height);*/

	// Bind texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);	

	// Draw triangles
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDepthMask(GL_TRUE);
}

Saucer::Saucer(GLuint shader_program_, SDL_Window* window, GLfloat x_, GLfloat y_, GLfloat z_, GLfloat w_, GLfloat h_) {
	shader_program = shader_program_;
	x = x_;
	x_init = x;
	y = y_;
	z = z_;
	w = w_;
	h = h_;
	collider.x = x;
	collider.y = y;
	collider.w = w;
	collider.h = h;

	surface = IMG_Load("../assets/saucer.png");

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Saucer::~Saucer() {
	SDL_FreeSurface(surface);
}

void Saucer::update(GLfloat &player_x) {
	x = x_init + (player_x / 15.0f);
}

Life::Life(GLuint shader_program_, SDL_Window* window, GLfloat x_, GLfloat y_, GLfloat z_, GLfloat w_, GLfloat h_) {
	shader_program = shader_program_;
	x = x_;
	y = y_;
	z = z_;
	w = w_;
	h = h_;
	collider.x = x;
	collider.y = y;
	collider.w = w;
	collider.h = h;

	surface = IMG_Load("../assets/heart.png");

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Life::~Life() {
	SDL_FreeSurface(surface);
}