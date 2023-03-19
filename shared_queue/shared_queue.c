#include <err.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "shared_queue.h"

shared_queue* shared_queue_new()
{
    struct shared_queue* sq = malloc(sizeof(struct shared_queue));
    if (sq == NULL)
        err(EXIT_FAILURE, "Failure shared queue new");
    sem_init(&sq->lock, 0, 1);
    sem_init(&sq->size, 0, 0);
    sq->queue = NULL;
    return sq;
}

void shared_queue_push(shared_queue* sq, SDL_Renderer* r, int c, int m_x,
        int m_y, int radius, SDL_Rect* t)
{
    sem_wait(&sq->lock);
    sq->queue = queue_push(sq->queue, r, c, m_x, m_y, radius, t);
    sem_post(&sq->lock);
    sem_post(&sq->size);
}

void shared_queue_pop(shared_queue* sq, SDL_Renderer** r, int* c, int* m_x,
        int* m_y, int* radius, SDL_Rect** t)
{
    sem_wait(&sq->size);
    sem_wait(&sq->lock);
    sq->queue = queue_pop(sq->queue, r, c, m_x, m_y, radius, t);
    sem_post(&sq->lock);
}

void shared_queue_destroy(shared_queue* sq)
{
    queue_empty(&sq->queue);
    sem_destroy(&sq->lock);
    sem_destroy(&sq->size);
    free(sq);
}
