#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <numeric>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_sdlrenderer2.h>

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
    double tempRight = 200;
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


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);


    bool running = true;
    SDL_Event e;
    while(running){
        while(SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
                    ImGui_ImplSDL2_ProcessEvent(&e);
        }

        double avg = std::accumulate(Temp.begin(), Temp.end(), 0.0) / Temp.size();       
        string txt = to_string(avg);
        totalTime+= timeStep;
        string time = to_string(totalTime);
        
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        
        ImGui::Begin("Avg Temp");
        ImGui::SetWindowFontScale(2.0f);
        ImGui::Text("%s", txt.c_str());
        ImGui::End();

        ImGui::Begin("Time Elapsed");
        ImGui::SetWindowFontScale(2.0f);
        ImGui::Text("%s", time.c_str());
        ImGui::End();

        ImGui::Begin("TempLeft");

        ImGui::Text("200");
        ImGui::End();

        ImGui::Begin("TempRight");

        ImGui::Text("100");
        ImGui::End();

        ImGui::Render();

        
        SDL_SetRenderTarget(renderer, NULL);
        SDL_SetRenderDrawColor(renderer,174, 182, 210, 1);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
        UpdateTemp(Temp, timeStep, numberOfBars, barWidth, k, tempLeft, tempRight, dx);
        
        UpdateColor(renderer, Temp, numberOfBars, maxTemp, minTemp, barWidth, height, dx);

        

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}