#ifndef MYCOMPLEX_H
#define MYCOMPLEX_H

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<SDL2/SDL.h>

#define PI 3.141592653589793238

typedef struct vect_double
{
	double x, y;
} vect_double;

SDL_Point c_add(SDL_Point z1, vect_double z2);
SDL_Point c_sub(SDL_Point z1, vect_double z2);
SDL_Point c_mul(SDL_Point z1, vect_double z2);
SDL_Point c_div(SDL_Point z1, vect_double z2);
SDL_Point c_sym(SDL_Point z);
SDL_Point c_inv(SDL_Point z);
SDL_Point c_conj(SDL_Point z);
//SDL_Point c_pow(SDL_Point z, unsigned int n);
int c_modu(SDL_Point z);
double c_arg(SDL_Point z);
vect_double cart_from_pol(int r, double th);

#endif

