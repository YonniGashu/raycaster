#include <SDL.h>

#include <iostream>

#include "colors.hpp"
#include "config.hpp"
#include "map.hpp"
#include "player.hpp"

// Window Properties
const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 640;
SDL_Window *g_main_window;
SDL_Renderer *g_main_renderer;

static bool Init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) > 0) {
        std::cout << "SDL Init failed with error: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }
    g_main_window = SDL_CreateWindow(
        "Wolfenstein Style Raycaster",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL);

    if (g_main_window == nullptr) {
        std::cout << "Unable to create the main. Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    g_main_renderer = SDL_CreateRenderer(g_main_window, -1, SDL_RENDERER_PRESENTVSYNC);

    return true;
}

void Shutdown() {
    if (g_main_window != nullptr) {
        SDL_DestroyWindow(g_main_window);
        g_main_window = nullptr;
    }

    if (g_main_renderer != nullptr) {
        SDL_DestroyRenderer(g_main_renderer);
        g_main_renderer = nullptr;
    }

    SDL_Quit();
}

static void ClearScreen(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, Colors::GRAY.r, Colors::GRAY.g, Colors::GRAY.b, Colors::GRAY.a);
    SDL_RenderClear(renderer);
}

int main() {
    if (!Init()) {
        Shutdown();
        return EXIT_FAILURE;
    }

    Player player(300, 300, 1.5, 8);
    SDL_Event event;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    player.handlePlayerInteractions(event.key.keysym.scancode);
                    running = event.key.keysym.scancode != SDL_SCANCODE_ESCAPE;
                    break;
                case SDL_QUIT:
                    running = false;
                    break;
                default:
                    break;
            }
        }

        ClearScreen(g_main_renderer);
        player.handlePlayerMovement();
        drawSky(g_main_renderer, player.playerAngle);
        player.drawRays2D(g_main_renderer);
        SDL_RenderPresent(g_main_renderer);
    }

    Shutdown();
    return EXIT_SUCCESS;
}