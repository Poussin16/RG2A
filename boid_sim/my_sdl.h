#ifndef MY_SDL_H
#define MY_SDL_H

#include<err.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<SDL2/SDL.h>
#include"mycomplex.h"

#include <time.h>
#include <pthread.h>

#define Draw_Polygon SDL_RenderDrawLines
#define MOD(x,y) sqrt(x*x + y*y)


typedef struct Boid
{
	SDL_Point tr[4]; //triangle
	SDL_Point ctr; //center
	SDL_Point step; //direction
	int vel; //velocity
	int acc; //acceleration
	int oth; //angle
	int r;
	int g;
	int b;
} Boid;

int randint(int min, int max);

void set_run(const char run);
void main_loop(SDL_Renderer *renderer, int Window_Width, int Window_Height);

void set_triangle(Boid *boid);
SDL_Point rotate_point(SDL_Point point, SDL_Point center, double deg_angle);
void rotate_boid(Boid *boid, double th);
void move_boid(Boid *boid);
void move_boids(Boid *boid, size_t n);
void refresh_boid_rot(Boid *boid);
void refresh_boids_rot(Boid *boids, size_t n);

/////////
void wrap_thr_edge(Boid *boid);
void wrap_thr_edges(Boid *boids, size_t n);
////////

void init_boids_rot(Boid *boid, size_t n);
void init_boid(Boid *boid);
void init_boids(Boid *boid, size_t n);
void draw_boid(SDL_Renderer *renderer, Boid *boid, int draw_b);
void draw_boids(SDL_Renderer *renderer, Boid *boid, size_t n, int draw_b);
void SDL_ExitWithError(const char *message);

/////////
void align(Boid *boid, Boid *boids, size_t id, size_t *dists, size_t n);
void separate(Boid *boid, Boid *boids, size_t id, size_t *dists, size_t n);
void cohere(Boid *boid, Boid *boids, size_t id, size_t *dists, size_t n);
int new_th(int x, int a);
void set_dists(Boid *boid, Boid *boids, size_t *dists, size_t n);
size_t dist(SDL_Point p1, SDL_Point p2);
int detect_point(Boid *boid, SDL_Point point);
//void set_dists(Boid *boid, Boid *boids, size_t *dists, size_t n);

/////////
void set_coef_align(size_t coef);
void set_coef_sep(size_t coef);
void set_coef_coh(size_t coef);

/////////
void set_active_obstacle(size_t set);
void set_active_pathfinding(size_t set);

#endif
