#ifndef MAP_HPP
#define MAP_HPP

#include <SDL.h>

#define WIDTH_MAP 16
#define HEIGHT_MAP 12
#define TILE 50

extern int worldMap[HEIGHT_MAP][WIDTH_MAP];

void draw_map(SDL_Renderer* renderer);

#endif