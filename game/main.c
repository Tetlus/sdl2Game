#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <string.h>

#define WIDTH 1000
#define HEIGHT 750
#define scaleSize 50

#define mapHeight 50
#define mapWidth 50
#define render 6

#define FPS 60

int health = 100, i, j;

int map[mapHeight][mapWidth] = {};



// loadmap function
void loadMap() {

	char mapName[25] = "map.txt";

	printf("Enter the name of the map\n");
	scanf("%s", &mapName);
	
	
	FILE *file = fopen(mapName, "r");

	for (i = 0; i < mapWidth; i++) {
		for (j = 0; j < mapHeight; j++) {
			fscanf(file, "%d", &map[i][j]);
		}
	}
	fclose(file);
}

// main function
int main(int argc, char* argv[]) {

	loadMap();	// call loadmap function

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("Error initalizing SDL: %s\n", SDL_GetError());
		return 0;
	}

	// create a window
	SDL_Window* wind = SDL_CreateWindow("game",
					SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED,
					WIDTH, HEIGHT, 
					SDL_WINDOW_RESIZABLE);
	
	if (!wind) {
		printf("Error while creating window: %s\n", SDL_GetError());
		SDL_Quit();
		return 0;
	}


	// create a renderer
	Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	SDL_Renderer* rend = SDL_CreateRenderer(wind, -1, render_flags);

	if (!rend) {
		printf("Error creating renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(wind);
		SDL_Quit();
		return 0;
	}
	
	// set logical resolution
	SDL_RenderSetLogicalSize(rend, 1000, 650);

	// create surfaces and textures
	SDL_Surface* surface = SDL_LoadBMP("./images/player.bmp");
	SDL_Texture* player = SDL_CreateTextureFromSurface(rend, surface);
	SDL_FreeSurface(surface);	// free the surface
	SDL_Surface* surface1 = SDL_LoadBMP("./images/grass.bmp");
	SDL_Texture* grass = SDL_CreateTextureFromSurface(rend, surface1);
	SDL_FreeSurface(surface1);
	SDL_Surface* surface2 = SDL_LoadBMP("./images/water.bmp");
	SDL_Texture* water = SDL_CreateTextureFromSurface(rend, surface2);
	SDL_FreeSurface(surface2);
	SDL_Surface* surface3 = SDL_LoadBMP("./images/tree.bmp");
	SDL_Texture* tree = SDL_CreateTextureFromSurface(rend, surface3);
	SDL_FreeSurface(surface3);
	
	// mainloop
	bool running = true, w_pressed = false, a_pressed = false, s_pressed = false, d_pressed = false;
	int playerX = 0, playerY = 0;
	
	SDL_Rect rect = {(int) 0, (int) 0, scaleSize, scaleSize};

	SDL_Event event;


	while (running) {
		// process events
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.scancode) {
						case SDL_SCANCODE_W:
							w_pressed = true;
							break;
						case SDL_SCANCODE_A:
							a_pressed = true;
							break;
						case SDL_SCANCODE_S:
							s_pressed = true;
							break;
						case SDL_SCANCODE_D:
							d_pressed = true;
							break;
						case SDL_SCANCODE_Q:
							health--;
							break;
						case SDL_SCANCODE_ESCAPE:
							running = false;
							break;
						case SDL_SCANCODE_L:
							loadMap();	// call loadmap function
							break;
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					switch (event.key.keysym.scancode) {
						case SDL_SCANCODE_W:
							w_pressed = false;
							break;
						case SDL_SCANCODE_A:
							a_pressed = false;
							break;
						case SDL_SCANCODE_S:
							s_pressed = false;
							break;
						case SDL_SCANCODE_D:
							d_pressed = false;
							break;
					}
					break;
				default:
					break;
			}
		}

		// handle movement

		if (w_pressed == true && playerY != 0) {
			if (map[playerY - 1][playerX] < 5)
				playerY -= 1;
			printf("%dX %dY\n", playerX, playerY);
		}
		if (a_pressed == true && playerX != 0) {
			if (map[playerY][playerX -1] < 5)
				playerX -= 1;
			printf("%dX %dY\n", playerX, playerY);
		}
		if (s_pressed == true && playerY != mapHeight-1) {
			if (map[playerY + 1][playerX] < 5)
				playerY += 1;
			printf("%dX %dY\n", playerX, playerY);
		}
		if (d_pressed == true && playerX != mapWidth-1) {
			if (map[playerY][playerX + 1] < 5)
				playerX += 1;
			printf("%dX %dY\n", playerX, playerY);
		}
		
		// clear and draw background

		SDL_SetRenderDrawColor(rend, 100, 100, 100, 255);
		SDL_RenderClear(rend);


		// draw map and player

		// draw the map AROUND the player

		int i2 = 0;
		int j2 = 0;

		for (i = playerY - (render); i < playerY + (render+1); i++) {
			j2 = 0;
			i2++;
			for (j = playerX - (render); j < playerX + (render+1); j++) {
				j2++;

				// draw the boundry
				

				// this puts the rectangle position on the screen
				rect.y = (i2 -1) * scaleSize;
				rect.x = (j2 -1) * scaleSize;

				if (i < 0 || i > mapHeight -1 || j < 0 || j > mapWidth -1 || map[i][j] == 2) {
					SDL_RenderCopy(rend,water,NULL,&rect);
				}

				else if (map[i][j] == 1) {
					SDL_SetRenderDrawColor(rend, 155, 155, 0, 255);
					SDL_RenderFillRect(rend, &rect);
				}
				else if (map[i][j] == 5) {
					SDL_RenderCopy(rend,water,NULL,&rect);
				}
				else if (map[i][j] == 6) {
					SDL_RenderCopy(rend,tree,NULL,&rect);
				}
				else {
					SDL_RenderCopy(rend,grass,NULL,&rect);
				}
				if (i == playerY && j == playerX) {
					SDL_RenderCopy(rend,player,NULL,&rect);
				}
			}
		}

		// draw the hud

		SDL_Rect Healthrect = {(int) 0, (int) 0, health*2, scaleSize};

		Healthrect.y = scaleSize;
		Healthrect.x = 750;

		SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
		SDL_RenderFillRect(rend, &Healthrect);


		SDL_RenderPresent(rend);
		SDL_Delay(6000/FPS);
	}
	// clear resources
	SDL_DestroyTexture(player);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(wind);
	SDL_Quit();
	return 0;


}
