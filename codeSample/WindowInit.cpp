#include <SDL2/SDL.h>
#include <stdio.h>



int main(int argc, char** argv){

    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    SDL_Window* window = nullptr;

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Physics Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer * renderer = SDL_CreateRenderer(window,
                       -1, SDL_RENDERER_PRESENTVSYNC);




    bool gameRunning = true;
    while (gameRunning) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) { 
            if (event.type == SDL_QUIT)
            gameRunning = false;
        }
    }





    SDL_DestroyWindow(window);
    
    return 0;
}