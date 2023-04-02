#include "queue.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>


int cols;


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




/*
int distance(int x1, int y1, int x2, int y2){
    int pst = (x2 - x1)*(x2 - x1);
    int pst2 = (y2 - y1)*(y2 - y1);
    float t = ((float)pst) + ((float)pst2);
    int rst = (int)sqrt(t);
    return rst;
}
*/


void add_list_vect(struct list_vect* c, int x, int y, int x_f, int y_f){
    struct list_vect* p = malloc(sizeof(struct list_vect));
    p->x = x;
    p->y = y;
    p->x_father = x_f;
    p->y_father = y_f;
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



    printf("test %i    %i\n", u->x, u->y);

    //iteration sur voisins de u
    for(int dr = -1; dr <=1; dr++){
        for(int dc = -1; dc <= 1; dc++){
            if(dr == 0 && dc == 0)
                continue;

            int u_x = u->x + dr;
            int u_y = u->y + dc;

            if(u_x >= 0 && u_x < cols && u_y >= 0 && u_y < cols){
                if(u_x == x_end && u_y == y_end){
                    add_list_vect(chemin, u->x, u->y, u->x_father, u->y_father);
                    printf("We found it\n\n");
                    return 1;
                }
                if(map[u_x*cols +u_y] == 0){
                    if(is_in_pqueue(u_x, u_y, u->cout, closedList, openList) == 0){
                        int heur = distance(u_x, u_y, x_end, y_end) + u->cout + 2;
                        add(openList, u_x, u_y, heur, (u->cout + 1), u->x, u->y);
                    }
                }
            }
        }
    }


    add_pqueue(closedList, u);
    if(A(openList, closedList, x_end, y_end, chemin, map) == 1){
        add_list_vect(chemin, u->x, u->y, u->x_father, u->y_father);
        printf("found it !!!!\n\n");
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





struct list_vect* pathcleaning(struct list_vect* chemin, int x_end, int y_end){
    int X_father = 0;
    int Y_father = 0;
    for(struct list_vect* a = chemin; a!=NULL; a = a->next){
        if(a->x == x_end && a->y == y_end){
            printf("test2 x_father = %i y_father = %i\n",a->x_father, a->y_father);
            X_father = a->x_father;
            Y_father = a->y_father;
            break;
        }
    }

    struct list_vect* final_path = malloc(sizeof(struct list_vect));
    add_list_vect(final_path, x_end, y_end, X_father, Y_father);

    while(X_father != -1 && Y_father != -1){
        printf("test2 x_father = %i y_father = %i\n",X_father, Y_father);
        struct list_vect* f = found_in(chemin, X_father, Y_father);
        add_list_vect(final_path, f->x, f->y, f->x_father, f->y_father);
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
        



int A_star(int x_begin, int y_begin, int x_end, int y_end, 
        struct list_vect* chemin, int* map, int cols_2){
    cols = cols_2;
    struct pqueue* closedList = create();
    struct pqueue* openList = create();

    add(openList, x_begin, y_begin, 0, 0, -1, -1);
    int rslt = A(openList, closedList, x_end, y_end, chemin, map);

    printf("oh mon frere\n");

    struct list_vect* final_path = pathcleaning(chemin, x_end, y_end);
    struct list_vect* TEST = chemin->next;
    chemin = final_path;

    suppr_pqueue(closedList);
    suppr_pqueue(openList);
    return rslt;
}
