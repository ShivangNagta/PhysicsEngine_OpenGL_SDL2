#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <numeric>

using namespace std;


int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 400;

SDL_Color ColorMap(double temp, double maxTemp, double minTemp){
    double ratio = (temp - minTemp) / (maxTemp - minTemp);
    Uint8 b = (Uint8) ((1 - ratio) * 255);
    Uint8 r = 255 - b;
    return {r, 0, b, 255};
}

// void Draw(SDL_Renderer* renderer, double numberOfBars, double height, int barWidth, double initialTemp, double minTemp, double maxTemp){
//     SDL_Color initialColor = ColorMap(initialTemp, maxTemp, minTemp);
//     SDL_SetRenderDrawColor(renderer, initialColor.r, initialColor.g, initialColor.b, initialColor.a);
//     for (int i = 0; i< numberOfBars; ++i){
//         SDL_Rect rect = { (i+1)* barWidth + int(WINDOW_WIDTH / 8), WINDOW_HEIGHT / 4, barWidth, (int)height };
//         SDL_RenderFillRect(renderer, &rect);
//     }
// }


void UpdateTemp(vector<double> &Temp, double dt, int numberOfBars, int barWidth, double k, double tempLeft, double tempRight, double dx){
    vector<double> newTemp = Temp;
    for (int i = 1; i< numberOfBars - 1; ++i){
        newTemp[i] = Temp[i] + (dt * k * (Temp[i+1] - 2 * Temp[i] + Temp[i-1]) / (dx * dx));
    }
    newTemp[0] = tempLeft;
    newTemp[numberOfBars - 1] = tempRight;
    Temp = newTemp;
}


void UpdateColor(SDL_Renderer* renderer,const vector<double> &Temp, int numberOfBars, double maxTemp, double minTemp, int barWidth, double height, double dx){
    for (int i = 0; i< numberOfBars ; ++i){
        SDL_Color color = ColorMap(Temp[i], maxTemp, minTemp);
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_Rect rect = { (i)* barWidth + int(WINDOW_WIDTH / 8), WINDOW_HEIGHT / 2, barWidth, (int)height };
        SDL_RenderFillRect(renderer, &rect);
    }
    SDL_RenderPresent(renderer);
}


int main(int argc, char** argv){

    double k = 0.89;
    double length = 10;
    double tempLeft = 100;
    double tempRight = 100;
    double totalTime = 0;
    double timeStep = 0.0001;
    double dx = 0.1;
    int barWidth = 6;
    int numberOfBars = length / dx;
    double initialTemp = 0;
    int height = WINDOW_HEIGHT / 4;

    double maxTemp = tempRight;
    double minTemp = 0;

    vector<double> Temp(numberOfBars, initialTemp);
    Temp[0] = tempLeft;
    Temp[numberOfBars-1] = tempRight;

    SDL_Window* window = SDL_CreateWindow("1-D Heat Equation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    bool running = true;
    SDL_Event e;
    while(running){
        while(SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
        }
        SDL_SetRenderTarget(renderer, NULL);
        SDL_SetRenderDrawColor(renderer, 222, 161, 148, 0.8);
        SDL_RenderClear(renderer);
        UpdateTemp(Temp, timeStep, numberOfBars, barWidth, k, tempLeft, tempRight, dx);
        totalTime+= timeStep;
        UpdateColor(renderer, Temp, numberOfBars, maxTemp, minTemp, barWidth, height, dx);
        double avg = std::accumulate(Temp.begin(), Temp.end(), 0.0) / Temp.size();
        cout << avg << endl;
        

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}