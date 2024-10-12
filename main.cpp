#include <iostream>
#include <SDL.h>
#include "Headers/Colors.h"

// Window Properties
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 512;
SDL_Window* g_main_window;
SDL_Renderer* g_main_renderer;

// Player
float px, py; // Position
float PLAYER_SPEED = 8.0f; // Speed
const float PLAYER_SIZE = 8.0f; // Size

// Map
int mapX = 8, mapY = 8, mapS = 64;
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
        SDL_WINDOW_OPENGL
    );

    if (g_main_window == nullptr) {
        std::cout << "Unable to create the main. Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    g_main_renderer = SDL_CreateRenderer(g_main_window, -1, SDL_RENDERER_PRESENTVSYNC);
    px = 300;
    py = 300;

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

static void ClearScreen(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, Colors::GRAY.r, Colors::GRAY.g, Colors::GRAY.b, Colors::GRAY.a);
    SDL_RenderClear(renderer);
}

#pragma region PLAYER
void drawPlayer(SDL_Renderer* renderer) {
     SDL_SetRenderDrawColor(renderer, Colors::YELLOW.r, Colors::YELLOW.g, Colors::YELLOW.b, Colors::YELLOW.a);

    SDL_FRect playerRect;
    playerRect.x = px;
    playerRect.y = py;
    playerRect.w = 8;
    playerRect.h = 8; 

    SDL_RenderFillRectF(renderer, &playerRect);
}

void handlePlayerMovement(SDL_Scancode keyPressed) {
    if (keyPressed == SDL_SCANCODE_D) {
        px += std::min(PLAYER_SPEED, WINDOW_WIDTH - (px + PLAYER_SIZE));
    }
    if (keyPressed == SDL_SCANCODE_A) {
        px -= std::min(PLAYER_SPEED, px);  
    }
    if (keyPressed == SDL_SCANCODE_W) {
        py -= std::min(PLAYER_SPEED, py);
    }
    if (keyPressed == SDL_SCANCODE_S) {
        py += std::min(PLAYER_SPEED, WINDOW_HEIGHT - (py + PLAYER_SIZE));
    }
}
#pragma endregion
    

#pragma region MAP
void drawMap2D() {
    int x, y, xo, yo;

    for (y = 0; y < mapY; y++) {
        for (x = 0; x < mapX; x++) {
            xo = x * mapS;
            yo = y * mapS;

            if (map[y * mapX + x] == 1) {
                SDL_SetRenderDrawColor(g_main_renderer, Colors::WHITE.r, Colors::WHITE.g, Colors::WHITE.b, Colors::WHITE.a);
            } else {
                SDL_SetRenderDrawColor(g_main_renderer, Colors::BLACK.r, Colors::BLACK.g, Colors::BLACK.b, Colors::BLACK.a);
            }

            SDL_Rect rect = { xo, yo, mapS - 1, mapS - 1};
            SDL_RenderFillRect(g_main_renderer, &rect);
        }
    }
}
#pragma endregion
int main() {
    if (!Init()) {
        Shutdown();
        return EXIT_FAILURE;
    }
    
    // Draw loop
    SDL_Event event;
    bool running = true;

    while (running) {
        ClearScreen(g_main_renderer);
        drawMap2D();
        drawPlayer(g_main_renderer);
        // Check and process I/O Events
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_KEYDOWN:
                handlePlayerMovement(event.key.keysym.scancode);
                running = event.key.keysym.scancode != SDL_SCANCODE_ESCAPE;
                break;
            case SDL_QUIT:
                running = false;
                break;
            default:
                break;
            }
        }

        // Update the screen with content from renderer
        SDL_RenderPresent(g_main_renderer);
    }

    Shutdown();
    return EXIT_SUCCESS;
}