#include"mycomplex.h"

SDL_Point c_add(SDL_Point z1, vect_double z2)
{
    SDL_Point sum;
    sum.x = z1.x + z2.x;
    sum.y = z1.y + z2.y;
    return sum;
}

SDL_Point c_sub(SDL_Point z1, vect_double z2)
{
    SDL_Point dif;
    dif.x = z1.x - z2.x;
    dif.y = z1.y - z2.y;
    return dif;
}

SDL_Point c_mul(SDL_Point z1, vect_double z2)
{
    SDL_Point prod;
    prod.x = (z1.x*z2.x) - (z1.y*z2.y);
    prod.y = (z1.x*z2.y) + (z1.y*z2.x);
    return prod;
}

SDL_Point c_div(SDL_Point z1, vect_double z2)
{
    int den = z2.x*z2.x + z2.y*z2.y;
    SDL_Point res;
    res.x = ((z1.x*z2.x) + (z1.y*z2.y)) / den;
    res.y = ((z1.y*z2.x) - (z1.x*z2.y)) / den;
    return res;
}

SDL_Point c_sym(SDL_Point z)
{
    SDL_Point sym;
    sym.x = -z.x;
    sym.y = -z.y;
    return sym;
}

SDL_Point c_inv(SDL_Point z)
{
    int den = z.x*z.x + z.y*z.y;
    SDL_Point inv;
    inv.x = z.x / den;
    inv.y = -z.y / den;
    return inv;
}

SDL_Point c_conj(SDL_Point z)
{
    z.y = -z.y;
    return z;
}

/*
   SDL_Point c_pow(SDL_Point z, unsigned int n)
   {
   if(n == 0)
   {
   SDL_Point one;
   one.x = 1;
   one.y = 0;
   return one;
   }
   if(n == 1)
   return z;
   SDL_Point res = c_pow(z, n/2);
   if (!(n % 2))
   return c_mul(res, res);
   if (n % 2)
   return c_mul(z, c_mul(res, res));
   }*/

int c_modu(SDL_Point z)
{
    return sqrt(z.x*z.x + z.y*z.y);
}

double c_arg(SDL_Point z)
{
    if(z.x > 0)
        return atan(z.y/z.x);
    else if(z.y > 0)
        return PI/2.0 - atan(z.x/z.y);
    else if(z.y < 0)
        return -PI/2.0 - atan(z.x/z.y);
    else if(z.x < 0)
        return atan(z.y/z.x) + PI;

    return 0;
}

vect_double cart_from_pol(int r, double th)
{
    vect_double z;
    z.x = r*cos(th);
    z.y = r*sin(th);
    return z;
}

//SDL_Point cart

