#include "map.h"
#include "colors.h"
#include <SDL.h>

const int mapX = 8;
const int mapY = 8;
const int mapS = 64;

int map[] = {
    1,1,1,1,1,1,1,1,
    1,0,1,0,0,0,0,1,
    1,0,1,0,0,0,0,1,
    1,0,1,0,0,1,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1
};


void drawMap2D(SDL_Renderer* renderer) {
    int x, y, xo, yo;

    for (y = 0; y < mapY; y++) {
        for (x = 0; x < mapX; x++) {
            xo = x * mapS;
            yo = y * mapS;

            if (map[y * mapX + x] == 1) {
                SDL_SetRenderDrawColor(renderer, Colors::WHITE.r, Colors::WHITE.g, Colors::WHITE.b, Colors::WHITE.a);
            } else {
                SDL_SetRenderDrawColor(renderer, Colors::BLACK.r, Colors::BLACK.g, Colors::BLACK.b, Colors::BLACK.a);
            }

            SDL_Rect rect = { xo, yo, mapS - 1, mapS - 1};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}