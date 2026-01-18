#include "ray_casting.hpp"

#include "settings.hpp"

#include "map.hpp"
#include <cmath>

void ray_casting(SDL_Renderer* renderer, Pos player_pos, float angle) {
	float cur_angle = angle - (FOV / 2);
	
	float xo = player_pos.x;
	float yo = player_pos.y;
	
	for (int ray = 0; ray < num_rays; ray++) {
		float cos_a = cos(cur_angle);
		float sin_a = sin(cur_angle);
		for (int depth = 0; depth < DEPTH; depth++) {
			float x = xo + depth * cos_a;
			float y = yo + depth * sin_a;
			
			if (worldMap[(int)(y / TILE)][(int)(x / TILE)]) {
				depth *= cos(angle - cur_angle);
				SDL_Rect wall = {ray * (width / num_rays), (int)(height / 2 - (PROJ_COEFF / depth) / 2), width / num_rays, (int)(PROJ_COEFF / depth)};
				SDL_SetRenderDrawColor(renderer, 255 / (1 + depth * depth * 0.0001), 255 / (1 + depth * depth * 0.0001), 255 / (1 + depth * depth * 0.0001), 255);
				SDL_RenderFillRect(renderer, &wall);
				break;
			}
		}
		cur_angle += DELTA;
	}
	
}