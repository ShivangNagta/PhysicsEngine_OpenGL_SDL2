#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>


const int SCREEN_W = 800;
const int SCREEN_H = 600;
const int TEXTURE_H = 600;
const int TEXTURE_W = 800;


int main(int argc, char** argv){

    SDL_Rect source{0, 0, SCREEN_W/32, SCREEN_H/32};
    SDL_Rect dest{10, 10, SCREEN_W - 20, SCREEN_H - 20};

    SDL_Init(SDL_INIT_VIDEO);
    auto window = SDL_CreateWindow("Physics Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
    SDL_Event e;

    auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    auto texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, TEXTURE_W, TEXTURE_H);

    std::vector<SDL_Point> pv;
    for (int i = 0; i < 100000; i++) {pv.emplace_back(SDL_Point{rand() % TEXTURE_W, rand() % TEXTURE_H});}

    bool loopRunning = true;
    while (loopRunning) {
        
        while (SDL_PollEvent(&e)) { 
            if (e.type == SDL_QUIT) loopRunning = false;
            if (e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym)
                {
                case SDLK_UP: source.y -= 3; break;
                case SDLK_DOWN: source.y += 3; break;
                case SDLK_LEFT: source.x -= 3; break;
                case SDLK_RIGHT: source.x += 3; break;
                case SDLK_1: source.w *=2; source.h *=2; break;
                case SDLK_2: source.w /=2; source.h /=2; break;
                }
            }
            
        }
        SDL_SetRenderTarget(renderer, texture);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer); 

        std::for_each(pv.begin(), pv.end(), [](auto& item){
            item.x += rand() % 3 -1;
            item.y += rand() % 3 -1;
        });

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawPoints(renderer, pv.data(), pv.size());

        SDL_SetRenderTarget(renderer, nullptr);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, &source, &dest);
        SDL_RenderPresent(renderer);
        SDL_Delay(50);
    }



    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}