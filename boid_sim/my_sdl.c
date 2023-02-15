#include"my_sdl.h"

/*
void Draw_Polygon(SDL_Renderer *renderer, polygon poly)
{
	SDL_RenderDrawLines(renderer,poly.points,poly.size);
}
*/
/*
SDL_Points Gen_Points(int x, int y, unsigned int l, size_t n)
{

}*/

int randint(int min, int max)
{
	//srand(time(NULL));
	return ((rand() % (max - min + 1)) + min);
}

SDL_Point rotate_point(SDL_Point point, SDL_Point center, double deg_angle)
{
	double rad_angle = deg_angle * (PI / 180);
	double cos_th = cos(rad_angle);
	double sin_th = sin(rad_angle);
	int x, y;
	x = (int)(cos_th * (point.x - center.x) -
		sin_th * (point.y - center.y) + center.x);
	y = (int)(sin_th * (point.x - center.x) +
		cos_th * (point.y - center.y) + center.y);
	SDL_Point res;
	res.x = x;
	res.y = y;
	return res;
/*
	SDL_Point rot = cart_from_pol(1, 15);
	SDL_Point tmp = c_mul(boid->ctr, rot);

	boid->tr[0] = c_mul(boid->tr[0], rot);
	boid->tr[1] = c_mul(boid->tr[1], rot);
	boid->tr[2] = c_mul(boid->tr[2], rot);

	boid->tr[0] = c_sub(boid->tr[0], tmp);
	boid->tr[1] = c_sub(boid->tr[1], tmp);
	boid->tr[2] = c_sub(boid->tr[2], tmp);

	boid->tr[0] = c_add(boid->tr[0], boid->ctr);
	boid->tr[1] = c_add(boid->tr[1], boid->ctr);
	boid->tr[2] = c_add(boid->tr[2], boid->ctr);

	boid->tr[3] = boid->tr[0];

	boid->ctr.x = (boid->tr[0].x + boid->tr[1].x + boid->tr[2].x) / 3;
	boid->ctr.y = (boid->tr[0].y + boid->tr[1].y + boid->tr[2].y) / 3;
*/
}

void rotate_boid(Boid *boid, double th)
{
	if(th == 0)
		return;
	boid->tr[0] = rotate_point(boid->tr[0], boid->ctr, th);
	boid->tr[1] = rotate_point(boid->tr[1], boid->ctr, th);
	boid->tr[2] = rotate_point(boid->tr[2], boid->ctr, th);
	boid->tr[3] = boid->tr[0];
	boid->ctr.x = (boid->tr[0].x + boid->tr[1].x + boid->tr[2].x) / 3;
	boid->ctr.y = (boid->tr[0].y + boid->tr[1].y + boid->tr[2].y) / 3;
	boid->step.x = boid->tr[0].x - boid->ctr.x;
	boid->step.y = boid->tr[0].y - boid->ctr.y;
	boid->oth += th;
}

void move_boid(Boid *boid)
{
	boid->tr[0].x += boid->step.x * boid->vel;
	boid->tr[0].y += boid->step.y * boid->vel;
	boid->tr[1].x += boid->step.x * boid->vel;
	boid->tr[1].y += boid->step.y * boid->vel;
	boid->tr[2].x += boid->step.x * boid->vel;
	boid->tr[2].y += boid->step.y * boid->vel;
	boid->tr[3] = boid->tr[0];
	boid->ctr.x = (boid->tr[0].x + boid->tr[1].x + boid->tr[2].x) / 3;
	boid->ctr.y = (boid->tr[0].y + boid->tr[1].y + boid->tr[2].y) / 3;
}

void move_boids(Boid *boid, size_t n)
{
	for(size_t i = 0; i < n; i++)
	{
		move_boid(&boid[i]);
	}
}

void init_boids_rot(Boid *boid, size_t n)
{
	for(size_t i = 0; i < n; i++)
	{
		rotate_boid(&boid[i], randint(-180, 180));
	}
}

void wiggle_boids(Boid *boid, size_t n, int min, int max)
{
	for(size_t i = 0; i < n; i++)
	{
		rotate_boid(&boid[i], randint(min, max));
	}
}

void set_triangle(Boid *boid)
{
	boid->tr[0].x = randint(0,800);
	boid->tr[0].y = randint(0,600);
	boid->tr[1].x = boid->tr[0].x + 3;
	boid->tr[1].y = boid->tr[0].y - 9;
	boid->tr[2].x = boid->tr[0].x - 3;
	boid->tr[2].y = boid->tr[0].y - 9;
	boid->tr[3] = boid->tr[0];
}

void init_boid(Boid *boid)
{
	set_triangle(boid);
	boid->ctr.x = (boid->tr[0].x + boid->tr[1].x + boid->tr[2].x) / 3;
	boid->ctr.y = (boid->tr[0].y + boid->tr[1].y + boid->tr[2].y) / 3;
	boid->step.x = (boid->tr[0].x - boid->ctr.x)/6;
	boid->step.y = (boid->tr[0].y - boid->ctr.y)/6;
	boid->vel = 1;
	boid->acc = 0;
	boid->oth = 0;
}

