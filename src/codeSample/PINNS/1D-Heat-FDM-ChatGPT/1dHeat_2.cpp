#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <algorithm>

// Simulation parameters
const int nx = 100; // Number of spatial points
const double length = 1.0; // Length of the rod
const double dx = length / (nx - 1); // Spatial step size
const double alpha = 0.1; // Thermal diffusivity
const double dt = 0.0005; // Time step size

// SDL2 parameters
const int window_width = 800;
const int window_height = 200;

// Fixed temperature range for color mapping
const double min_temp = 0.0; // Minimum temperature in the range (e.g., 0°C)
const double max_temp = 1000.0; // Maximum temperature in the range (e.g., 100°C)

// Function to initialize the temperature distribution
void initializeTemperature(std::vector<double>& u) {
    u.assign(nx, 0.0);
    u[0] = 1000.0; // Left end hot
    u[nx - 1] = 1000.0; // Right end hot

    int center_start = nx / 3;
    int center_end = 2 * nx / 3;
    for (int i = center_start; i <= center_end; ++i) {
        u[i] = 0.0; // Center cold
    }
}

// Function to update the temperature distribution
void updateTemperature(std::vector<double>& u) {
    std::vector<double> un = u;
    for (int i = 1; i < nx - 1; ++i) {
        u[i] = un[i] + alpha * dt / (dx * dx) * (un[i + 1] - 2 * un[i] + un[i - 1]);
    }
    u[0] = 1000.0; // Maintain left end hot
    u[nx - 1] = 1000.0; // Maintain right end hot
}

// Function to map temperature to color (blue to red)
SDL_Color mapToColor(double value) {
    double ratio = (value - min_temp) / (max_temp - min_temp);
    ratio = std::max(0.0, std::min(1.0, ratio)); // Clamp ratio between 0 and 1
    Uint8 r = static_cast<Uint8>(255 * ratio);
    Uint8 b = static_cast<Uint8>(255 * (1 - ratio));
    return { r, 0, b, 255 };
}

// Function to draw the temperature distribution
void draw(SDL_Renderer* renderer, const std::vector<double>& u) {
    int bar_width = window_width / nx;
    for (int i = 0; i < nx; ++i) {
        SDL_Color color = mapToColor(u[i]);
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_Rect bar = { i * bar_width, 0, bar_width, window_height };
        SDL_RenderFillRect(renderer, &bar);
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("1D Heat Equation Simulation",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          window_width, window_height,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    std::vector<double> u(nx);
    initializeTemperature(u);

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Update temperature distribution multiple times per frame for faster simulation
        for (int i = 0; i < 1; ++i) {
            updateTemperature(u);
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        draw(renderer, u);
        SDL_RenderPresent(renderer);

        SDL_Delay(5); // Delay to control the frame rate
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
