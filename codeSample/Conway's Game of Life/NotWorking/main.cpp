#include "screen.h"
#include <array>


const int SCREEN_W = 640;
const int SCREEN_H = 480;


bool isAlive(std::array<std::array<int, SCREEN_H>, SCREEN_W>& game, const int x, const int y){
    
    int alive = 0;
    if (x > 0 && game[x-1][y] == 1) alive++;
    if (x < SCREEN_W && game[x+1][y] == 1) alive++;
    if (y> 0 && game[x][y-1] == 1) alive++;
    if (y < SCREEN_H && game[x][y+1] == 1) alive++;

    if (y > 0 && x > 0 && game[x-1][y-1] == 1) alive++;
    if (y > 0 && x < SCREEN_W && game[x+1][y-1] == 1) alive++;
    if (y <  SCREEN_H && x > 0 && game[x-1][y+1] == 1) alive++;
    if (y < SCREEN_H && x < SCREEN_W && game[x+1][y+1] == 1) alive++;

    if (game[x][y] == 1 && alive < 2) return false;
    if (game[x][y] == 1 && (alive == 2 || alive == 3)) return true;
    if (alive > 3) return false;
    if (game[x][y] == 0 && alive == 3) return true;

    return false;
}

int main (int argc, char *argv[]){

    Screen screen;

    std::array<std::array<int, SCREEN_H>,SCREEN_W> display {};
    std::array<std::array<int, SCREEN_H>,SCREEN_W> swap {};

    for(auto& row : display) std::generate(row.begin(), row.end(), []() {return rand()%10 == 0 ? 1 : 0 ;});

    while (true){
        
        for (int x = 0; x < SCREEN_W; ++x){
            for (int y = 0; y < SCREEN_H; ++y){
                swap[x][y] = isAlive(display, x, y) ? 1 : 0;
            }
        }
        

        for (int x = 0; x < SCREEN_W; ++x) {
            for (int y = 0; y < SCREEN_H; ++y) {
                if (swap[x][y]) {
                    screen.setPixels(x, y);
                }
            }
        }

        std::copy(swap.begin(), swap.end(), display.begin());

        screen.update();
        SDL_Delay(20);
        screen.input();
        screen.clearPixels();
        
        
    }
}

