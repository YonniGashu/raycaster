#include "SDL.h"
#include "math.h"
#define PI 3.1415926535f
#define P2 PI / 2
#define P3 3 * PI / 2
#define DR 0.0174533  // one degree in radians

float dist(float ax, float ay, float bx, float by, float ang);
float fixAngle(float ang);
float fixAngleD(float deg);
float radToDeg(float rad);

class Player {
   public:
    Player(float startX, float startY, float playerSpeed, float playerSize);
    float playerAngle;
    void handlePlayerMovement();
    void handlePlayerInteractions(SDL_Scancode keyPressed);
    void drawPlayer(SDL_Renderer *renderer);
    void drawRays2D(SDL_Renderer *renderer);

   private:
    float playerX, playerY, playerDeltaX, playerDeltaY;  // Position, deltaX, deltaY, and angle
    const float PLAYER_SIZE;                             // Size
};