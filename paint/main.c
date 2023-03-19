#include <err.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>

#include <pthread.h>
#include "paint.h"

int BOOL = 1;

SDL_Renderer* renderer;

const int INIT_WIDTH = 1200;
const int INIT_HEIGHT = 800;

// Boolean paint
int PAINT = 0;

int DRAW = 0;
int ERASE = 255;

// Event loop that calls the relevant event handler.
//
// renderer: Renderer to draw on.
void* event_loop()//SDL_Renderer* renderer, const int INIT_WIDTH, const int INIT_HEIGHT)
{
    SDL_Rect target = {0, 0, 20, 20};

    // Clears the renderer (sets the background to white).
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    // Draw target
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    drawtarget(renderer, &target, INIT_WIDTH, INIT_HEIGHT);

    // Creates a variable to get the events.
    SDL_Event event;

    while (1)
    {
        // Waits for an event.
        SDL_WaitEventTimeout(&event, 0);

        switch (event.type)
        {
            // If the "quit" button is pushed, ends the event loop.
            case SDL_QUIT:
                return NULL;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    drawrect(renderer, DRAW, event.motion.x, event.motion.y,
                            10, &target);
                    PAINT = 1;
                }
                else if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    drawrect(renderer, ERASE, event.motion.x, event.motion.y,
                            20, &target);
                    PAINT = -1;
                }
                break;

            case SDL_MOUSEBUTTONUP:
                PAINT = 0;
                break;
            // If the mouse is moving, updates the position of the cursor.
            case SDL_MOUSEMOTION:
                if (PAINT == 1)
                    drawrect(renderer, DRAW, event.motion.x, event.motion.y,
                            10, &target);
                else if (PAINT == -1)
                    drawrect(renderer, ERASE, event.motion.x, event.motion.y,
                            20, &target);
                break;

            case SDL_KEYDOWN:
                drawtarget(renderer, &target, INIT_WIDTH, INIT_HEIGHT);
                break;
            default:
                break;
        }
    }
    return NULL;
}

int main()
{
    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a window.
    SDL_Window* window = SDL_CreateWindow("RG2A Simulation", 0, 0, INIT_WIDTH, INIT_HEIGHT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a renderer.
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    //surface = SDL_GetWindowSurface(window);

    if (BOOL)
    {
        pthread_t thr;
        int e = pthread_create(&thr, NULL, event_loop, NULL);
        if (e != 0)
            err(EXIT_FAILURE, "pthread create()");
        pthread_join(thr, NULL);
    }
    else
    // Dispatches the events.
        event_loop();//renderer, INIT_WIDTH, INIT_HEIGHT);

    //SDL_FreeSurface(surface);

    // Destroys the objects.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

