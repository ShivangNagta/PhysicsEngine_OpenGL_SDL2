#include <iostream>
#include <SDL2/SDL.h>
#include <math.h>
#include <time.h>
#include <cstring>

using namespace std;

// Screen Class
class Screen
{

public:
	const static int SCREEN_WIDTH = 1200;
	const static int SCREEN_HEIGHT = 800;

private:
	SDL_Window *m_window;
	SDL_Renderer *m_renderer;
	SDL_Texture *m_texture;
	Uint32 *m_buffer;

public:
	Screen() : m_window(NULL), m_renderer(NULL), m_texture(NULL), m_buffer(NULL)
	{
	}

	bool init()
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			return false;
		}

		m_window = SDL_CreateWindow("Particle Fire Explosion",
									SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
									SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (m_window == NULL)
		{
			SDL_Quit();
			return false;
		}

		m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);
		m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888,
									  SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

		if (m_renderer == NULL)
		{
			SDL_DestroyWindow(m_window);
			SDL_Quit();
			return false;
		}

		if (m_texture == NULL)
		{
			SDL_DestroyRenderer(m_renderer);
			SDL_DestroyWindow(m_window);
			SDL_Quit();
			return false;
		}

		m_buffer = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];

		memset(m_buffer, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));

		SDL_RendererInfo info;
		SDL_GetRendererInfo(m_renderer, &info);
		printf("Renderer name: %s\n", info.name);
		if (info.flags & SDL_RENDERER_ACCELERATED)
		{
			printf("Using hardware acceleration\n");
		}
		else
		{
			printf("Using software rendering\n");
		}

		return true;
	}

	void clear()
	{
		memset(m_buffer, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
	}

	void setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue)
	{

		if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
		{
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

		m_buffer[(y * SCREEN_WIDTH) + x] = color;
	}

	void update()
	{
		SDL_UpdateTexture(m_texture, NULL, m_buffer, SCREEN_WIDTH * sizeof(Uint32));
		SDL_RenderClear(m_renderer);
		SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
		SDL_RenderPresent(m_renderer);
	}

	bool processEvents()
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				return false;
			}
		}
		return true;
	}

	void close()
	{
		delete[] m_buffer;
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyTexture(m_texture);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
	}
};

// Particle class

struct Particle
{
	double m_x;
	double m_y;

	double m_speed;
	double m_direction;

public:
	Particle() : m_x(0), m_y(0)
	{

		m_direction = (2 * M_PI * rand()) / RAND_MAX;
		m_speed = (0.005 * rand()) / RAND_MAX;
	}

	virtual ~Particle()
	{
	}

	void update()
	{

		m_direction += 0.002;
		double xspeed = m_speed * cos(m_direction);
		double yspeed = m_speed * sin(m_direction);

		m_x += xspeed;
		m_y += yspeed;
	};
};

// Swarm Class

class Swarm
{
public:
	const static int NPARTICLES = 400000;

private:
	Particle *m_pParticles;

public:
	Swarm()
	{
		m_pParticles = new Particle[NPARTICLES];
	}

	virtual ~Swarm()
	{
		delete[] m_pParticles;
	}

	void update()
	{
		for (int i = 0; i < NPARTICLES; i++)
		{
			m_pParticles[i].update();
		}
	}

	const Particle *const getParticles() { return m_pParticles; };
};



int main(int argc, char *argv[])
{

	srand(time(NULL));

	Screen screen;

	if (screen.init() == false)
	{
		cout << "Error initialising SDL." << endl;
	}

	Swarm swarm;

	while (true)
	{

		int elapsed = SDL_GetTicks();

		screen.clear();
		swarm.update();

		unsigned char green = (unsigned char)((1.5 + sin(elapsed * 0.0001)) * 96);
		unsigned char red   = (unsigned char)((1.5 + sin(elapsed * 0.0002)) * 96);
		unsigned char blue  = (unsigned char)((1.5 + sin(elapsed * 0.0003)) * 96);

		const Particle *const pParticles = swarm.getParticles();

		for (int i = 0; i < swarm.NPARTICLES; i++)
		{
			Particle particle = pParticles[i];

			int x = (particle.m_x + 1) * screen.SCREEN_WIDTH / 2;
			int y = particle.m_y * screen.SCREEN_WIDTH / 2 + screen.SCREEN_HEIGHT / 2;

			screen.setPixel(x, y, red, green, blue);
		}

		screen.update();

	
		if (screen.processEvents() == false)
		{
			break;
		}
	}

	screen.close();

	return 0;
}