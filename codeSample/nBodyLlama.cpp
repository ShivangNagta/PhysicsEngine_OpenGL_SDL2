#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <cmath>
#include <ctime>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float G = 6.67e-11f; // Gravitational constant
const float MASS = 1.0f; // Mass of each body
const float MAX_SPEED = 10.0f; // Maximum speed of bodies
const float TIME_STEP = 0.001f; // Reduced time step for increased simulation speed

struct Body {
    float x, y;
    float vx, vy;
    float mass;

    Body(float x, float y, float mass) : x(x), y(y), mass(mass) {
        vx = (float)(rand() % 10000 - 5000) / 10000.0f;
        vy = (float)(rand() % 10000 - 5000) / 10000.0f;
    }
};

int main(int argc, char* argv[]) {
    srand(time(nullptr));

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("N-Body Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    std::vector<Body> bodies;
    for (int i = 0; i < 100; ++i) {
        float x = (float)(rand() % SCREEN_WIDTH);
        float y = (float)(rand() % SCREEN_HEIGHT);
        bodies.push_back(Body(x, y, MASS));
    }

    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Update bodies
        for (Body& body : bodies) {
            for (Body& other : bodies) {
                if (&body == &other) continue;

                float dx = body.x - other.x;
                float dy = body.y - other.y;
                float distance = sqrtf(dx * dx + dy * dy);
                float force = G * body.mass * other.mass / (distance * distance * distance);

                body.vx += dx / distance * force * TIME_STEP / body.mass;
                body.vy += dy / distance * force * TIME_STEP / body.mass;

                // Limit speed
                float speed = sqrtf(body.vx * body.vx + body.vy * body.vy);
                if (speed > MAX_SPEED) {
                    body.vx *= MAX_SPEED / speed;
                    body.vy *= MAX_SPEED / speed;
                }
            }

            body.x += body.vx * TIME_STEP;
            body.y += body.vy * TIME_STEP;

            // Wrap around screen edges
            if (body.x < 0) body.x += SCREEN_WIDTH;
            if (body.x >= SCREEN_WIDTH) body.x -= SCREEN_WIDTH;
            if (body.y < 0) body.y += SCREEN_HEIGHT;
            if (body.y >= SCREEN_HEIGHT) body.y -= SCREEN_HEIGHT;
        }

        // Draw bodies
        for (const Body& body : bodies) {
            SDL_Rect rect = {static_cast<int>(body.x - 1), static_cast<int>(body.y - 1), 10, 10}; // Reduced particle size
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &rect);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(1); // Reduced delay for increased simulation speed
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}