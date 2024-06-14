#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>
#include <iostream>

const int SCREEN_W = 1200;
const int SCREEN_H = 800;
const int GRID_W = 1200;  // Grid width
const int GRID_H = 800;  // Grid height
const int CELL_SIZE = 48; // Size of each cell in pixels

bool isAlive(const std::vector<std::vector<int>>& game, int x, int y) {
    int alive = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;
            int nx = x + i;
            int ny = y + j;
            if (nx >= 0 && nx < GRID_W && ny >= 0 && ny < GRID_H) {
                if (game[nx][ny] == 1) alive++;
            }
        }
    }
    if (game[x][y] == 1) {
        return alive == 2 || alive == 3;
    }
    return alive == 3;
}

int main(int argc, char** argv) {
    std::vector<std::vector<int>> display(SCREEN_W, std::vector<int>(SCREEN_H, 0));
    std::vector<std::vector<int>> swap(SCREEN_W, std::vector<int>(SCREEN_H, 0));

    // Initialize display with random values
    for (auto& row : display) {
        std::generate(row.begin(), row.end(), []() { return rand() % 2 == 0 ? 1 : 0; });
    }

    SDL_Rect source{0, 0, SCREEN_W / CELL_SIZE, SCREEN_H / CELL_SIZE};
    SDL_Rect dest{10, 10, SCREEN_W-20, SCREEN_H-20};

    SDL_Init(SDL_INIT_VIDEO);
    auto window = SDL_CreateWindow("Conway's Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
    SDL_Event e;

    auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    auto texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, GRID_W, GRID_H);

    std::vector<SDL_Point> points;

    bool loopRunning = true;
    while (loopRunning) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) loopRunning = false;
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP: source.y -= 5; break;
                    case SDLK_DOWN: source.y += 5; break;
                    case SDLK_LEFT: source.x -= 5; break;
                    case SDLK_RIGHT: source.x += 5; break;
                    case SDLK_1: source.w *= 2; source.h *= 2; break;
                    case SDLK_2: source.w /= 2; source.h /= 2; break;
                }
            }
        }

        SDL_SetRenderTarget(renderer, texture);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set background color to black
        SDL_RenderClear(renderer);

        // Update the game state
        for (int x = 0; x < SCREEN_W; ++x) {
            for (int y = 0; y < SCREEN_H; ++y) {
                swap[x][y] = isAlive(display, x, y) ? 1 : 0;
            }
        }

        // Prepare points to render
        points.clear();
        for (int x = 0; x < SCREEN_W; ++x) {
            for (int y = 0; y < SCREEN_H; ++y) {
                if (swap[x][y]) {
                    points.push_back(SDL_Point{x, y});
                }
            }
        }

        // Swap buffers
        std::swap(display, swap);

        // Render cells with dark theme
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // Dark gray for dead cells
        for (const auto& point : points) {
            if (display[point.x][point.y] == 1) {
                SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255); // Green for alive cells
            } else {
                SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // Dark gray for dead cells
            }
            SDL_RenderDrawPoint(renderer, point.x, point.y);
        }

        // Reset render target
        SDL_SetRenderTarget(renderer, nullptr);
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        // Copy texture to renderer and present
        SDL_RenderCopy(renderer, texture, &source, &dest);
        SDL_RenderPresent(renderer);

        // Delay for smooth animation
        SDL_Delay(10);
    }

    // Cleanup resources
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
