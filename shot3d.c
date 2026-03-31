#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <SDL.h>

#define TITLE "shot3d"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WINDOWPOS SDL_WINDOWPOS_UNDEFINED

#define WALL_COLOR 0, 255, 255, 255
#define MAP_WIDTH 20 
#define MAP_HEIGHT 20
#define TOPDOWN_PX_SIZE 8

#define PLAYER_COLOR 255, 255, 0, 255
#define NUM_RAYS 100
#define MAX_RAY_DIST 300
#define FOV 100
#define BRIGHT 3

// add uniform grids

const uint8_t map[MAP_WIDTH * MAP_HEIGHT] = {
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};

typedef struct {
	float x;
	float y;
	float dir;
	float speed;
} Entity;

uint8_t is_colliding_walls(float x, float y) {
	return 0;
}

int main() {
	printf("Hello World!");

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "SDL_INIT error!");
		return 1;
	}
	SDL_Window *window = SDL_CreateWindow(TITLE, WINDOWPOS, WINDOWPOS, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (!window) {
		fprintf(stderr, "window opening error!");
		return 1;
	}
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Event e;
	uint8_t running = 1;

	Entity player;
	player.x = 20.0;
	player.y = 20.0; 
	player.dir = 0;
	player.speed = 0;

	uint8_t show_lines = 0;
	uint64_t last_time, now_time;
	while (running == 1) {
		last_time = now_time;
		now_time = SDL_GetPerformanceCounter();
		double deltaTime = (double)((now_time - last_time) / (double)SDL_GetPerformanceFrequency());

		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) running = 0;

			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
					case SDLK_p: show_lines = !show_lines; break;
					case SDLK_ESCAPE: running = 0;   break;
				}
			}
		}

		const Uint8* keystate = SDL_GetKeyboardState(NULL);
		if (keystate[SDL_SCANCODE_W]) {
			// This code runs every frame as long as 'W' is held
			player.speed += 50.0f * deltaTime;
		}
		if (keystate[SDL_SCANCODE_S]) {
			player.speed -= 50.0f * deltaTime;
		}
		if (keystate[SDL_SCANCODE_LEFT]) player.dir -= 10.0f * deltaTime;
		if (keystate[SDL_SCANCODE_RIGHT]) player.dir += 10.0f * deltaTime;

		player.speed -= player.speed * 30.0f * deltaTime;

		player.x += cos(player.dir) * player.speed * deltaTime * 20.0f;
		player.y += sin(player.dir) * player.speed * deltaTime * 20.0f;

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// render map
		SDL_Rect mapstamp;
		mapstamp.w = TOPDOWN_PX_SIZE;
		mapstamp.h = TOPDOWN_PX_SIZE;
		// render 2d map
		for (uint16_t i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++) {
			mapstamp.x = (i % MAP_WIDTH) * TOPDOWN_PX_SIZE;
			mapstamp.y = (i / MAP_WIDTH) * TOPDOWN_PX_SIZE;

			if (map[i] == 1) {
				SDL_SetRenderDrawColor(renderer, WALL_COLOR);
			} else if (map[i] == 0) {
				continue;
			}
			SDL_RenderFillRect(renderer, &mapstamp);
		}

		// render the player
		SDL_Vertex vertices[3] = {
			{{player.x + cos(player.dir) * TOPDOWN_PX_SIZE, player.y + sin(player.dir) * TOPDOWN_PX_SIZE}, PLAYER_COLOR, {1, 1}},
			{{player.x + cos(player.dir + 1.6) * TOPDOWN_PX_SIZE, player.y + sin(player.dir + 1.6) * TOPDOWN_PX_SIZE}, PLAYER_COLOR, {1, 1}},
			{{player.x + cos(player.dir - 1.6) * TOPDOWN_PX_SIZE, player.y + sin(player.dir - 1.6) * TOPDOWN_PX_SIZE}, PLAYER_COLOR, {1, 1}},
		};
		SDL_RenderGeometry(renderer, 0, vertices, 3, 0, 0);

		// render raycast lines
		for (uint16_t x = 0; x < NUM_RAYS; x++) {
			float mult = ((float)FOV / 180) * 3.14f;
			float rayAngle = player.dir - (mult / 2) + (mult / NUM_RAYS) * x;
			float eyeX;
			float eyeY;
			float dist = 0;
			uint8_t hit_wall = 0;

			while (dist < MAX_RAY_DIST && !hit_wall) {
				eyeX = player.x + cos(rayAngle) * dist;
				eyeY = player.y + sin(rayAngle) * dist;
				dist = dist + 0.1;

				uint16_t gridIdx = (uint16_t)(eyeX / TOPDOWN_PX_SIZE) + (uint16_t)(eyeY / TOPDOWN_PX_SIZE) * MAP_WIDTH;
				if (map[gridIdx] == 1) {
					hit_wall = 1;
					if (show_lines) {
						SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
						SDL_RenderDrawLine(renderer, player.x, player.y, eyeX, eyeY);
					}

					float line_x = x * (float)(SCREEN_WIDTH / NUM_RAYS);
					float line_h = (float)SCREEN_HEIGHT / dist;
					uint8_t darkness = 255;
					// vignette effect
					if (fabs(SCREEN_WIDTH / 2 - line_x) / 3 <= 255) {
						darkness = fabs(SCREEN_WIDTH / 2 - line_x) / 3;
					} 
					if (darkness > BRIGHT * 2) {
						darkness -= BRIGHT * 2;
					}
					if (darkness <= 255 - dist) {
						darkness += dist;
					}

					SDL_SetRenderDrawColor(renderer, 255 - darkness, 0, 0, 255);
					SDL_Rect cast = { line_x, SCREEN_HEIGHT / 2 - line_h / 2, SCREEN_WIDTH / NUM_RAYS, line_h };
					SDL_RenderFillRect(renderer, &cast);
				}
			}
		}

		SDL_RenderPresent(renderer);
	}

	printf("Done!");

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}
