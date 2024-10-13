#include "SDL.h"

class Player {
public:
    Player(float startX, float startY, float playerSpeed, float playerSize);
    void handlePlayerMovement(SDL_Scancode keyPressed); 
    void drawPlayer(SDL_Renderer* renderer);
private:
    float px, py; // Position
    float PLAYER_SPEED; // Speed
    const float PLAYER_SIZE; // Size
};