#include <err.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>

#define PI 3.14159

SDL_Surface* surface;
SDL_Rect target = {0, 0, 20, 20};

// Initial width and height of the window.
const int INIT_WIDTH = 640;
const int INIT_HEIGHT = 400;

// Boolean paint
int PAINT = 0;

int DRAW = 0;
int ERASE = 255;

void drawrect(SDL_Renderer* renderer, int color,
        int mouse_x, int mouse_y, int radius)
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

    if (!SDL_HasIntersection(&rect, &target))
    {
        SDL_RenderFillRect(renderer, &rect);
    }

    // Updates the display.
    SDL_RenderPresent(renderer);
}

void drawtarget(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &target);

    target.x = rand() % (INIT_WIDTH-50) + 20;
    target.y = rand() % (INIT_HEIGHT-50) + 20;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &target);

    // Updates the display.
    SDL_RenderPresent(renderer);
}

// Event loop that calls the relevant event handler.
//
// renderer: Renderer to draw on.
void event_loop(SDL_Renderer* renderer)//, SDL_Surface* surface)
{
    // Clears the renderer (sets the background to white).
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    // Draw target
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    drawtarget(renderer);

    // Creates a variable to get the events.
    SDL_Event event;

    while (1)
    {
        // Waits for an event.
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            // If the "quit" button is pushed, ends the event loop.
            case SDL_QUIT:
                return;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    drawrect(renderer, DRAW, event.motion.x, event.motion.y, 10);
                    PAINT = 1;
                }
                else if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    drawrect(renderer, ERASE, event.motion.x, event.motion.y, 20);
                    PAINT = -1;
                }
                break;

            case SDL_MOUSEBUTTONUP:
                PAINT = 0;
                break;
            // If the mouse is moving, updates the position of the cursor.
            case SDL_MOUSEMOTION:
                if (PAINT == 1)
                    drawrect(renderer, DRAW, event.motion.x, event.motion.y, 10);
                else if (PAINT == -1)
                    drawrect(renderer, ERASE, event.motion.x, event.motion.y, 20);
                break;

            case SDL_KEYDOWN:
                drawtarget(renderer);
                break;
        }
    }
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
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    surface = SDL_GetWindowSurface(window);

    // Dispatches the events.
    event_loop(renderer);

    SDL_FreeSurface(surface);

    // Destroys the objects.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
