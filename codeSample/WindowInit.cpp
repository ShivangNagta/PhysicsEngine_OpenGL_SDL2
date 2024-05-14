#include <SDL2/SDL.h>
#include <stdio.h>



int main(int argc, char** argv){
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Physics Engine", 20, 20, 640,480, SDL_WINDOW_SHOWN);

    bool game_is_still_running = true;
    while (game_is_still_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {  // poll until all events are handled!
            // decide what to do with this event.
            if (event.type == SDL_QUIT) {
            game_is_still_running = false;
    }
    }

 
}
    return 0;
}