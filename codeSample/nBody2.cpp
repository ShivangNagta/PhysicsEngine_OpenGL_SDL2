#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <cmath>
#include <ctime>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float G = 1.0f; // Gravitational constant
const int NUM_PARTICLES = 1000; // Number of particles
const float SOFTENING = 0.1f; // Softening length

struct Particle {
    float x, y, z;
    float vx, vy, vz;
    float mass;

    Particle(float x, float y, float z, float mass) : x(x), y(y), z(z), mass(mass) {
        vx = 0.0f;
        vy = 0.0f;
        vz = 0.0f;
        // vx = (float)(rand() % 10000 - 5000) / 10000.0f;
        // vy = (float)(rand() % 10000 - 5000) / 10000.0f;
        // vz = (float)(rand() % 10000 - 5000) / 10000.0f;
    }
};

float getAcceleration(const std::vector<Particle>& particles, int i, int j, float G, float softening) {
    float dx = particles[i].x - particles[j].x;
    float dy = particles[i].y - particles[j].y;
    float dz = particles[i].z - particles[j].z;
    float distance = sqrtf(dx * dx + dy * dy + dz * dz + softening * softening);
    float inv_r3 = powf(distance * distance, -1.5f);
    float force = G * particles[i].mass * particles[j].mass * inv_r3;
    return force;
}

void updateParticles(std::vector<Particle>& particles, float dt, float G, float softening) {
    int N = particles.size();
    std::vector<float> ax(N, 0.0f);
    std::vector<float> ay(N, 0.0f);
    std::vector<float> az(N, 0.0f);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (i == j) continue;
            float force = getAcceleration(particles, i, j, G, softening);
            ax[i] += force * particles[j].mass;
            ay[i] += force * particles[j].mass;
            az[i] += force * particles[j].mass;
        }
    }

    for (int i = 0; i < N; ++i) {
        particles[i].vx += ax[i] * dt;
        particles[i].vy += ay[i] * dt;
        particles[i].vz += az[i] * dt;
    }
}

int main(int argc, char* argv[]) {
    srand(time(nullptr));

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("N-Body Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    std::vector<Particle> particles;
    for (int i = 0; i < NUM_PARTICLES; ++i) {
        // Generating normally distributed random numbers for particle positions
        float x, y;
        do {
            float u1 = static_cast<float>(rand()) / RAND_MAX;
            float u2 = static_cast<float>(rand()) / RAND_MAX;
            x = sqrtf(-2.0f * log(u1)) * cosf(2 * M_PI * u2);
            y = sqrtf(-2.0f * log(u1)) * sinf(2 * M_PI * u2);
            // Scale and translate to fit screen
            x = x * SCREEN_WIDTH / 20.0f + SCREEN_WIDTH / 2.0f;
            y = y * SCREEN_HEIGHT / 20.0f + SCREEN_HEIGHT / 3.0f;
            // float x = rand()% SCREEN_WIDTH;
            // float y = rand()% SCREEN_HEIGHT;

            std::cout << x << "\n";
        } while (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT);
        
        float z = 0.0f; // Assuming 2D simulation
        float mass = 1.0f;
        particles.push_back(Particle(x, y, z, mass));
    }

    float dt = 1.0f; // Timestep
    float t = 0.0f; // Current time
    float tEnd = 10000.0f; // End time

    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Update particles
        updateParticles(particles, dt, G, SOFTENING);

        for (Particle& particle : particles) {
            particle.x += particle.vx * dt;
            particle.y += particle.vy * dt;
            particle.z += particle.vz * dt;

            // Wrap around screen edges
            // if (particle.x < 0) particle.x += SCREEN_WIDTH;
            // if (particle.x >= SCREEN_WIDTH) particle.x -= SCREEN_WIDTH;
            // if (particle.y < 0) particle.y += SCREEN_HEIGHT;
            // if (particle.y >= SCREEN_HEIGHT) particle.y -= SCREEN_HEIGHT;
        }

        // Draw particles
        for (const Particle& particle : particles) {
            SDL_Rect rect = {static_cast<int>(particle.x - 2), static_cast<int>(particle.y - 2), 1, 1};
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &rect);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(1); // Control simulation speed

        t += dt;
        if (t >= tEnd) {
            quit = true;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
