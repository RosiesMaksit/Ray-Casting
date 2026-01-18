#include "player.hpp"
#include "settings.hpp"
#include "draws.hpp"

#include <cmath>

Player::Player() {
	x = player_x;
	y = player_y;
	angle = player_angle;
}

void Player::update(SDL_Renderer* renderer) {
	const Uint8* key = SDL_GetKeyboardState(NULL);
	
	if (key[SDL_SCANCODE_W]) {
		y += player_speed * sin(angle);
		x += player_speed * cos(angle);
	}
	if (key[SDL_SCANCODE_S]) {
		y += -player_speed * sin(angle);
		x += -player_speed * cos(angle);
	}
	if (key[SDL_SCANCODE_A]) {
		y += -player_speed * cos(angle);
		x += player_speed * sin(angle);
	}
	if (key[SDL_SCANCODE_D]) {
		y += player_speed * cos(angle);
		x += -player_speed * sin(angle);
	}
	
	if (key[SDL_SCANCODE_Q]) angle -= 0.05;
	if (key[SDL_SCANCODE_E]) angle += 0.05;
}

Pos Player::getPos() {
	return {(int)x, (int)y};
}

float Player::getAngle() {
	return angle;
}