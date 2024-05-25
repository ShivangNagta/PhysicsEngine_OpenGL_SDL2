// Standard input-output stream library for basic IO operations
#include <iostream>
// SDL2 library for handling graphics and events
#include <SDL2/SDL.h>
// Math library for mathematical functions such as sin and cos
#include <math.h>
// Time library for seeding the random number generator
#include <time.h>
// String manipulation functions library for memory operations like memset
#include <cstring>

using namespace std;

// Screen Class for managing the display window and rendering operations
class Screen {
public:
    // Constants for screen dimensions
    const static int SCREEN_WIDTH = 1200;
    const static int SCREEN_HEIGHT = 800;

private:
    // Pointers for SDL window, renderer, texture, and buffer
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    SDL_Texture *m_texture;
    Uint32 *m_buffer1;
    Uint32 *m_buffer2;

public:
    // Constructor to initialize member pointers to NULL
    Screen() : m_window(NULL), m_renderer(NULL), m_texture(NULL), m_buffer1(NULL), m_buffer2(NULL) {}

    // Initialize SDL, create window, renderer, and texture, and allocate buffer
    bool init() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            return false;
        }

        m_window = SDL_CreateWindow("Particle Fire Explosion",
                                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                    SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        if (m_window == NULL) {
            SDL_Quit();
            return false;
        }

        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);
        m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888,
                                      SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

        if (m_renderer == NULL) {
            SDL_DestroyWindow(m_window);
            SDL_Quit();
            return false;
        }

        if (m_texture == NULL) {
            SDL_DestroyRenderer(m_renderer);
            SDL_DestroyWindow(m_window);
            SDL_Quit();
            return false;
        }

        m_buffer1 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
        m_buffer2 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];

        memset(m_buffer1, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
        memset(m_buffer2, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));

        SDL_RendererInfo info;
        SDL_GetRendererInfo(m_renderer, &info);
        printf("Renderer name: %s\n", info.name);
        if (info.flags & SDL_RENDERER_ACCELERATED) {
            printf("Using hardware acceleration\n");
        } else {
            printf("Using software rendering\n");
        }

        return true;
    }

    // Set a pixel in the buffer to a specific color
    void setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue) {
        if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
            return;
        }

        Uint32 color = 0;
        color += red;
        color <<= 8;
        color += green;
        color <<= 8;
        color += blue;
        color <<= 8;
        color += 0xFF;

        m_buffer1[(y * SCREEN_WIDTH) + x] = color;
    }

    // Update the screen with the current buffer content
    void update() {
        SDL_UpdateTexture(m_texture, NULL, m_buffer1, SCREEN_WIDTH * sizeof(Uint32));
        SDL_RenderClear(m_renderer);
        SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
        SDL_RenderPresent(m_renderer);
    }

    // Process events such as window close
    bool processEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return false;
            }
        }
        return true;
    }

    // Clean up resources
    void close() {
        delete[] m_buffer1;
        delete[] m_buffer2;
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyTexture(m_texture);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

	void boxBlur() {
		Uint32 *temp = m_buffer1;
		m_buffer1 = m_buffer2;
		m_buffer2 = temp;

		for (int y = 0; y< SCREEN_HEIGHT; y++){
			for (int x = 0; x< SCREEN_WIDTH; x++){

				int redTotal = 0;
				int greenTotal = 0;
				int blueTotal = 0;

				for (int row=-1; row<=1; row++){
					for (int col=-1; col<=1; col++){
						int currentX = x + col;
						int currentY = y + row;

						if (currentX >=0 && currentX < SCREEN_WIDTH && currentY>=0 && currentY <SCREEN_HEIGHT){
							Uint32 color = m_buffer2[SCREEN_WIDTH*currentY + currentX];
							Uint8 red = color >> 24;
							Uint8 green = color >> 16;
							Uint8 blue = color >> 8;

							redTotal += red;
						    greenTotal += green;
						    blueTotal += blue;
						}
					}
				}
				Uint8 red = redTotal / 9.5;
				Uint8 green = greenTotal / 9.5;
				Uint8 blue = blueTotal / 9.5 ;

				setPixel (x, y, red, green, blue);

			}
		}
	}
};

// Particle class for managing individual particles
struct Particle {
    double m_x;  // X position
    double m_y;  // Y position
    double m_speed;  // Speed of the particle
    double m_direction;  // Direction of the particle

    // Constructor to initialize position and direction
    Particle() : m_x(0), m_y(0) {
        m_direction = (2 * M_PI * rand()) / RAND_MAX;
        m_speed = (0.005 * rand()) / RAND_MAX;
    }

    // Virtual destructor
    virtual ~Particle() {}

    // Update the particle's position based on its speed and direction
    void update() {
        // m_direction += 0.002;
        double xspeed = m_speed * cos(m_direction);
        double yspeed = m_speed * sin(m_direction);

        m_x += xspeed;
        m_y += yspeed;

		// if (m_x < -1 || m_x > 1 || m_y < -1 || m_y > 1) {
        //     m_x = 0;
        //     m_y = 0;
        //     m_direction = (2 * M_PI * rand()) / RAND_MAX;
        //     m_speed = (0.005 * rand()) / RAND_MAX;
        // }
    }
};

// Swarm class for managing a collection of particles
class Swarm {
public:
    const static int NPARTICLES = 20000;  // Number of particles

private:
    Particle *m_pParticles;  // Array of particles

public:
    // Constructor to allocate particles
    Swarm() {
        m_pParticles = new Particle[NPARTICLES];
    }

    // Destructor to deallocate particles
    virtual ~Swarm() {
        delete[] m_pParticles;
    }

    // Update all particles in the swarm
    void update() {
        for (int i = 0; i < NPARTICLES; i++) {
            m_pParticles[i].update();
        }
    }

    // Get the particles array
    const Particle *const getParticles() {
        return m_pParticles;
    }
};

// Main function
int main(int argc, char *argv[]) {
    srand(time(NULL));  // Seed the random number generator

    Screen screen;  // Create screen object

    // Initialize screen and check for errors
    if (screen.init() == false) {
        cout << "Error initialising SDL." << endl;
        return 1;
    }

    Swarm swarm;  // Create swarm object

    // Main loop
    while (true) {
        int elapsed = SDL_GetTicks();  // Get the elapsed time

        // screen.clear(); // Clear the screen
        swarm.update();  // Update the swarm

        // Calculate colors based on elapsed time
        unsigned char green = (unsigned char)((1.5 + sin(elapsed * 0.0001)) * 96);
        unsigned char red   = (unsigned char)((1.5 + sin(elapsed * 0.0002)) * 96);
        unsigned char blue  = (unsigned char)((1.5 + sin(elapsed * 0.0003)) * 96);

        // Get the particles from the swarm
        const Particle *const pParticles = swarm.getParticles();

        // Draw the particles
        for (int i = 0; i < Swarm::NPARTICLES; i++) {
            Particle particle = pParticles[i];

            int x = (particle.m_x + 1) * Screen::SCREEN_WIDTH / 2;
            int y = particle.m_y * Screen::SCREEN_WIDTH / 2 + Screen::SCREEN_HEIGHT / 2;

            screen.setPixel(x, y, red, green, blue);
        }
		screen.boxBlur();

        screen.update();  // Update the screen

        // Process events and check if the user wants to quit
        if (screen.processEvents() == false) {
            break;
        }
    }

    screen.close();  // Clean up and close the screen

    return 0;
}
