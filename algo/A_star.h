#ifndef A_STAR_H
#define A_STAR_H


struct list_vect{
    int x;
    int y;
    struct list_vect * next;
};


int A_star(int x_begin, int y_begin, int x_end, int y_end, struct list_vect* chemin, int* map);

#endif
