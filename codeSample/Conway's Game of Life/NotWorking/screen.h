#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <cstdlib>
#include <algorithm>

class Screen {
    SDL_Event e;
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<SDL_FPoint> points;

    public:
    Screen(){
        SDL_Init(SDL_INIT_VIDEO);
        SDL_CreateWindowAndRenderer(640*2, 480*2, 0, &window, &renderer);
        SDL_RenderSetScale(renderer, 2, 2);
    };


    void setPixels(float x, float y)
    {
        points.emplace_back(SDL_FPoint{x,y});
    };


    void update(){
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (auto& point : points){
            SDL_RenderDrawPointF(renderer, point.x, point.y);
        }
        SDL_RenderPresent(renderer);
    }


    void clearPixels(){
        points.clear();
    }

    void input(){
        while(SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                SDL_Quit();
                exit(0);
            }
        }
    }
};