#include <iostream>
#include "player.hpp"
#include "map.hpp"
#include "colors.hpp"
#include "config.hpp"

Player::Player(float startX, float startY, float playerSpeed, float playerSize)
    : px(startX), py(startY), pa(0.0f), PLAYER_SIZE(playerSize) {
    pdx = cos(pa) * 5;
    pdy = sin(pa) * 5;
}

void Player::drawPlayer(SDL_Renderer* renderer) {
     SDL_SetRenderDrawColor(renderer, Colors::YELLOW.r, Colors::YELLOW.g, Colors::YELLOW.b, Colors::YELLOW.a);

    SDL_FRect playerRect = {px, py, PLAYER_SIZE, PLAYER_SIZE};
    SDL_RenderFillRectF(renderer, &playerRect);

    SDL_RenderDrawLineF(renderer, px + 4, py + 4, px + pdx * 5, py + pdy * 5);
}

void Player::handlePlayerMovement(SDL_Scancode keyPressed) {
    if (keyPressed == SDL_SCANCODE_D) {
        pa += 0.1;
        if (pa > 2 * PI) {
            pa -= 2 * PI;
        }
        pdx = cos(pa) * 5;
        pdy = sin(pa) * 5;
    }
    if (keyPressed == SDL_SCANCODE_A) {
        pa -= 0.1;
        if (pa < 0) {
            pa += 2 * PI;
        }
        pdx = cos(pa) * 5;
        pdy = sin(pa) * 5;
    }
    if (keyPressed == SDL_SCANCODE_W) {
        py += pdy;
        px += pdx;
    }
    if (keyPressed == SDL_SCANCODE_S) {
        py -= pdy;
        px -= pdx;
    }
}

float dist(float ax, float ay, float bx, float by, float ang) {
    return ( sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay))  );
}

void Player::drawRays2D(SDL_Renderer* renderer) {
    int r, mx, my, mp, dof;
    float rx, ry, ra, xo, yo, disT;
    ra = pa - DR*30; // Ray angle = Player angle - 30 degrees
    // Normalize the angle
    if (ra < 0) ra += 2 * PI;
    if (ra > 2 * PI) ra -= 2 * PI;

    for (r = 0; r < 60; r++) {
        // --- Horizontal Line Checks ---
        dof = 0;
        float disH = 1000000, hx = px, hy = py;
        float aTan = -1 / tan(ra);
        if (ra > PI) { // Looking up
            ry = (((int)py >> 6) << 6) - 0.0001;
            rx = (py - ry) * aTan + px;
            yo = -64;
            xo = -yo * aTan;
        } 
        if (ra < PI) { // Looking down
            ry = (((int)py >> 6) << 6) + 64;
            rx = (py - ry) * aTan + px;
            yo = 64;
            xo = -yo * aTan;
        } 
        if (ra == PI || ra == 0) { // Looking straight left or right
            rx = px;
            ry = py;
            dof = 8; // Skip further checks
        }
        while (dof < 8) {
            mx = (int)(rx) >> 6;
            my = (int)(ry) >> 6;
            mp = my * mapX + mx;
            if (mp > 0 && mp < mapX * mapY && map[mp] == 1) { // Hit a horizontal wall
                hx = rx;
                hy = ry;
                disH = dist(px, py, hx, hy, ra);
                dof = 8;
            } else {
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }
        
        // --- Vertical Line Checks ---
        dof = 0;
        float disV=1000000, vx=px, vy=py;
        float nTan = -tan(ra);
        if (ra > P2 && ra < P3) { // Looking left
            rx = (((int)px >> 6) << 6) - 0.0001;
            ry = (px - rx) * nTan + py;
            xo = -64;
            yo = -xo * nTan;
        } 
        if (ra < P2 || ra > P3) { // Looking right
            rx = (((int)px >> 6) << 6) + 64;
            ry = (px - rx) * nTan + py;
            xo = 64;
            yo = -xo * nTan;
        } 
        if (ra == 0 || ra == PI) { // Looking straight up or down
            rx = px;
            ry = py;
            dof = 8; // Skip further checks
        }

        while (dof < 8) {
            mx = (int)(rx) >> 6;
            my = (int)(ry) >> 6;
            mp = my * mapX + mx;
            if (mp > 0 && mp < mapX * mapY && map[mp] == 1) { // Hit a vertical wall
                vx = rx;
                vy = ry;
                disV = dist(px, py, vx, vy, ra);
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

        SDL_RenderDrawLineF(renderer, px + PLAYER_SIZE / 2, py + PLAYER_SIZE / 2, rx, ry);
        //---Draw 3D Walls---
        float ca=pa-ra; if (ca < 0) { ca += 2 * PI; } if (ca > 2 * PI) { ca -= 2 * PI; } disT = disT * cos(ca); // fix fisheye
        float lineHeight = (mapS*320)/disT; if (lineHeight > 320) { lineHeight = 320; } // set line height based on distance
        float lineOffset = 160-lineHeight/2;
        SDL_FRect rect = { r * 8 + 530, lineOffset, 8, lineHeight+lineOffset };SDL_RenderFillRectF(renderer, &rect); // draw the rectangles
        ra += DR; if (ra < 0) ra += 2 * PI; if (ra > 2 * PI) ra -= 2 * PI;
    }
}
