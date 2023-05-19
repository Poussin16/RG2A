#ifndef A_STAR_H
#define A_STAR_H


struct list_vect{
    int x;
    int y;
    int x_father;
    int y_father;
    struct list_vect * next;
};


void free_list_vect(struct list_vect* chemin);

struct list_vect* A_star(int x_begin, int y_begin, int x_end, int y_end,
        int depth, int* map, int cols_2, int raw_2);

#endif
