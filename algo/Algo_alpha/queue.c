#include <stdlib.h>


struct pqueue{
    struct pqueue* next;
    int x;
    int y;
    int heur;
    int cout;
    int x_father;
    int y_father;
};


int is_pq_empty(struct pqueue* p){
    if(p->next != NULL)
        return 0;
    return 1;
}


struct pqueue* create(){
    struct pqueue* pq = malloc(sizeof(struct pqueue));
    pq->next = NULL;
    return pq;
}

void add(struct pqueue* pq, int x, int y, int heur, int cout, int x_father, int y_father){
    struct pqueue* pq_add = malloc(sizeof(struct pqueue));
    pq_add->x = x;
    pq_add->y = y;
    pq_add->heur = heur;
    pq_add->cout = cout;
    pq_add->x_father = x_father;
    pq_add->y_father = y_father;
    while( pq->next != NULL && pq->next->heur < heur)
        pq = pq->next;
    struct pqueue* a = pq->next;
    pq->next = pq_add;
    pq_add->next = a;
}

void add_pqueue(struct pqueue* pq, struct pqueue* pq_add){
    while( pq->next != NULL && pq->next->heur < pq_add->heur)
        pq = pq->next;
    struct pqueue* a = pq->next;
    pq->next = pq_add;
    pq_add->next = a;
}


struct pqueue* get_first(struct pqueue* p){
    if(p->next == NULL)
        return NULL;
    struct pqueue* a = p->next;
    p->next = a->next;
    return a;
}



void suppr_pqueue(struct pqueue* p){
    struct pqueue* a;
    while((a = get_first(p)) != NULL)
        free(a);
    free(p);


}


int is_not_in_closedList(int x, int y, struct pqueue* closedList){
    struct pqueue* p = closedList->next;
    while(p != NULL){
        if(p->x == x && p->y == y)
            return 0;
        p = p->next;
    }
    return 1;
}




int is_in_pqueue(int x, int y, int cout, struct pqueue* closedList, struct pqueue* openList){
    struct pqueue* p = closedList->next;
    while(p != NULL){
        if(p->x == x && p->y == y)
            return 2;
        p = p->next;
    }
    struct pqueue* p2 = openList->next;
    while(p2 != NULL){
        if(p2->x == x && p2->y == y){
            if(p2->cout > cout){
                p2->x = x;
                p2->y = y;
                return 1;
            }
            else
                return 1;
        }
        p2 = p2->next;
    }
    return 0;
}

