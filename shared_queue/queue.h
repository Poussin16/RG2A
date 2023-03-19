#ifndef QUEUE
#define QUEUE

typedef struct queue
{
    SDL_Renderer* renderer;
    int color;
    int mouse_x;
    int mouse_y;
    int radius;
    SDL_Rect* target;

    // Pointer to the next element.
    struct queue *next;
} queue;

// Pushes a new value onto a queue.
// start = Starting address of the queue.
// val = Value to push.
// Returns the new starting address of the queue.
queue* queue_push(queue* start, SDL_Renderer* r, int c, int m_x, int m_y,
        int radius, SDL_Rect* t);

// Pops a value off a queue.
// start = Starting address of the queue.
// pval = Pointer used to return the value.
// Returns the new starting address of the queue.
queue* queue_pop(queue* start, SDL_Renderer** r, int* c, int* m_x, int* m_y,
        int* radius, SDL_Rect** t);

// Removes all the elements of a queue.
// pstart = Address that contains the starting address of the queue.
// Must set the starting address to NULL.
void queue_empty(queue** pstart);

#endif
