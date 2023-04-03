#include<stdio.h>
#include<stddef.h>
#include<string.h>
#include<math.h>
#include"mycomplex.h"
#include"my_sdl.h"
#include<SDL2/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HIGHT 600

int main()
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	//SDL_Texture *map = NULL;

	if(SDL_Init(SDL_INIT_VIDEO) != 0)
		SDL_ExitWithError("SDL_Init");
	window = SDL_CreateWindow("Boids",
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				WINDOW_WIDTH,
				WINDOW_HIGHT, 0);
	if(window == NULL)
		SDL_ExitWithError("SDL_CreateWindow");

	renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED);
	if(renderer == NULL)
		SDL_ExitWithError("SDL_CreateRenderer");
	/*
	int error;
	error = SDL_SetRenderDrawColor(renderer,0xB2,0x42,0xD4,0xFF);
	if(error < 0)
		SDL_ExitWithError("SDL_SetRenderDrawColor");
	error = SDL_RenderDrawLine(renderer,4,100,300,100);
	if(error < 0)
		SDL_ExitWithError("SDL_RenderDrawLine");
	SDL_RenderPresent(renderer);
	SDL_Delay(1000);
	*/
	main_loop(renderer);//, map);

	/*
	char prog_running = 1;
	int error;
	while(prog_running)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					prog_running = 0;
					break;
				default:
					break;
			}
			SDL_Point triangle[4];
			triangle[0].x = triangle[0].y = 100;
			triangle[1].x = triangle[1].y = 300;
			triangle[2].x = 200;
			triangle[2].y = 300;
			triangle[3] = triangle[0];
			error = SDL_RenderDrawLines(renderer,triangle,4);
			if(error < 0)
				SDL_ExitWithError("SDL_RenderDrawLines");
			SDL_RenderPresent(renderer);
			SDL_Delay(1000);
		}
	}*/

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

/*
SDL_Window *window; 
SDL_Renderer *render;
SDL_Texture *map; //map texture (my layer)
*/

//window = SDL_CreateWindow("Test window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
//render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//map = SDL_CreateTexture(render,  SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 640, 480); //Creating a texture

/*Map is a red background stored in map texture*/
/*SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
SDL_SetRenderTarget(render, map);
SDL_RenderClear(render);
SDL_SetRenderTarget(render, NULL);
*/
/*Seting the line color*/
//SDL_SetRenderDrawColor(render, 0, 255, 0, 255);

/*Coping the map texture to the render and drawing a green line on top of this*/
/*SDL_RenderCopy(render, map, NULL, NULL);
SDL_RenderDrawLine(render, 0, 0, 640, 480);
SDL_RenderPresent(render);
SDL_Delay(2000);
*/
/*Another line*//*
SDL_RenderCopy(render, map, NULL, NULL);
SDL_RenderDrawLine(render, 0, 480, 640, 0);
SDL_RenderPresent(render);
SDL_Delay(2000);

SDL_DestroyWindow(window);
SDL_DestroyRenderer(render);
SDL_Quit();
*/

