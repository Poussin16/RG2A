#include "queue.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>


int cols;


struct list_vect{
    int x;
    int y;
    struct list_vect* next;
};



int distance(int x1, int y1, int x2, int y2){
    int pst = (x2 - x1)*(x2 - x1);
    int pst2 = (y2 - y1)*(y2 - y1);
    float t = ((float)pst) + ((float)pst2);
    int rst = (int)sqrt(t);
    return rst;
}


void add_list_vect(struct list_vect* c, int x, int y){
    struct list_vect* p = malloc(sizeof(struct list_vect));
    p->x = x;
    p->y = y;
    p->next = c->next;
    c->next = p;
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
    if(u->x == x_end && u->y == y_end){
        add_list_vect(chemin, u->x, u->y);
        return 1;
    }
    if(((u->x) -1 ) > -1){
        if(((u->y) -1) > -1){
            int u_y = u->y -1;
            int u_x = u->x -1;
            if(map[u_x * cols + u_y] == 0){
                if(is_in_pqueue(u_x, u_y, u->cout, closedList, openList) == 0){
                    int heur = distance(u_x, u_y, x_end, y_end) + u->cout + 1;
                    add(openList, u_x, u_y, heur, (u->cout + 1));
                }
            }
        }
        if(((u->y)) > -1){
            int u_y = u->y;
            int u_x = u->x -1;
            if(map[u_x * cols + u_y] == 0){
                if(is_in_pqueue(u_x, u_y, u->cout, closedList, openList) == 0){
                    int heur = distance(u_x, u_y, x_end, y_end) + u->cout + 1;
                    add(openList, u_x, u_y, heur, (u->cout + 1));
                }
            }
        }
        if(((u->y) + 1 ) < cols){
            int u_y = u->y +1;
            int u_x = u->x -1;
            if(map[u_x * cols + u_y] == 0){
                if(is_in_pqueue(u_x, u_y, u->cout, closedList, openList) == 0){
                    int heur = distance(u_x, u_y, x_end, y_end) + u->cout + 1;
                    add(openList, u_x, u_y, heur, (u->cout + 1));
                }
            }
        }
    }

   if(((u->x) ) > -1){
        if(((u->y) -1) > -1){
            int u_y = u->y -1;
            int u_x = u->x;
            if(map[u_x * cols + u_y] == 0){
                if(is_in_pqueue(u_x, u_y, u->cout, closedList, openList) == 0){
                    int heur = distance(u_x, u_y, x_end, y_end) + u->cout + 1;
                    add(openList, u_x, u_y, heur, (u->cout + 1));
                }
            }
        }
        if(((u->y) + 1 ) < cols){
            int u_y = u->y +1;
            int u_x = u->x;
            if(map[u_x * cols + u_y] == 0){
                if(is_in_pqueue(u_x, u_y, u->cout, closedList, openList) == 0){
                    int heur = distance(u_x, u_y, x_end, y_end) + u->cout + 1;
                    add(openList, u_x, u_y, heur, (u->cout + 1));
                }
            }
        }
   }

    if(((u->x) + 1 ) < cols){
        if(((u->y) -1) > -1){
            int u_y = u->y -1;
            int u_x = u->x +1;
            if(map[u_x * cols + u_y] == 0){
                if(is_in_pqueue(u_x, u_y, u->cout, closedList, openList) == 0){
                    int heur = distance(u_x, u_y, x_end, y_end) + u->cout + 1;
                    add(openList, u_x, u_y, heur, (u->cout + 1));
                }
            }
        }
        if(((u->y)) > -1){
            int u_y = u->y;
            int u_x = u->x +1;
            if(map[u_x * cols + u_y] == 0){
                if(is_in_pqueue(u_x, u_y, u->cout, closedList, openList) == 0){
                    int heur = distance(u_x, u_y, x_end, y_end) + u->cout + 1;
                    add(openList, u_x, u_y, heur, (u->cout + 1));
                }
            }
        }
        if(((u->y) + 1 ) < cols){
            int u_y = u->y +1;
            int u_x = u->x +1;
            if(map[u_x * cols + u_y] == 0){
                if(is_in_pqueue(u_x, u_y, u->cout, closedList, openList) == 0){
                    int heur = distance(u_x, u_y, x_end, y_end) + u->cout + 1;
                    add(openList, u_x, u_y, heur, (u->cout + 1));
                }
            }
        }
    }

    /*
    if(A(openList, closedList, x_end, y_end, chemin, map)){
        printf("test\n");
        if(is_not_in_closedList(u->x, u->y, closedList)){
                add_list_vect(chemin, u->x, u->y);
                add_pqueue(closedList, u);
        }
        return 1;
    }
    add_pqueue(closedList, u);
    */
    add_pqueue(closedList, u);
    if(A(openList, closedList, x_end, y_end, chemin, map)){
        add_list_vect(chemin, u->x, u->y);
        return 1;
    }

    return 0;
}






int A_star(int x_begin, int y_begin, int x_end, int y_end, 
        struct list_vect* chemin, int* map, int cols_2){
    cols = cols_2;
    struct pqueue* closedList = create();
    struct pqueue* openList = create();
    add(openList, x_begin, y_begin, 0, 0);
    int rslt = A(openList, closedList, x_end, y_end, chemin, map);
    suppr_pqueue(closedList);
    suppr_pqueue(openList);
    return rslt;
}
