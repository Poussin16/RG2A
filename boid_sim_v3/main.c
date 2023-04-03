#include<stdio.h>
#include<stddef.h>
#include<string.h>
#include<stdint.h>
#include<time.h>
#include<math.h>
#include<SDL2/SDL.h>
#include"my_data.h"

int main()
{
	srand(time(NULL));
	Init();
	Start();
	Loop();
	Kill();
	return 0;
}
