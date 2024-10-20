#ifndef MAP_H
#define MAP_H
#include <SDL.h>
extern const int mapX, mapY, mapS;
extern int mapW[];

void drawMap2D(SDL_Renderer* renderer);
#endif