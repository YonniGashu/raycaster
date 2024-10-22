#include "player.hpp"

#include <iostream>

#include "colors.hpp"
#include "config.hpp"
#include "map.hpp"
#include "texture.hpp"

Uint32 previousFrameTime = SDL_GetTicks();

Player::Player(float startX, float startY, float playerSpeed, float playerSize)
    : playerX(startX), playerY(startY), playerAngle(0.0f), PLAYER_SIZE(playerSize) {
    playerDeltaX = cos(playerAngle) * 5;
    playerDeltaY = sin(playerAngle) * 5;
}

void Player::drawPlayer(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, Colors::YELLOW.r, Colors::YELLOW.g, Colors::YELLOW.b, Colors::YELLOW.a);

    SDL_FRect playerRect = {playerX, playerY, PLAYER_SIZE, PLAYER_SIZE};
    SDL_RenderFillRectF(renderer, &playerRect);

    SDL_RenderDrawLineF(renderer, playerX + 4, playerY + 4, playerX + playerDeltaX * 5, playerY + playerDeltaY * 5);
}

void Player::handlePlayerMovement() {
    const Uint8 *keyState = SDL_GetKeyboardState(NULL);  // Get the current key state

    // Get current time & deltaTime
    Uint32 currentFrameTime = SDL_GetTicks();
    float deltaTime = (currentFrameTime - previousFrameTime) / 1000.0f;
    previousFrameTime = currentFrameTime;

    //---HANDLE TURNING---
    if (keyState[SDL_SCANCODE_D]) {
        playerAngle = fixAngle(playerAngle + 0.05);

        playerDeltaX = cos(playerAngle) * 5;
        playerDeltaY = sin(playerAngle) * 5;
    }
    if (keyState[SDL_SCANCODE_A]) {
        playerAngle = fixAngle(playerAngle - 0.05);

        playerDeltaX = cos(playerAngle) * 5;
        playerDeltaY = sin(playerAngle) * 5;
    }
    //---HANDLE MOVEMENT & COLLISIONS---
    int xOffset = 0;
    if (playerDeltaX < 0) {
        xOffset = -20;
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
    int ipx_sub_xo = (playerX - xOffset) / 64.0;  // player grid position, +/- x-offset
    int ipy = playerY / 64.0;
    int ipy_add_yo = (playerY + yOffset) / 64.0;
    int ipy_sub_yo = (playerY - yOffset) / 64.0;  // player grid position, +/- y-offset

    if (keyState[SDL_SCANCODE_W]) {
        if (mapW[ipy * mapX + ipx_add_xo] == 0) {
            playerX += playerDeltaX * deltaTime * 30.0f;
        }
        if (mapW[ipy_add_yo * mapX + ipx] == 0) {
            playerY += playerDeltaY * deltaTime * 30.0f;
        }
    }

    if (keyState[SDL_SCANCODE_S]) {
        if (mapW[ipy * mapX + ipx_sub_xo] == 0) {
            playerX -= playerDeltaX * deltaTime * 30.0f;
        }
        if (mapW[ipy_sub_yo * mapX + ipx] == 0) {
            playerY -= playerDeltaY * deltaTime * 30.0f;
        }
    }
}

void Player::handlePlayerInteractions(SDL_Scancode keyPressed) {
    if (keyPressed == SDL_SCANCODE_E) {
        int xOffset = 0;
        int yOffset = 0;

        if (playerDeltaX < 0) {
            xOffset = -25;
        } else {
            xOffset = 25;
        }

        if (playerDeltaY < 0) {
            yOffset = -25;
        } else {
            yOffset = 25;
        }

        int ipx = playerX / 64.0;
        int ipx_add_xo = (playerX + xOffset) / 64.0;
        int ipy = playerY / 64.0;
        int ipy_add_yo = (playerY + yOffset) / 64.0;

        // IF IN RANGE OF A DOOR, OPEN IT WHEN E IS PRESSED
        if (mapW[ipy_add_yo * mapX + ipx_add_xo] == 4) {
            mapW[ipy_add_yo * mapX + ipx_add_xo] = 0;
        }
    }
}

void Player::drawRays2D(SDL_Renderer *renderer) {
    int mx, my, mp, dof;
    float rayX, rayY, rayAngle, xOffset, yOffset, disT;
    rayAngle = fixAngle(playerAngle - DR * 30);  // Ray angle = Player angle - 30 degrees

    for (int r = 0; r < 60; r++) {
        // Vertical and Horizontal Map Texture Number
        int vmt = 0;
        int hmt = 0;

        // --- Horizontal Line Checks ---
        dof = 0;
        float disH = 1000000;
        float hx = playerX;
        float hy = playerY;
        float aTan = -1 / tan(rayAngle);

        // Looking up
        if (rayAngle > PI) {
            rayY = (((int)playerY >> 6) << 6) - 0.0001;
            rayX = (playerY - rayY) * aTan + playerX;
            yOffset = -64;
            xOffset = -yOffset * aTan;
        }

        // Looking down
        if (rayAngle < PI) {
            rayY = (((int)playerY >> 6) << 6) + 64;
            rayX = (playerY - rayY) * aTan + playerX;
            yOffset = 64;
            xOffset = -yOffset * aTan;
        }

        // Looking straight left or right
        if (rayAngle == PI || rayAngle == 0) {
            rayX = playerX;
            rayY = playerY;
            dof = 8;  // Skip further checks
        }
        while (dof < 8) {
            mx = (int)(rayX) >> 6;
            my = (int)(rayY) >> 6;
            mp = my * mapX + mx;
            if (mp > 0 && mp < mapX * mapY && mapW[mp] > 0) {  // Hit a horizontal wall
                hmt = mapW[mp] - 1;
                hx = rayX;
                hy = rayY;
                disH = dist(playerX, playerY, hx, hy, rayAngle);
                dof = 8;
            } else {
                rayX += xOffset;
                rayY += yOffset;
                dof += 1;
            }
        }

        // --- Vertical Line Checks ---
        dof = 0;
        float disV = 1000000, vx = playerX, vy = playerY;
        float nTan = -tan(rayAngle);

        // Looking left
        if (rayAngle > P2 && rayAngle < P3) {
            rayX = (((int)playerX >> 6) << 6) - 0.0001;
            rayY = (playerX - rayX) * nTan + playerY;
            xOffset = -64;
            yOffset = -xOffset * nTan;
        }

        // Looking right
        if (rayAngle < P2 || rayAngle > P3) {
            rayX = (((int)playerX >> 6) << 6) + 64;
            rayY = (playerX - rayX) * nTan + playerY;
            xOffset = 64;
            yOffset = -xOffset * nTan;
        }

        // Looking straight up or down
        if (rayAngle == 0 || rayAngle == PI) {
            rayX = playerX;
            rayY = playerY;
            dof = 8;  // Skip further checks
        }

        while (dof < 8) {
            mx = (int)(rayX) >> 6;
            my = (int)(rayY) >> 6;
            mp = my * mapX + mx;

            // Hit a vertical wall
            if (mp > 0 && mp < mapX * mapY && mapW[mp] > 0) {
                vmt = mapW[mp] - 1;
                vx = rayX;
                vy = rayY;
                disV = dist(playerX, playerY, vx, vy, rayAngle);
                dof = 8;
            } else {
                rayX += xOffset;
                rayY += yOffset;
                dof += 1;
            }
        }
        float shade = 1;
        if (disV < disH) {
            hmt = vmt;
            shade = 0.5f;
            rayX = vx;
            rayY = vy;
            disT = disV;
            SDL_SetRenderDrawColor(renderer, Colors::V.r, Colors::V.g, Colors::V.b, Colors::V.a);
        } else {
            rayX = hx;
            rayY = hy;
            disT = disH;
            SDL_SetRenderDrawColor(renderer, Colors::H.r, Colors::H.g, Colors::H.b, Colors::H.a);  // different colors allow for V and H => easy shading
        }
        SDL_RenderDrawLineF(renderer, playerX + PLAYER_SIZE / 2, playerY + PLAYER_SIZE / 2, rayX, rayY);

        // --- Draw 3D Walls ---
        float ca = fixAngle(playerAngle - rayAngle);
        disT = disT * cos(ca);  // fix fisheye

        int lineHeight = (mapS * 320) / disT;
        float textureY_Step = 32.0 / (float)lineHeight;
        float textureY_Offset = 0;

        if (lineHeight > 320) {
            textureY_Offset = (lineHeight - 320) / 2.0;
            lineHeight = 320;  // set line height based on distance
        }

        int lineOffset = 160 - (lineHeight >> 1);

        float textureY = textureY_Offset * textureY_Step + hmt * 32;
        float textureX;

        if (shade == 1) {
            textureX = (int)(rayX / 2.0) % 32;
            if (rayAngle > PI) {
                textureX = 31 - textureX;
            }
        } else {
            textureX = (int)(rayY / 2.0) % 32;
            if (rayAngle > PI / 2 && rayAngle < 3 * PI / 2) {
                textureX = 31 - textureX;
            }
        }

        for (int y = 0; y < lineHeight; y++) {
            float c = All_Textures[(int)(textureY) * 32 + (int)(textureX)] * shade;
            switch (hmt) {
                case 0:  // Checkerboard Red
                    SDL_SetRenderDrawColor(renderer, (Uint8)(c * 255), (Uint8)(c * 255) / 2, (Uint8)(c * 255) / 2, SDL_ALPHA_OPAQUE);
                    break;
                case 1:  // Brick Yellow
                    SDL_SetRenderDrawColor(renderer, (Uint8)(c * 255), (Uint8)(c * 255), (Uint8)(c * 255) / 2, SDL_ALPHA_OPAQUE);
                    break;
                case 2:  // Window Purple
                    SDL_SetRenderDrawColor(renderer, (Uint8)(c * 255), (Uint8)(c * 255) / 2, (Uint8)(c * 255), SDL_ALPHA_OPAQUE);
                    break;
                case 3:  // Door Green
                    SDL_SetRenderDrawColor(renderer, (Uint8)(c * 255) / 2, (Uint8)(c * 255), (Uint8)(c * 255) / 2, SDL_ALPHA_OPAQUE);
                    break;
            }
            SDL_FRect rect = {r * 8 + 530, y + lineOffset, 8, 1};
            SDL_RenderFillRectF(renderer, &rect);
            textureY += textureY_Step;
        }

        // --- Draw Floors & Ceiling---
        for (int y = lineOffset + lineHeight; y < 320; y++) {
            float deltaY = y - (320 / 2.0f);
            float deg = rayAngle;
            float rayAngleFix = cos(fixAngle(playerAngle - rayAngle));

            textureX = playerX / 2 + cos(deg) * 158 * 32 / deltaY / rayAngleFix;
            textureY = playerY / 2 + sin(deg) * 158 * 32 / deltaY / rayAngleFix;

            // FLOORS
            int mp = mapF[(int)(textureY / 32.0) * mapX + (int)(textureX / 32.0)] * 32 * 32;
            float c = All_Textures[((int)(textureY) & 31) * 32 + ((int)(textureX) & 31) + mp] * 0.7f;
            SDL_SetRenderDrawColor(renderer, (Uint8)(c * 255 / 2), (Uint8)(c * 255 / 2), (Uint8)(c * 255), SDL_ALPHA_OPAQUE);
            SDL_FRect rect = {r * 8 + 530, y, 8, 1};
            SDL_RenderFillRectF(renderer, &rect);

            // CEILINGS
            mp = mapC[(int)(textureY / 32.0) * mapX + (int)(textureX / 32.0)] * 32 * 32;
            c = All_Textures[((int)(textureY) & 31) * 32 + ((int)(textureX) & 31) + mp] * 0.7f;
            SDL_SetRenderDrawColor(renderer, (Uint8)(c * 255 / 2), (Uint8)(c * 255), (Uint8)(c * 255 / 2), SDL_ALPHA_OPAQUE);
            SDL_FRect rectC = {r * 8 + 530, 320 - y, 8, 1};
            SDL_RenderFillRectF(renderer, &rectC);
        }

        rayAngle = fixAngle(rayAngle + DR);
    }
}

// --- HELPER METHODS ---
float dist(float ax, float ay, float bx, float by, float ang) {
    return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

float radToDeg(float rad) {
    return rad * 180 / M_PI;
}

float fixAngle(float ang) {
    if (ang < 0) {
        ang += 2 * PI;
    }

    if (ang > 2 * PI) {
        ang -= 2 * PI;
    }

    return ang;
}