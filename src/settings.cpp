#include "settings.hpp"
#include <cmath>
#include "map.hpp"

const int width = 800;
const int height = 600;
const int FPS = 60;

const float FOV = 3.14159265f / 3;
const int num_rays = width;
const int DEPTH = 500;
const float DELTA = FOV / num_rays;

const float DIST = num_rays / (2.0f * tan(FOV / 2.0f));
const float PROJ_COEFF = 3 * DIST * TILE;

int player_x = width / 2;
int player_y = height / 2;
int player_angle = 0;
int player_speed = 5;