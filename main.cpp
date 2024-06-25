#include <SDL2/SDL.h>
#include <stdio.h>



int main(int argc, char** argv){

    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    auto window = SDL_CreateWindow("Physics Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Init(SDL_INIT_VIDEO);
    auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

    auto red_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 800, 600);
    auto blue_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 800, 600);

    SDL_SetRenderTarget(renderer, red_texture);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderTarget(renderer, blue_texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer); 

    SDL_SetRenderTarget(renderer, nullptr);

    SDL_RenderCopy(renderer, red_texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);
    SDL_RenderCopy(renderer, blue_texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);



    bool loopRunning = true;
    while (loopRunning) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) { 
            if (event.type == SDL_QUIT)
            loopRunning = false;
        }
    }



    SDL_DestroyWindow(window);
    
    return 0;
}