
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "A_star.h"

int map_build[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 1, 1, 1, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 1, 1, 1, 1, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };



void print_map(int* map){
    printf("map == \n");
    for(int i = 0;i < 10; i++){
        for(int j = 0; i<10; j++){
            printf("%i", *(map+i* 10 +j));
        }
        printf("\n");
    }
    printf("\n\n");
}

int main(){
    struct list_vect* chemin = malloc(sizeof(struct list_vect));
    chemin->x = 0;
    chemin->y = 0;
    chemin->next = NULL;
    print_map(map_build);
    if(A(0, 0, 9, 9, chemin, map_build))
        printf("A* succeed\n\n");
    while(chemin != NULL){
        struct list_vect* c = chemin;
        int x_vect = c->x;
        int y_vect = c->y;
        map_build[x_vect * 10 + y_vect] = 9;
        chemin = chemin->next;
        free(c);
    }
    print_map(map_build);
    return 1;
}


