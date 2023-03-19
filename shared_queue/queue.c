#include <err.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "queue.h"

queue* queue_push(queue* start, SDL_Renderer* r, int c, int m_x, int m_y,
        int radius, SDL_Rect* t)
{
    struct queue* q = malloc(sizeof(struct queue));
    if (q == NULL)
        err(EXIT_FAILURE, "Failure malloc push");

    q->renderer = r;
    q->color = c;
    q->mouse_x = m_x;
    q->mouse_y = m_y;
    q->radius = radius;
    q->target = t;

    if (start == NULL)
        q->next = q;
    else
    {
        q->next = start->next;
        start->next = q;
    }
    return q;
}

queue* queue_pop(queue* start, SDL_Renderer** r, int* c, int* m_x, int* m_y,
        int* radius, SDL_Rect** t)
{
    if (start == NULL)
        return NULL;

    struct queue* old = start->next;
    *r = old->renderer;
    *c = old->color;
    *m_x = old->mouse_x;
    *m_y = old->mouse_y;
    *radius = old->radius;
    *t = old->target;

    if (old != start)
    {
        start->next = old->next;
        free(old);
        return start;
    }
    else
    {
        free(start);
        return NULL;
    }

}

void queue_empty(queue** pstart)
{
    SDL_Renderer* r;
    int c;
    int m_x;
    int m_y;
    int radius;
    SDL_Rect* t;
    while (*pstart != NULL)
    {
        *pstart = queue_pop(*pstart, &r, &c, &m_x, &m_y, &radius, &t);
    }
}
