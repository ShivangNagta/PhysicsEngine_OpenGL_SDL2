#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void fillCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius) {
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

int main(int argc, char *argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    float x = 400.0f, y = 300.0f;
    float vx = 500.0f, vy = 0.0f;
    float ax = 0.0f, ay = 4000.8f;
    float E = 0.95f;
    const float RADIUS = 40.0f;
    const float TARGET_FPS = 60.0f;
    const Uint32 frameDelay = 1000 / TARGET_FPS;

    // Create window
    SDL_Window *window = SDL_CreateWindow("SDL2 Filled Circle", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Seed the random number generator
    srand(time(NULL));

    // Main loop flag
    int quit = 0;

    // Event handler
    SDL_Event e;

    // Time management
    Uint32 frameStart, frameTime;
    float deltaTime = 0.0f;

    // While application is running
    while (!quit) {
        frameStart = SDL_GetTicks();

        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

    // Update positions using previous velocities
    x = x + vx * deltaTime;
    y = y + vy * deltaTime;

    // Handle boundary collisions
    if (y + RADIUS > 600 || y - RADIUS < 0) {
        vy = -vy * E; // Reverse the vertical velocity and apply coefficient of restitution
        y = y + vy * deltaTime; // Update position again with the new velocity
    }

    if (x + RADIUS > 800 || x - RADIUS < 0) {
        vx = -vx * E; // Reverse the horizontal velocity and apply coefficient of restitution
        x = x + vx * deltaTime; // Update position again with the new velocity
    }

    // Ensure the ball stays within the boundaries
    if (y + RADIUS > 600) {
        y = 600 - RADIUS;
    } else if (y - RADIUS < 0) {
        y = RADIUS;
    }

    if (x + RADIUS > 800) {
        x = 800 - RADIUS;
    } else if (x - RADIUS < 0) {
        x = RADIUS;
    }

    // Update velocities using accelerations
    vy = vy + ay * deltaTime;
    vx = vx + ax * deltaTime;



        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);

        // Draw the circle
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        fillCircle(renderer, (int)x, (int)y, (int)RADIUS);

        // Update the screen
        SDL_RenderPresent(renderer);

        // Calculate the time taken for the frame
        frameTime = SDL_GetTicks() - frameStart;

        // Calculate deltaTime (in seconds)
        deltaTime = frameTime / 1000.0f;

        // If frame rendering was faster than the target time, delay to cap the FPS
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
            deltaTime = frameDelay / 1000.0f;  // Use the target frame time for stable physics update
        }
    }

    // Destroy window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
