#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL.h>

struct Pos {
	int x;
	int y;
};

class Player {
private:
	float x;
	float y;
	float angle;
	
public:
	Player();
	void update(SDL_Renderer* renderer);
	
	Pos getPos();
	float getAngle();
	
};

#endif