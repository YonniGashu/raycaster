#include <iostream>
#include "player.hpp"
#include "map.hpp"
#include "colors.hpp"
#include "config.hpp"

Uint32 previousFrameTime = SDL_GetTicks();

Player::Player(float startX, float startY, float playerSpeed, float playerSize)
    : playerX(startX), playerY(startY), playerAngle(0.0f), PLAYER_SIZE(playerSize) {
    playerDeltaX = cos(playerAngle) * 5;
    playerDeltaY = sin(playerAngle) * 5;
}

void Player::drawPlayer(SDL_Renderer* renderer) {
     SDL_SetRenderDrawColor(renderer, Colors::YELLOW.r, Colors::YELLOW.g, Colors::YELLOW.b, Colors::YELLOW.a);

    SDL_FRect playerRect = {playerX, playerY, PLAYER_SIZE, PLAYER_SIZE};
    SDL_RenderFillRectF(renderer, &playerRect);

    SDL_RenderDrawLineF(renderer, playerX + 4, playerY + 4, playerX + playerDeltaX * 5, playerY + playerDeltaY * 5);
}

void Player::handlePlayerMovement() {
    const Uint8* keyState = SDL_GetKeyboardState(NULL); // Get the current key state

    // Get current time & deltaTime
    Uint32 currentFrameTime = SDL_GetTicks();
    float deltaTime = (currentFrameTime - previousFrameTime) / 1000.0f;
    previousFrameTime = currentFrameTime;

    //---HANDLE TURNING---
    if (keyState[SDL_SCANCODE_D]) {
        playerAngle += 0.05;
        if (playerAngle > 2 * PI) {
            playerAngle -= 2 * PI;
        }
        playerDeltaX = cos(playerAngle) * 5;
        playerDeltaY = sin(playerAngle) * 5;
    }
    if (keyState[SDL_SCANCODE_A]) {
        playerAngle -= 0.05;
        if (playerAngle < 0) {
            playerAngle += 2 * PI;
        }
        playerDeltaX = cos(playerAngle) * 5;
        playerDeltaY = sin(playerAngle) * 5;
    }
    //---HANDLE MOVEMENT & COLLISIONS---
    int xOffset = 0; 
    if (playerDeltaX < 0) { 
        xOffset =-20; 
    } else { 
        xOffset = 20; 
    }

    int yOffset = 0;
    if (playerDeltaY < 0) { 
        yOffset = -20;
    } else {
        yOffset = 20;
    }
    int ipx = playerX / 64.0; 
    int ipx_add_xo = (playerX + xOffset) / 64.0;
    int ipx_sub_xo = (playerX - xOffset) / 64.0; // player grid position, +/- x-offset
    int ipy = playerY / 64.0;
    int ipy_add_yo = (playerY + yOffset) / 64.0;
    int ipy_sub_yo = (playerY - yOffset) / 64.0; // player grid position, +/- y-offset

    if (keyState[SDL_SCANCODE_W]) {
        if (map[ipy * mapX + ipx_add_xo] == 0) { 
            playerX += playerDeltaX * deltaTime * 30.0f; 
        }
        if (map[ipy_add_yo * mapX + ipx] == 0) { 
            playerY += playerDeltaY * deltaTime * 30.0f;
        }   
    }

    if (keyState[SDL_SCANCODE_S]) {
       if (map[ipy * mapX + ipx_sub_xo] == 0) { 
            playerX -= playerDeltaX * deltaTime * 30.0f; 
        }
        if (map[ipy_sub_yo * mapX + ipx] == 0) { 
            playerY -= playerDeltaY * deltaTime * 30.0f;
        }
    }
}

float dist(float ax, float ay, float bx, float by, float ang) {
    return ( sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay))  );
}

