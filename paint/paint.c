#include <err.h>
#include <math.h>
#include <SDL2/SDL.h>

#define PI 3.14159

// Initial width and height of the window.
const int INIT_WIDTH = 640;
const int INIT_HEIGHT = 400;

// Boolean paint
int PAINT = 0;

int DRAW = 0;
int ERASE = 255;

void draw(SDL_Renderer* renderer, int color, int mouse_x, int mouse_y, int radius)
{
    // Sets the color for drawing operations to black.
    SDL_SetRenderDrawColor(renderer, color, color, color, 255);

    const SDL_Rect rect = {mouse_x-(radius/2), mouse_y-(radius/2), radius, radius};

    //SDL_Surface* s = SDL_CreateRGBSurface(0, INIT_WIDTH, INIT_HEIGHT, 32, 0,0,0,0);
    //Uint32* pixels = s->pixels;
    //SDL_RenderReadPixels(renderer, &rect, SDL_PIXELFORMAT_RGBA8888,
      //      pixels, s->pitch);

    SDL_RenderFillRect(renderer, &rect);

    // Updates the display.
    SDL_RenderPresent(renderer);
}

// Event loop that calls the relevant event handler.
//
// renderer: Renderer to draw on.
void event_loop(SDL_Renderer* renderer)
{
    // Clears the renderer (sets the background to white).
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

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
                    draw(renderer, DRAW, event.motion.x, event.motion.y, 10);
                    PAINT = 1;
                }
                else if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    draw(renderer, ERASE, event.motion.x, event.motion.y, 20);
                    PAINT = -1;
                }
                break;

            case SDL_MOUSEBUTTONUP:
                PAINT = 0;
                break;
            // If the mouse is moving, updates the position of the cursor.
            case SDL_MOUSEMOTION:
                if (PAINT == 1)
                    draw(renderer, DRAW, event.motion.x, event.motion.y, 10);
                else if (PAINT == -1)
                    draw(renderer, ERASE, event.motion.x, event.motion.y, 20);
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
    SDL_Window* window = SDL_CreateWindow("Dynamic Fractal Canopy", 0, 0, INIT_WIDTH, INIT_HEIGHT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    /*SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, INIT_WIDTH, INIT_HEIGHT);
    if (texture == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    */
    // Dispatches the events.
    event_loop(renderer);

    // Destroys the objects.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
