#ifndef QUEUE_H
#define QUEUE_H


struct pqueue{
    struct pqueue* next;
    int x;
    int y;
    int heur;
    int cout;
};

int is_pq_empty(struct pqueue* p);


struct pqueue* create();

struct pqueue* get_first(struct pqueue* p);

void add(struct pqueue* pq, int x, int y, int heur, int cout);

void add_pqueue(struct pqueue* pq, struct pqueue* pq_add);

void suppr_pqueue(struct pqueue* p);

struct pqueue* get_first(struct pqueue* p);

int is_in_pqueue(int x, int y, int cout, struct pqueue* closedList, struct pqueue* openList);

#endif
