#include <err.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "paint.h"

#define PI 3.14159

//SDL_Surface* surface;
//SDL_Rect target = {0, 0, 20, 20};

// Initial width and height of the window.
//const int INIT_WIDTH = 640;
//const int INIT_HEIGHT = 400;

// Boolean paint
//int PAINT = 0;

//int DRAW = 0;
//int ERASE = 255;

int drawrect(SDL_Renderer* renderer, int color,
        int mouse_x, int mouse_y, int radius, SDL_Rect* target)
{
    // Sets the color for drawing operations to black.
    SDL_SetRenderDrawColor(renderer, color, color, color, 255);

    SDL_Rect rect = {mouse_x-(radius/2), mouse_y-(radius/2), radius, radius};

    /*Uint32* pixels = surface->pixels;
    SDL_RenderReadPixels(renderer, &rect, SDL_PIXELFORMAT_RGBA8888,
            pixels, surface->pitch);
    Uint8 r, g, b;
    SDL_GetRGB(pixels[0], surface->format, &r, &g, &b);
    */

    if (!SDL_HasIntersection(&rect, target))
    {
        SDL_RenderFillRect(renderer, &rect);
    	SDL_RenderPresent(renderer);
    	return 1;
    }

    // Updates the display.
    SDL_RenderPresent(renderer);
    return 0;
}

void drawtarget(SDL_Renderer* renderer, SDL_Rect* target, const int INIT_WIDTH,
	const int INIT_HEIGHT)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, target);

    srand(time(NULL));
    target->x = rand() % (INIT_WIDTH-50) + 20;
    target->y = rand() % (INIT_HEIGHT-50) + 20;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, target);

    // Updates the display.
    SDL_RenderPresent(renderer);
}


void* worker(void* arg)
{
	struct draw_rect* draw = arg;
	drawrect(draw->renderer,
		draw->color,
		draw->mouse_x,
		draw->mouse_y,
		draw->radius,
		draw->target);

	return NULL;
}
