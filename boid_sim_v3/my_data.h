#ifndef MY_DATA_H
#define MY_DATA_H

#include<stdio.h>
#include<stddef.h>
#include<string.h>
#include<stdint.h>
#include<time.h>
#include<math.h>
#include<SDL2/SDL.h>
#define _GNU_SOURCE

typedef struct long_double2{
    long double x,y;
} long_double2;

void Init();
void Start();
void Loop();
void Update();
void Kill();
void ExitWithError();

void dir_agent(size_t i, float deg);
void move_agent(size_t i);
void rot_agent(size_t i, float ged);
void UpdateGrid();
void CheckTheta();
void resolve_blockage();
void wiggle_agent(size_t i);
//static inline double dist(double x1, double y1, double x2, double y2);
void follow_agent(size_t n);

void set_rules(size_t n);

//static inline long double nextX(size_t i);
//static inline long double nextY(size_t i);
//static inline long double nextVX(size_t i);
//static inline long double nextVY(size_t i);

//void align_add(size_t n, size_t id);
//void align_sub(size_t n, size_t id);
//void align_div(size_t n, size_t nb);
//void align_rule(size_t n);

SDL_Point coord(double x, double y);



#endif // !MY_DATA_H
