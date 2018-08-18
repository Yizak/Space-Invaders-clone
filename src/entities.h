// ENTITIES.H

#pragma once
#include "constants.h"
#include "graphics.h"

class Invader : public Sprite {
public:
	bool dying = false;
	GLfloat death_trajectory = 0.05f;
	Invader(char*, GLuint, SDL_Window*, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
	~Invader();
	void update(bool &, bool &, bool &, float &);
	bool get_create_missile(float &);
};

class Player : public Sprite {
public:
	GLint lives = 3;
	float speed = 0.01;
	Player(GLuint, SDL_Window*, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
	~Player();
	void update(vector<Invader*> &, vector<Life*> &, bool &);
};

class Bullet : public Sprite {
	float speed = 0.04;
public:
	Bullet(GLuint, SDL_Window*, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
	~Bullet();
	GLint update_get_delete_status(vector<Invader*> &, vector<Barrier*> &);
};

class Missile : public Sprite {
	float speed = 0.02;
public:
	Missile(GLuint, SDL_Window*, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
	~Missile();
	GLint update_get_delete_status(Player &, vector<Barrier*> &, GLint &lives);
};