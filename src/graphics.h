// GRAPHICS.H

#pragma once
#include "constants.h"

class Sprite {
protected:
	SDL_Surface* surface = nullptr;
	GLuint shader_program, texture;
	GLfloat rotation = 0.0f;
public:
	Rect collider;
	GLfloat x, y, z, w, h;

	Sprite();
	Sprite(char*, SDL_Window*, GLuint, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
	virtual void update();
	virtual void render(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLint, GLint);
	bool is_colliding(Rect, Rect);
};

class Barrier : public Sprite {
public:
	Barrier(GLuint, SDL_Window*, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
	~Barrier();
};

class Boundary : public Sprite {
public:
	Boundary(GLuint, SDL_Window*, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
	~Boundary();
};

class Background : public Sprite {
public:
	Background(char*, GLuint, SDL_Window*, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
	~Background();
	void update();
};

class Cubemap : public Sprite {
	vector<char*> file_paths;
public:
	Cubemap(char*, char*, GLuint, SDL_Window*);
	~Cubemap();
	void render();
};

class Saucer : public Sprite {
	GLfloat x_init;
public:
	Saucer(GLuint, SDL_Window*, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
	~Saucer();
	void update(GLfloat &);
};

class Life : public Sprite {
public:
	Life(GLuint, SDL_Window*, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
	~Life();
};