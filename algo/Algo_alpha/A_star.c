#include "queue.h"
#include <math.h>
#include <stdlib.h>
#include <err.h>
#include <stdio.h>


int cols;
int raw;


struct list_vect{
    int x;
    int y;
    int x_father;
    int y_father;
    struct list_vect* next;
};




// distance based on abso
int distance(int x1, int y1, int x2, int y2){
    return (int)(abs(x1-x2) + abs(y1 - y2));
}


void add_list_vect(struct list_vect* c, int x, int y, int x_f, int y_f){
    struct list_vect* p = malloc(sizeof(struct list_vect));
    p->x = x;
    p->y = y;
    p->x_father = x_f;
    p->y_father = y_f;
    p->next = NULL;
    while(c->next != NULL)
        c = c->next;
    c->next = p;
    //if(c->x == -1 && c->y == -1)
      //  c->next = p;
    //else{
      //  if((c->x  == x+1  || c->x  == x-1) || (c->y == y-1 || c->y == y+1))
        //     c->next = p;
        //else
          //  free(p);
    //}
}

struct list_vect* add_list_vect_2(struct list_vect* c, int x, int y, int x_f, int y_f){
    struct list_vect* p = malloc(sizeof(struct list_vect));
    p->x = x;
    p->y = y;
    p->x_father = x_f;
    p->y_father = y_f;
    p->next = c;
    return p;
}


int compare_heu(struct pqueue* a,struct pqueue* b){
    if(a->heur < b->heur)
        return 1;
    if(a->heur == b->heur)
        return 0;
    return -1;
}



//recusion sur la map pour trouver la cible en se basant sur l'algorithme de A*
//return True si chemin trouver
//       False sinon


int A(struct pqueue* openList, struct pqueue* closedList, int x_end, int y_end, 
        struct list_vect* chemin, int* map){
    struct pqueue* u = get_first(openList);

    int a = 10;
    if(x_end-a <= u->x && u->x <= x_end+a &&
            y_end-a <= u->y && u->y <= y_end+a){
        add_list_vect(chemin, u->x, u->y, u->x_father, u->y_father);
        //printf("FIND\n");
        return 1;
    }

    //iteration sur voisins de u
    for(int dr = -1; dr <=1; dr++){
        for(int dc = -1; dc <= 1; dc++){
            if(dr == 0 && dc == 0)
                continue;

            int u_x = u->x + dr*a;
            int u_y = u->y + dc*a;

            if(u_x >= 0 && u_x < cols && u_y >= 0 && u_y < raw){

                if(map[u_y*cols +u_x] != 1){
                    if(is_in_pqueue(u_x, u_y, u->cout, closedList, openList) == 0){
                        int heur = distance(u_x, u_y, x_end, y_end) + u->cout + 2;
                        add(openList, u_x, u_y, heur, (u->cout + 1), u->x, u->y);
                    }
                }
            }
        }
    }
    

    add_pqueue(closedList, u);
    int x = A(openList, closedList, x_end, y_end, chemin, map);
    if(x == 1){
        add_list_vect(chemin, u->x, u->y, u->x_father, u->y_father);
        return 1;
    }

    return 0;
}


// sans cible pareille que en haut.
int A_2(struct pqueue* openList, struct pqueue* closedList,
        struct list_vect* chemin, int* map){
    struct pqueue* u = get_first(openList);


    if(map[u->y *cols +u->x]  == 2){
        add_list_vect(chemin, u->x, u->y, u->x_father, u->y_father);
        return 1;
    }

    //iteration sur voisins de u
    for(int dr = -1; dr <=1; dr++){
        for(int dc = -1; dc <= 1; dc++){
            if(dr == 0 && dc == 0)
                continue;

            int u_x = u->x + dr;
            int u_y = u->y + dc;

            if(u_x >= 0 && u_x < cols && u_y >= 0 && u_y < raw){

                if(map[u_y*cols +u_x] == 0 || map[u_y*cols + u_x] == 2){
                    if(is_in_pqueue(u_x, u_y, u->cout, closedList, openList) == 0){
                        add(openList, u_x, u_y, 0, (u->cout + 1), u->x, u->y);
                    }
                }
            }
        }
    }


    add_pqueue(closedList, u);
    int x = A_2(openList, closedList, chemin, map);
    if(x == 1){

        add_list_vect(chemin, u->x, u->y, u->x_father, u->y_father);
        return 1;
    }

    return 0;
}



    

struct list_vect* found_in(struct list_vect* path,int  x,int  y){
    for(struct list_vect* a = path; a!= NULL; a = a->next){
        if(a->x == x && a->y == y)
            return a;
    }
    return NULL;
}





struct list_vect* pathcleaning(struct list_vect* chemin){
    int X_father = chemin->next->x_father;
    int Y_father = chemin->next->y_father;

    struct list_vect* final_path = malloc(sizeof(struct list_vect));
    final_path->next = NULL;


    add_list_vect(final_path, chemin->next->x, chemin->next->y, X_father, Y_father);

    while(X_father != -1 && Y_father != -1){
        struct list_vect* f = found_in(chemin, X_father, Y_father);
        final_path = add_list_vect_2(final_path, f->x, f->y, f->x_father, f->y_father);
        X_father = f->x_father;
        Y_father = f->y_father;
    }

    return final_path;
}




void free_list_vect(struct list_vect* chemin){
    while(chemin != NULL){
        struct list_vect* a = chemin;
        chemin = chemin->next;
        free(a);
    }
}
        




// x_begin, y_begin : start
// x_end, y_end : destination 
//
//depth : depth were the boid need to be.
//
// map : map
// cols_2 : column number
// raw_2 : raw number




struct list_vect* A_star(int x_begin, int y_begin, int x_end, int y_end,
        int depth, int* map, int cols_2, int raw_2){

    struct list_vect* chemin = malloc(sizeof(struct list_vect));
    chemin->x = -1;
    chemin->y = -1;
    chemin->next = NULL;


    cols = cols_2;
    raw = raw_2;

    struct pqueue* closedList = create();
    struct pqueue* openList = create();

    add(openList, x_begin, y_begin, 0, 0, -1, -1);

    int rslt;
    if(x_end == -1 || y_end == -1){
        rslt = A_2(openList, closedList, chemin, map);
    }
    else{
        rslt = A(openList, closedList, x_end, y_end, chemin, map);
    }

    if(rslt != 1){
        return NULL;//errx(1,"error while founding the objective");
    }

    struct list_vect* final_path = pathcleaning(chemin);

    struct list_vect* f = final_path;

    while(f->next != NULL && depth >= 0){
        f = f->next;
        depth -= 1;
    }
    f->next = NULL;
    
    /*for (struct list_vect* f = chemin; f != NULL; f = f->next){
        printf("x:%i\n", f->x);
    }*/
    suppr_pqueue(closedList);
    suppr_pqueue(openList);
    return final_path;
}