void Player::drawRays2D(SDL_Renderer* renderer) {
    int r, mx, my, mp, dof;
    float rx, ry, ra, xo, yo, disT;
    ra = playerAngle - DR*30; // Ray angle = Player angle - 30 degrees
    // Normalize the angle
    if (ra < 0) ra += 2 * PI;
    if (ra > 2 * PI) ra -= 2 * PI;

    for (r = 0; r < 60; r++) {
        // --- Horizontal Line Checks ---
        dof = 0;
        float disH = 1000000, hx = playerX, hy = playerY;
        float aTan = -1 / tan(ra);
        if (ra > PI) { // Looking up
            ry = (((int)playerY >> 6) << 6) - 0.0001;
            rx = (playerY - ry) * aTan + playerX;
            yo = -64;
            xo = -yo * aTan;
        } 
        if (ra < PI) { // Looking down
            ry = (((int)playerY >> 6) << 6) + 64;
            rx = (playerY - ry) * aTan + playerX;
            yo = 64;
            xo = -yo * aTan;
        } 
        if (ra == PI || ra == 0) { // Looking straight left or right
            rx = playerX;
            ry = playerY;
            dof = 8; // Skip further checks
        }
        while (dof < 8) {
            mx = (int)(rx) >> 6;
            my = (int)(ry) >> 6;
            mp = my * mapX + mx;
            if (mp > 0 && mp < mapX * mapY && map[mp] == 1) { // Hit a horizontal wall
                hx = rx;
                hy = ry;
                disH = dist(playerX, playerY, hx, hy, ra);
                dof = 8;
            } else {
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }
        
        // --- Vertical Line Checks ---
        dof = 0;
        float disV=1000000, vx=playerX, vy=playerY;
        float nTan = -tan(ra);
        if (ra > P2 && ra < P3) { // Looking left
            rx = (((int)playerX >> 6) << 6) - 0.0001;
            ry = (playerX - rx) * nTan + playerY;
            xo = -64;
            yo = -xo * nTan;
        } 
        if (ra < P2 || ra > P3) { // Looking right
            rx = (((int)playerX >> 6) << 6) + 64;
            ry = (playerX - rx) * nTan + playerY;
            xo = 64;
            yo = -xo * nTan;
        } 
        if (ra == 0 || ra == PI) { // Looking straight up or down
            rx = playerX;
            ry = playerY;
            dof = 8; // Skip further checks
        }

        while (dof < 8) {
            mx = (int)(rx) >> 6;
            my = (int)(ry) >> 6;
            mp = my * mapX + mx;
            if (mp > 0 && mp < mapX * mapY && map[mp] == 1) { // Hit a vertical wall
                vx = rx;
                vy = ry;
                disV = dist(playerX, playerY, vx, vy, ra);
                dof = 8;
            } else {
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }
        if (disV < disH) 
        { rx = vx; ry = vy; disT = disV; SDL_SetRenderDrawColor(renderer, Colors::V.r, Colors::V.g, Colors::V.b, Colors::V.a);} 
        else 
        { rx = hx; ry = hy; disT = disH; SDL_SetRenderDrawColor(renderer, Colors::H.r, Colors::H.g, Colors::H.b, Colors::H.a);} // different colors allow for V and H => easy shading

        SDL_RenderDrawLineF(renderer, playerX + PLAYER_SIZE / 2, playerY + PLAYER_SIZE / 2, rx, ry);
        //---Draw 3D Walls---
        float ca=playerAngle-ra; if (ca < 0) { ca += 2 * PI; } if (ca > 2 * PI) { ca -= 2 * PI; } disT = disT * cos(ca); // fix fisheye
        float lineHeight = (mapS*320)/disT; if (lineHeight > 320) { lineHeight = 320; } // set line height based on distance
        float lineOffset = 160-lineHeight/2;
        SDL_FRect rect = { r * 8 + 530, lineOffset, 8, lineHeight+lineOffset };SDL_RenderFillRectF(renderer, &rect); // draw the rectangles
        ra += DR; if (ra < 0) ra += 2 * PI; if (ra > 2 * PI) ra -= 2 * PI;
    }
}
