#ifndef MAP_H
#define MAP_H
#include <SDL.h>
extern const int mapX, mapY, mapS;
extern int mapW[];
extern int mapF[];
extern int mapC[];

void drawMap2D(SDL_Renderer *renderer);
#endif