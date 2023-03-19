#ifndef PAINT_H
#define PAINT_H

void drawrect(SDL_Renderer* renderer, int color,
        int mouse_x, int mouse_y, int radius, SDL_Rect* target);
void drawtarget(SDL_Renderer* renderer, SDL_Rect* target, const int INIT_WIDTH,
        const int INIT_HEIGHT);
void* worker(void* arg);

typedef struct draw_rect {
	SDL_Renderer* renderer;
	int color;
	int mouse_x;
	int mouse_y;
	int radius;
	SDL_Rect* target;
} draw_rect;

#endif
