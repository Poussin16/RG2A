#include<stdio.h>
#include<stddef.h>
#include<string.h>
#include<math.h>
#include"mycomplex.h"
#include"boid.h"
#include<SDL2/SDL.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

void* run(void* arg)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if(SDL_Init(SDL_INIT_VIDEO) != 0)
        SDL_ExitWithError("SDL_Init");
    window = SDL_CreateWindow("Boids",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT, 0);
    if(window == NULL)
        SDL_ExitWithError("SDL_CreateWindow");

    renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED);
    if(renderer == NULL)
        SDL_ExitWithError("SDL_CreateRenderer");

    main_loop(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return arg;
}

/*int main()
{
    run();
    return 0;
}*/
