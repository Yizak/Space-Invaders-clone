// ENTITIES.CPP

#pragma once
#include "constants.h"
#include "entities.h"

Player::Player(GLuint shader_program_, SDL_Window* window, GLfloat x_, GLfloat y_, GLfloat z_, GLfloat w_, GLfloat h_) {
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

	surface = IMG_Load("../assets/player.png");

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Player::~Player() {
	SDL_FreeSurface(surface);
}

void Player::update(vector<Invader*> &invaders, vector<Life*> &livesUI, bool &running) {
	for (GLint i = 0; i < invaders.size(); i++) {
		if (!invaders[i]->dying && is_colliding(collider, invaders[i]->collider)) {
			vector<Invader*>::iterator iter = invaders.begin() + i;
			invaders.erase(iter);
			lives -= 1;
			if (livesUI.size()) livesUI.pop_back();
			else if (lives == 0) running = false;
		}
	}

	if (!invaders.size()) running = false;
}

Invader::Invader(char* path, GLuint shader_program_, SDL_Window* window, GLfloat x_, GLfloat y_, GLfloat z_, GLfloat w_, GLfloat h_) {
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

Invader::~Invader() {
	SDL_FreeSurface(surface);
}

void Invader::update(bool &running, bool &enemy_direction, bool &enemy_descend, float &enemy_speed) {
	if (!dying) {
		if (enemy_direction && x >= 0.85) {
			enemy_direction = 0;
			enemy_speed += 0.00035;
			enemy_descend = true;
		}
		else if (!enemy_direction && x <= -0.85) {
			enemy_direction = 1;
			enemy_speed += 0.00035;
			enemy_descend = true;
		}
		else {
			if (enemy_direction) {
				x += enemy_speed;
				collider.x += enemy_speed;
			}
			else {
				x -= enemy_speed;
				collider.x -= enemy_speed;
			}
		}
	}

	if (!dying && y > 0.81f) running = false;

	if (dying) {
		y -= death_trajectory;
		collider.y -= death_trajectory;
		death_trajectory -= 0.004f;

		x += 0.01f;
		collider.x += 0.01f;

		rotation += 0.07f;
		if (rotation > 6.28f) rotation = 0.0f;
	}
}

bool Invader::get_create_missile(float &player_x) {
	if (abs(x - player_x) < 0.05 && abs(x - player_x) > -0.05 && (rand() % 200) == 0) {
		return true;
	}
	else {
		return false;
	}
}

Bullet::Bullet(GLuint shader_program_, SDL_Window* window, GLfloat x_, GLfloat y_, GLfloat z_, GLfloat w_, GLfloat h_) {
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

	surface = IMG_Load("../assets/bullet.png");

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Bullet::~Bullet() {
	SDL_FreeSurface(surface);
}

GLint Bullet::update_get_delete_status(vector<Invader*> &invaders, vector<Barrier*> &barriers) {
	y -= speed;
	collider.y -= speed;

	if (y < -1.1) {
		return 1;
	}

	for (GLint i = 0; i < invaders.size(); i++) {
		if (!invaders[i]->dying && is_colliding(collider, invaders[i]->collider)) {
			invaders[i]->dying = true;
			return 1;
		}
	}
	for (GLint i = 0; i < barriers.size(); i++) {
		if (is_colliding(collider, barriers[i]->collider)) {
			vector<Barrier*>::iterator iter = barriers.begin() + i;
			barriers.erase(iter);
			return 1;
		}
	}

	return 0;
}

Missile::Missile(GLuint shader_program_, SDL_Window* window, GLfloat x_, GLfloat y_, GLfloat z_, GLfloat w_, GLfloat h_) {
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

	surface = IMG_Load("../assets/bullet.png");

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Missile::~Missile() {
	SDL_FreeSurface(surface);
}

GLint Missile::update_get_delete_status(Player &player, vector<Barrier*> &barriers, GLint &lives) {
	y += speed;
	collider.y += speed;

	if (y < -1.1) {
		return 1;
	}

	if (is_colliding(collider, player.collider)) {
		lives -= 1;
		return 1;
	}

	for (GLint i = 0; i < barriers.size(); i++) {
		if (is_colliding(collider, barriers[i]->collider)) {
			vector<Barrier*>::iterator iter = barriers.begin() + i;
			barriers.erase(iter);
			return 1;
		}
	}

	return 0;
}