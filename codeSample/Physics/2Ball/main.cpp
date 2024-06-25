#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>

class Circle {
private:
    int centerX, centerY, radius, vx, vy;

public:
    Circle(int centerX, int centerY, int radius, int vx = 0, int vy = 0)
        : centerX(centerX), centerY(centerY), radius(radius), vx(vx), vy(vy) {}

    void draw(SDL_Renderer *renderer) {
        const int diameter = (radius * 2);
        int x = (radius - 1);
        int y = 0;
        int tx = 1;
        int ty = 1;
        int error = (tx - diameter);

        while (x >= y) {
            // Drawing horizontal lines for each segment of the circle
            SDL_RenderDrawLine(renderer, centerX - x, centerY - y, centerX + x, centerY - y);
            SDL_RenderDrawLine(renderer, centerX - y, centerY - x, centerX + y, centerY - x);
            SDL_RenderDrawLine(renderer, centerX - x, centerY + y, centerX + x, centerY + y);
            SDL_RenderDrawLine(renderer, centerX - y, centerY + x, centerX + y, centerY + x);

            if (error <= 0) {
                ++y;
                error += ty;
                ty += 2;
            }

            if (error > 0) {
                --x;
                tx += 2;
                error += (tx - diameter);
            }
        }
    }

    int getVx() const {
        return vx;
    }

    void setVx(int newVx) {
        vx = newVx;
    }

    int getVy() const {
        return vy;
    }

    void setVy(int newVy) {
        vy = newVy;
    }

    int getX() const {
        return centerX;
    }

    void setX(int newX) {
        centerX = newX;
    }

    int getY() const {
        return centerY;
    }

    void setY(int newY) {
        centerY = newY;
    }

    int getRadius() {
        return radius;
    }
};

void handleCollision(Circle &circle1, Circle &circle2) {
    int x1 = circle1.getX(), y1 = circle1.getY();
    int x2 = circle2.getX(), y2 = circle2.getY();
    int vx1 = circle1.getVx(), vy1 = circle1.getVy();
    int vx2 = circle2.getVx(), vy2 = circle2.getVy();
    int r1 = circle1.getRadius(), r2 = circle2.getRadius();

    int dx = x2 - x1;
    int dy = y2 - y1;
    int distance = sqrt(dx * dx + dy * dy);

    if (distance <= r1 + r2) {
        float angle = atan2(dy, dx);
        float sinAngle = sin(angle);
        float cosAngle = cos(angle);

        // Rotate velocities to align with the collision axis
        float v1xRot = vx1 * cosAngle + vy1 * sinAngle;
        float v1yRot = vy1 * cosAngle - vx1 * sinAngle;
        float v2xRot = vx2 * cosAngle + vy2 * sinAngle;
        float v2yRot = vy2 * cosAngle - vx2 * sinAngle;

        // Swap the x components of the rotated velocities
        float v1xRotNew = v2xRot;
        float v2xRotNew = v1xRot;

        // Rotate velocities back to the original coordinate system
        vx1 = v1xRotNew * cosAngle - v1yRot * sinAngle;
        vy1 = v1yRot * cosAngle + v1xRotNew * sinAngle;
        vx2 = v2xRotNew * cosAngle - v2yRot * sinAngle;
        vy2 = v2yRot * cosAngle + v2xRotNew * sinAngle;

        circle1.setVx(vx1);
        circle1.setVy(vy1);
        circle2.setVx(vx2);
        circle2.setVy(vy2);
    }
}

void UpdatePosAndVel(Circle &circle, float deltaTime) {
    int x = circle.getX(), y = circle.getY();
    int vx = circle.getVx(), vy = circle.getVy();
    x += vx * deltaTime;
    y += vy * deltaTime;

    int RADIUS = circle.getRadius();

    if (y + RADIUS > 600) {
        y = 600 - RADIUS;
        vy = -vy;
    } else if (y - RADIUS < 0) {
        y = RADIUS;
        vy = -vy;
    }

    if (x + RADIUS > 800) {
        x = 800 - RADIUS;
        vx = -vx;
    } else if (x - RADIUS < 0) {
        x = RADIUS;
        vx = -vx;
    }

    circle.setX(x);
    circle.setY(y);
    circle.setVx(vx);
    circle.setVy(vy);
}

void UpdateVelocities(Circle &circle, float ax, float ay, float deltaTime) {
    int vx = circle.getVx();
    int vy = circle.getVy();

    vy += ay * deltaTime;
    vx += ax * deltaTime;

    circle.setVx(vx);
    circle.setVy(vy);
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    float ax = 0.0f, ay = 4000.8f;
    float E = 1.0f;
    const float TARGET_FPS = 60.0f;
    const Uint32 frameDelay = 1000 / TARGET_FPS;

    SDL_Window *window = SDL_CreateWindow("SDL2 Circle", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    Circle ball1(200, 300, 50, 400, 300);
    Circle ball2(500, 200, 50, -400, 300);

    int quit = 0;
    SDL_Event e;
    Uint32 frameStart, frameTime;
    float deltaTime = 0.0f;

    while (!quit) {
        frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = 1;
        }
        
        // Update positions based on current velocities
        UpdatePosAndVel(ball1, deltaTime);
        UpdatePosAndVel(ball2, deltaTime);
        
        // Handle collisions
        handleCollision(ball1, ball2);

        // Update velocities based on acceleration after handling collisions
        UpdateVelocities(ball1, ax, ay, deltaTime);
        UpdateVelocities(ball2, ax, ay, deltaTime);

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        ball1.draw(renderer);
        ball2.draw(renderer);
        SDL_RenderPresent(renderer);

        // Debug output for tracking
        printf("Ball1 - X: %d, Y: %d, Vx: %d, Vy: %d\n", ball1.getX(), ball1.getY(), ball1.getVx(), ball1.getVy());
        printf("Ball2 - X: %d, Y: %d, Vx: %d, Vy: %d\n", ball2.getX(), ball2.getY(), ball2.getVx(), ball2.getVy());

        frameTime = SDL_GetTicks() - frameStart;
        deltaTime = frameTime / 1000.0f;

        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
            deltaTime = frameDelay / 1000.0f;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
