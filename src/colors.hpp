#ifndef COLORS_H
#define COLORS_H

#include <SDL.h>

namespace Colors {
    const SDL_Color BLACK = { 0, 0, 0, SDL_ALPHA_OPAQUE };
    const SDL_Color GRAY = { 110, 110, 110, SDL_ALPHA_OPAQUE };
    const SDL_Color YELLOW = { 255, 255, 0, SDL_ALPHA_OPAQUE };
    const SDL_Color WHITE = { 255, 255, 255, SDL_ALPHA_OPAQUE };
}

#endif 