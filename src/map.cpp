#include "map.hpp"

#include <SDL.h>

#include "player.hpp"
#include "texture.hpp"

const int mapX = 8;
const int mapY = 8;
const int mapS = 64;

// WALL MAP
int mapW[] = {
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 0, 0, 0, 0, 0, 0, 8,
    1, 1, 1, 2, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1};

// FLOOR MAP
int mapF[] = {
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 8, 8, 8, 8, 8, 8, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 0};

// CEILING MAP
int mapC[] = {
    -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 1, 2, 3, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5};

void drawSky(SDL_Renderer *renderer, float playerAngle) {
    for (int y = 0; y < 40; y++) {
        for (int x = 0; x < 120; x++) {
            int xo = (int)radToDeg(playerAngle) * .25 - x;
            if (xo < 0) {
                xo += 120;
            }
            xo = xo % 120;
            int pixel = (y * 120 + xo) * 3;
            int red = sky1[pixel + 0];
            int green = sky1[pixel + 1];
            int blue = sky1[pixel + 2];
            SDL_SetRenderDrawColor(renderer, red, green, blue, SDL_ALPHA_OPAQUE);
            SDL_FRect rect = {static_cast<float>(x * 8), static_cast<float>(y * 8), 8, 8};
            SDL_RenderFillRectF(renderer, &rect);
        }
    }
}
