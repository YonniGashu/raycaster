#include <iostream>
#include "player.h"
#include "colors.h"
#include "config.h"

Player::Player(float startX, float startY, float playerSpeed, float playerSize)
    : px(startX), py(startY), PLAYER_SPEED(playerSpeed), PLAYER_SIZE(playerSize) {
}

void Player::drawPlayer(SDL_Renderer* renderer) {
     SDL_SetRenderDrawColor(renderer, Colors::YELLOW.r, Colors::YELLOW.g, Colors::YELLOW.b, Colors::YELLOW.a);

    SDL_FRect playerRect = {px, py, PLAYER_SIZE, PLAYER_SIZE};
    SDL_RenderFillRectF(renderer, &playerRect);
}

void Player::handlePlayerMovement(SDL_Scancode keyPressed) {
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