void refresh_boid_rot(Boid *boid)
{
	boid->tr[0].x = boid->ctr.x;
	boid->tr[0].y = boid->ctr.y - 9;
	boid->tr[1].x = boid->tr[0].x + 3;
	boid->tr[1].y = boid->tr[0].y + 9;
	boid->tr[2].x = boid->tr[0].x - 3;
	boid->tr[2].y = boid->tr[0].y + 9;
	boid->tr[3] = boid->tr[0];

	if(boid->oth >= 180)
	{
		boid->oth = -(180 - (boid->oth % 180));
	}
	else if(boid->oth <= -180)
	{
		boid->oth = (180 - ((-boid->oth) % 180));
	}

	int save = boid->oth;
	rotate_boid(boid, save);
	boid->oth = save;
}

void refresh_boids_rot(Boid *boids, size_t n)
{
	for(size_t i = 0; i < n; i++)
	{
		refresh_boid_rot(&boids[i]);
	}
}

/////////////////////////////////////

void wrap_thr_edge(Boid *boid)
{
	if(boid->ctr.x > 800){
		boid->ctr.x = 9;
	}else if (boid->ctr.x < 0){
		boid->ctr.x = 800 - 9;
	}
	if(boid->ctr.y > 600){
		boid->ctr.y = 9;
	}
	else if (boid->ctr.y < 0){
		boid->ctr.y = 600 - 9;
	}
	refresh_boid_rot(boid);
}

void wrap_thr_edges(Boid *boids, size_t n)
{
	for(size_t i = 0; i < n; i++)
	{
		wrap_thr_edge(&boids[i]);
	}
}

size_t dist(SDL_Point p1, SDL_Point p2)
{
	int t1 = (p2.x-p1.x);
	int t2 = (p2.y-p1.y);
	return (int)sqrt(t1*t1 + t2*t2);
}

int detect_point(Boid *boid, SDL_Point point)
{
	int d1 = dist(boid->tr[1], point); //left
	int d2 = dist(boid->tr[2], point); //right
	int d0 = dist(boid->tr[0], point); //front

	if(d1 < d0 || d2 < d0)
		return 0;
	if(d1 < d2)
		return 1;
	else
		return -1;
}

int detect_point2(Boid *boid, SDL_Point point)
{
	int d1 = dist(boid->tr[1], point); //left
	int d2 = dist(boid->tr[2], point); //right

	if(d1 < d2)
		return 1;
	else
		return -1;
}


void set_dists(Boid *boid, Boid *boids, size_t *dists, size_t n)
{
	for(size_t i = 0; i < n; i++)
	{
		dists[i] = dist(boid->ctr, boids[i].ctr);
	}
}

int new_th(int x, int a)
{
	int y = x + a;
	if(y >= 180)
	{
		y = -(180 - (y % 180));
	}
	else if(y <= -180)
	{
		y = (180 - ((-y) % 180));
	}
	return y;
}

void align(Boid *boid, Boid *boids, size_t id, size_t *dists, size_t n)
{
	size_t perception = 100;
	int dir;
	for(size_t i = 0; i < n; i++)
	{
		dir = detect_point(boid, boids[i].ctr);
		if((i != id) && (dists[i] <= perception) && (dir != 0))
		{
			if(dir == 1)
				boid->oth = new_th(boid->oth, +4);
			else if(dir == -1)
				boid->oth = new_th(boid->oth, -4);
		}
	}
	refresh_boid_rot(boid);
}

void separate(Boid *boid, Boid *boids, size_t id, size_t *dists, size_t n)
{
	size_t perception = 25;
	int dir;
	for(size_t i = 0; i < n; i++)
	{
		dir = detect_point(boid, boids[i].ctr);
		if((i != id) && (dists[i] <= perception) && (dir != 0))
		{
			if(dir == 1)
				boid->oth = new_th(boid->oth, -4);
			else if(dir == -1)
				boid->oth = new_th(boid->oth, +4);
		}
	}
	refresh_boid_rot(boid);
}

