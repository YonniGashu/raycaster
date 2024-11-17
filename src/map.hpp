#ifndef MAP_H
#define MAP_H
#include <SDL.h>
extern const int mapX, mapY, mapS;
extern int mapW[];
extern int mapF[];
extern int mapC[];

void drawSky(SDL_Renderer *renderer, float playerAngle);
#endif