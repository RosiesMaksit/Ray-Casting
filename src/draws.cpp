#include "draws.hpp"
#include <cmath>

void drawCircle(SDL_Renderer* renderer, int x, int y, int radius) {
	for (int dy = -radius; dy <= radius; dy++) {
		int dx = (int)sqrt(radius * radius - dy * dy);
		
		SDL_RenderDrawLine(renderer, x - dx, y + dy, x + dx, y + dy);
	}
}