void cohere(Boid *boid, Boid *boids, size_t id, size_t *dists, size_t n)
{
	size_t perception = 100;
	int dir, count;
	SDL_Point point;
	count = 0;
	for(size_t i = 0; i < n; i++)
	{
		dir = detect_point(boid, boids[i].ctr);
		if((i != id) && (dists[i] <= perception) && (dir != 0))
		{
			point.x += boids[i].ctr.x;
			point.y += boids[i].ctr.y;
			count++;
		}
	}
	if(count == 0)
		return;
	point.x = point.x / count;
	point.y = point.y / count;
	dir = detect_point(boid, point);
	if(dir == 1)
		boid->oth = new_th(boid->oth, +4);
	else if(dir == -1)
		boid->oth = new_th(boid->oth, -4);

	refresh_boid_rot(boid);

	/*
	size_t perception = 100;
	long pos_x, pos_y, nb_scanned;
	int dir;
	nb_scanned = 0;
	for(size_t i = 0; i < n; i++)
	{
		dir = detect_point(boid, boids[i].ctr);
		if((i != id) && (dists[i] <= perception) && (dir != 0))
		{
			pos_x += boids[i].ctr.x;
			pos_y += boids[i].ctr.y;
			nb_scanned += 1;
		}
	}
	pos_x /= nb_scanned;
	pos_y /= nb_scanned;
	SDL_Point tmp;
	tmp.x = pos_x;
	tmp.y = pos_y;
	dir = detect_point(boid, tmp);
	if(dir == 1)
		boid->oth = new_th(boid->oth, +10);
	else if(dir == -1)
		boid->oth = new_th(boid->oth, -10);
	refresh_boid_rot(boid);
	*/
}

/////////////////////////////////////

void draw_boid(SDL_Renderer *renderer, Boid *boid)
{
	if(SDL_RenderDrawLines(renderer,boid->tr,4))
		SDL_ExitWithError("SDL_RenderDrawLines");
}

void init_boids(Boid *boid, size_t n)
{
	for(size_t i = 0; i < n; i++)
	{
		init_boid(&boid[i]);
	}
}

void draw_boids(SDL_Renderer *renderer, Boid *boid, size_t n)
{
	for(size_t i = 0; i < n; i++)
	{
		draw_boid(renderer, &boid[i]);
		//printf("( %d , %d )", boid[i].ctr.x , boid[i].ctr.y);
	}
}

void SDL_ExitWithError(const char *message)
{
	SDL_Log("ERROR: %s > %s\n",
			message,
			SDL_GetError());
	SDL_Quit();
}

#define NB_B 500

void main_loop(SDL_Renderer *renderer)//,SDL_Texture *map)
{
	srand(time(NULL));
	Boid boids[NB_B];
	size_t dists[NB_B];
	int error;
	char running = 1;
	init_boids(boids, NB_B);
	init_boids_rot(boids, NB_B);
	SDL_Event event;
	while(running)
	{
		//while(SDL_WaitEvent(&event) && running)
		//{
		/*SDL_WaitEvent(&event);
			if(event.type == SDL_QUIT)
			{
				running = 0;
				return;
			}
			else if(event.type == SDL_KEYDOWN)
			{
				if(event.key.keysym.sym == SDLK_ESCAPE)
				{
					running = 0;
					return;
				}
			}*/
			
		SDL_WaitEvent(&event);
		switch (event.type)
		{
			case SDL_QUIT:
				return;
			case SDL_KEYDOWN:
				error = SDL_SetRenderDrawColor(renderer,
						0x00, 0x00, 0x00, 0xFF);
				if(error < 0)
					SDL_ExitWithError("SDL_SetRenderDrawColor");
				error = SDL_RenderClear(renderer);
				if(error < 0)
					SDL_ExitWithError("SDL_RenderClear");

			/*
			SDL_Point triangle[4];
			triangle[0].x = triangle[0].y = 100;
			triangle[1].x = triangle[1].y = 300;
			triangle[2].x = 200;
			triangle[2].y = 300;
			triangle[3] = triangle[0];
			*/

				error = SDL_SetRenderDrawColor(renderer,
						0xFF, 0xFF, 0xFF, 0xFF);
				if(error < 0)
					SDL_ExitWithError("SDL_SetRenderDrawColor");

				draw_boids(renderer, boids, NB_B);
				move_boids(boids, NB_B);
				wiggle_boids(boids, NB_B, -1, +1);
				refresh_boids_rot(boids, NB_B);
				wrap_thr_edges(boids, NB_B);
				for(size_t i = 0; i < NB_B; i++)
				{
					set_dists(&boids[i], boids,
						dists, NB_B);
					align(&boids[i],
						boids, i,
						dists, NB_B);
				}
				for(size_t i = 0; i < NB_B; i++)
				{
					set_dists(&boids[i], boids,
						dists, NB_B);
					separate(&boids[i],
						boids, i,
						dists, NB_B);
				}
				//printf("test\n");
				for(size_t i = 0; i < NB_B; i++)
				{
					set_dists(&boids[i], boids,
						dists, NB_B);
					cohere(&boids[i],
						boids, i,
						dists, NB_B);
				}
			/*
			error = SDL_RenderDrawLines(renderer,triangle,4);
			if(error < 0)
				SDL_ExitWithError("SDL_RenderDrawLines");*/
				SDL_RenderPresent(renderer);
				//SDL_Delay(500);
				break;
			default:
				break;
		}
		//}
	}
}

