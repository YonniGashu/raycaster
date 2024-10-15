#include "SDL.h"
#include "math.h"
#define PI 3.1415926535f
#define P2 PI/2
#define P3 3 * PI/2
#define DR 0.0174533 // one degree in radians

class Player {
public:
    Player(float startX, float startY, float playerSpeed, float playerSize);
    void handlePlayerMovement(SDL_Scancode keyPressed); 
    void drawPlayer(SDL_Renderer* renderer);
    void drawRays2D(SDL_Renderer* renderer);
private:
    float px, py, pdx, pdy, pa; // Position, deltaX, deltaY, and angle
    const float PLAYER_SIZE; // Size
};