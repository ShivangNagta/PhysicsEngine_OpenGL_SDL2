#include <SDL2/SDL.h>
#include <vector>
#include <cmath>

const int SCREEN_W = 800;
const int SCREEN_H = 600;
const int TEXTURE_H = 600;
const int TEXTURE_W = 800;

struct pos {
    int x;
    int y;
};

int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Physics Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Window could not be created! SDL_Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_Log("Renderer could not be created! SDL_Error: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, TEXTURE_W, TEXTURE_H);
    if (!texture) {
        SDL_Log("Texture could not be created! SDL_Error: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    std::vector<SDL_Point> pv;
    for (int i = 0; i < 100000; i++) {
        pv.emplace_back(SDL_Point{rand() % TEXTURE_W, rand() % TEXTURE_H});
    }

pos pos1 = {200, 200};
pos pos2 = {250, 200};
const float THETA = 0.1f;
float originalLength = sqrt(pow(pos2.x - pos1.x, 2) + pow(pos2.y - pos1.y, 2));

bool loopRunning = true;
SDL_Event e;
while (loopRunning) {
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) loopRunning = false;
    }

    // Translate pos2 relative to pos1
    int relX = pos2.x - pos1.x;
    int relY = pos2.y - pos1.y;

    // Apply rotation
    int newRelX = static_cast<int>(relX * cos(THETA) - relY * sin(THETA));
    int newRelY = static_cast<int>(relX * sin(THETA) + relY * cos(THETA));

    // Calculate the new length of the line after rotation
    float newLength = sqrt(pow(newRelX, 2) + pow(newRelY, 2));

    // Scale the new coordinates to preserve the original length
    newRelX = static_cast<int>((newRelX / newLength) * originalLength);
    newRelY = static_cast<int>((newRelY / newLength) * originalLength);

    // Update pos2 with the new coordinates
    pos2.x = pos1.x + newRelX;
    pos2.y = pos1.y + newRelY;

    // Render the scene
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black background
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White line
    SDL_RenderDrawLine(renderer, pos1.x, pos1.y, pos2.x, pos2.y);
    SDL_RenderPresent(renderer);
    SDL_Delay(50);

    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
