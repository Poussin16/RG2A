#include"my_data.h"

#define WINDOW_WIDTH 1100
#define WINDOW_HIGHT 600
#define FIXED 1
#define NB_AGENTS 100
#define NB_ATTR 10
#define NB_EDGES 3
#define random(MIN, MAX) ((MAX - MIN) * ((float)rand() / RAND_MAX)) + MIN
#define FREEDOM 3

#define PI 3.14159265358979323846264338327950288419716939937510582097

const long double PIon180 = PI / 180;
const long double invPIon180 = 180 / PI;
char end_of_prog = 0;
const size_t nb_agents = NB_AGENTS;
const size_t nb_edges = NB_EDGES;
size_t act_agents = NB_AGENTS;
size_t used_edges = NB_EDGES;
int scale = 2;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Rect rect;
SDL_Event event;

long double agent[NB_AGENTS];
char blocked[NB_AGENTS];
char block_count[NB_AGENTS];
char leaning[NB_AGENTS];
long ID[NB_AGENTS];
long freedom[NB_AGENTS];
long double X[NB_AGENTS];
long double Y[NB_AGENTS];
long double DX[NB_AGENTS];
long double DY[NB_AGENTS];
long double VX[NB_AGENTS];
long double VY[NB_AGENTS];
long double AX[NB_AGENTS];
long double AY[NB_AGENTS];
long double theta[NB_AGENTS];
unsigned int Colors[NB_AGENTS];
long_double2 shape[3] = {{.x = 0, .y = 3},
						{.x = -2, .y = -2},
						{.x = 2, .y = -2}};

long grid[WINDOW_HIGHT+1][WINDOW_WIDTH+1];

long double pxf[NB_AGENTS];
long double pyf[NB_AGENTS];
long double pxr[NB_AGENTS];
long double pyr[NB_AGENTS];
long double pxl[NB_AGENTS];
long double pyl[NB_AGENTS];

double align_rad = 100;
double separate_rad = 90;
double cohere_rad = 90;

/*
long double alignment_VX[NB_AGENTS];
long double alignment_VY[NB_AGENTS];
long double alignment_THX[NB_AGENTS];
long double alignment_THY[NB_AGENTS];
long double separation[NB_AGENTS];
long double cohesion[NB_AGENTS];*/

///////////////////////////////////////////////////////

SDL_Point coord(double x, double y)
{
	SDL_Point tmp;
	tmp.x = x;
	tmp.y = y;
	return tmp;
}

///////////////////////////////////////////////////////

void Init()
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
		ExitWithError("SDL_Init");
	window = SDL_CreateWindow("Boids",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			WINDOW_WIDTH,
			WINDOW_HIGHT,
			(SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED)&&!FIXED);
	if(window == NULL)
		ExitWithError("SDL_CreateWindow");

	renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED);
	if(renderer == NULL)
		ExitWithError("SDL_CreateRenderer");

	rect.x = 0;
	rect.y = 0;
	rect.h = WINDOW_HIGHT;
	rect.w = WINDOW_WIDTH;
}

void InitGrid()
{
	for(size_t j = 0; j < WINDOW_WIDTH; j++)
	{
		grid[0][j] = -1;
	}
	for(size_t i = 1; i < WINDOW_HIGHT; i++)
	{
		grid[i][0] = -1;
		for (size_t j = 1; j < WINDOW_WIDTH; j++)
		{
			grid[i][j] = -2;
		}
		grid[i][WINDOW_WIDTH] = -1;
	}
	for(size_t j = 0; j < WINDOW_WIDTH; j++)
	{
		grid[WINDOW_HIGHT][j] = -1;
	}
}

void Start()
{
	InitGrid();
    for(size_t i = 0; i < act_agents; i++)
	{
		blocked[i] = 0;
		block_count[i] = 0;
		freedom[i] = random(0,FREEDOM);
		leaning[i] = ((char)random(0,1) == 0) ? 1 : -1;
		ID[i] = i;
		do{
			X[i] = random(60,WINDOW_WIDTH-50);
			Y[i] = random(60,WINDOW_HIGHT-50);
			//printf("passed\n");
		}while(grid[(int)Y[i]][(int)X[i]] == -1);
		grid[(int)Y[i]][(int)X[i]] = (long)i;
		//printf("%d\n", grid[(int)Y[i]][(int)X[i]]);
		DX[i] = 0;
		DY[i] = 1;
		VX[i] = random(1,4);
		VY[i] = VX[i];
		AX[i] = 0;
		AY[i] = 0;
		theta[i] = random(-180,180);
	}
}

void Loop()
{
	char running = 1;
	char paused = 1;
	while(running)
	{
		while(SDL_PollEvent(&event)
			&& 
			!SDL_TICKS_PASSED(SDL_GetTicks()
					,SDL_GetTicks()+10))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						running = 0;
						break;
					case SDLK_p:
						paused = !paused;
						break;
					case SDLK_SPACE:
						goto loop_despite_pause;
						break;
					default:
						break;
				}
				break;
			case SDL_QUIT:
                running = 0;
			default:
				break;
			}
			
		}
		if(paused)
			continue;
		loop_despite_pause:
		SDL_SetRenderDrawColor(renderer, 0x1B, 0x1C, 0x22, 0xFF);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0xDD, 0xF0, 0xE4, 0xFF);
		Update();
		SDL_RenderPresent(renderer);
		SDL_Delay(50);
	}
}

/////////////////////////////////////////////////////////////

static inline long double nextX(size_t i)
{
	return X[i] + VX[i]*DX[i] + random(0,1);
}

static inline long double nextY(size_t i)
{
	return Y[i] + VY[i]*DY[i] + random(0,1);
}

static inline long double nextVX(size_t i)
{
	return VX[i] + AX[i] + random(-0.5,0.5);
}

static inline long double nextVY(size_t i)
{
	return VY[i] + AY[i] + random(-0.5,0.5);
}

void CheckTheta()
{
	for (size_t i = 0; i < act_agents; i++)
	{
		theta[i] = theta[i] >= 0 ? fmodl(theta[i], 360) : -(fmodl((-theta[i]), 360));
	}
}

void UpdateGrid(size_t n)
{
	if(0 <= Y[n] && Y[n] <= WINDOW_HIGHT
		&& 0 <= X[n] && X[n] <= WINDOW_WIDTH)
	{
		if(grid[(int)(Y[n]+DY[n])]
				[(int)(X[n]+DX[n])] != -2)
		{
			blocked[n] = 1;
			block_count[n]+=2;
			VX[n]--;
			VY[n]--;
		}
		else
		{
			grid[(int)Y[n]][(int)X[n]] = ID[n];
			blocked[n] = 0;
			block_count[n] = block_count[n] > 0 ? (block_count[n]-1) : 0;
		}
	}
	else
	{
		Y[n] = Y[n] < 0 ? 0 : Y[n];
		Y[n] = Y[n] > WINDOW_HIGHT ? WINDOW_HIGHT : Y[n];
		X[n] = X[n] < 0 ? 0 : X[n];
		X[n] = X[n] > WINDOW_WIDTH ? WINDOW_WIDTH : X[n];
		blocked[n] = 1;
		block_count[n]+=2;
		VX[n]--;
		VY[n]--;
	}
	
	if(0 <= Y[n] && Y[n] <= WINDOW_HIGHT
		&& 0 <= X[n] && X[n] <= WINDOW_WIDTH)
	{
		grid[(int)Y[n]][(int)X[n]] = ID[n];
	}
	
}

void dir_agent(size_t i, float deg)
{
	long double sin_th, cos_th;
	float rad = deg*PIon180;
	sin_th = sinl(rad);
	cos_th = cosl(rad);
	DX[i] = sin_th;
	DY[i] = cos_th;
}

void move_agent(size_t i)
{
	//SDL_RenderDrawLine(renderer, X[i], Y[i], 
	//					X[i] + 10*DX[i],
	//					Y[i] + 10*DY[i]);
	/*if(0 <= Y[i] && Y[i] <= WINDOW_HIGHT
			&& 0 <= X[i] && X[i] <= WINDOW_WIDTH)
	{
		grid[(int)Y[i]][(int)X[i]] = (long)ID[i];
	}*/
	if(VX[i] < 0)
		VX[i] = -VX[i];
	if(VY[i] < 0)
		VY[i] = -VY[i];
	X[i] = nextX(i);
	Y[i] = nextY(i);
	VX[i] = nextVX(i)*10/10;
	VY[i] = nextVY(i)*10/10;
	AX[i] /= 2;
	AY[i] /= 2;
}

void rot_agent(size_t i, float deg)
{
	long double sin_th, cos_th;
	float rad = deg*PIon180;
	sin_th = sinl(rad);
	cos_th = cosl(rad);
	pxf[i] = X[i] + cos_th * scale * shape[0].x
				+ sin_th * scale * shape[0].y;
	pyf[i] = Y[i] + cos_th * scale * shape[0].y
				- sin_th * scale * shape[0].x;
	pxr[i] = X[i] + cos_th * scale * shape[1].x
				+ sin_th * scale * shape[1].y;
	pyr[i] = Y[i] + cos_th * scale * shape[1].y
				- sin_th * scale * shape[1].x;
	pxl[i] = X[i] + cos_th * scale * shape[2].x
				+ sin_th * scale * shape[2].y;
	pyl[i] = Y[i] + cos_th * scale * shape[2].y
				- sin_th * scale * shape[2].x;
}

void resolve_blockage()
{
	for (size_t i = 0; i < act_agents; i++)
	{
		if(blocked[i])
		{
			if(block_count[i] > 10)
				leaning[i] = -leaning[i];
			if(block_count[i] > 20)
			{
				theta[i] += leaning[i] * random(0,180);
			}
			else
			{
				theta[i] += leaning[i] * random(0,block_count[i]);
			}
			//leaning[i] = ((char)random(0,1) == 0) ? 1 : -1;
			dir_agent(i, theta[i]);
		}
	}
}
////////////////////////////////////////////////////////////////
//int tmp = 2147483640;
void Update()
{
	for(size_t i = 0; i < act_agents; i++)
	{
		SDL_RenderDrawLine(renderer, pxf[i], pyf[i], pxr[i], pyr[i]);
		SDL_RenderDrawLine(renderer, pxr[i], pyr[i], pxl[i], pyl[i]);
		SDL_RenderDrawLine(renderer, pxl[i], pyl[i], pxf[i], pyf[i]);
	}
	for(size_t i = 0; i < act_agents; i++)
	{
		wiggle_agent(i);
		dir_agent(i, theta[i]);
		rot_agent(i, theta[i]);
		move_agent(i);
		UpdateGrid(i);
		follow_agent(i);
	}
	CheckTheta();
	resolve_blockage();
	SDL_RenderDrawRect(renderer, &rect);
	/*for(size_t i = 1; i < WINDOW_HIGHT; i++)
	{
		for (size_t j = 1; j < WINDOW_WIDTH; j++)
		{

			if(0 <= grid[i][j])
			{
				SDL_RenderDrawPoint(renderer,X[grid[i][j]],Y[grid[i][j]]);
			}
		}	
	}*/
	for(size_t i = 1; i < WINDOW_HIGHT; i++)
	{
		for (size_t j = 1; j < WINDOW_WIDTH; j++)
		{

			if(0 <= grid[i][j])
			{
				grid[i][j] = -2;
			}
		}	
	}
	//tmp++;
	//tmp = tmp >= 0 ? tmp % 360 : -((-tmp) % 360);
	//printf("%d\n", tmp);
}
/////////////////////////////////////////////////////////




void wiggle_agent(size_t i)
{
	theta[i] += random(-1,1) * freedom[i];
}

static inline double dist(double x1, double y1, double x2, double y2)
{
	return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

void follow_agent(size_t n)
{
	double perc_rad = align_rad;
	if(separate_rad > perc_rad)
		perc_rad = separate_rad;
	if(cohere_rad > perc_rad)
		perc_rad = cohere_rad;

	long i_init, j_init, row, col;
	i_init = (size_t)(Y[n] - perc_rad);
	j_init = (size_t)(X[n] - perc_rad);
	i_init = (i_init < 0) ? 0 : i_init;
	j_init = (j_init < 0) ? 0 : j_init;
	col = (size_t)(X[n] + perc_rad);
	row = (size_t)(Y[n] + perc_rad);
	col = (col <= WINDOW_WIDTH) ? col : WINDOW_WIDTH;
	row = (row <= WINDOW_HIGHT) ? row : WINDOW_HIGHT;

	long id, nb;
	double d, df, dr, dl, x_acc, y_acc;
	nb = 0;
	for (int i = i_init; i <= row; i++)
	{
		for(int j = j_init; j <= col; j++)
		{
			id = grid[i][j];
			if(id >= 0 && (size_t)id != n)
			{
				d = dist((long)X[n],(long)Y[n],(long)X[id],(long)Y[id]);
				//printf("%d\n",id);
				if(d <= align_rad)
				{
					df = dist((long)pxf[n],(long)pyf[n],(long)X[id],(long)Y[id]);
					dr = dist((long)pxr[n],(long)pyr[n],(long)X[id],(long)Y[id]);
					dl = dist((long)pxl[n],(long)pyl[n],(long)X[id],(long)Y[id]);
					if(dr > dl)
						theta[n] += 6;
					else
						theta[n] -= 6;
					if(df > d)
					{
						AX[n] += 0.5;
						AY[n] += 0.5;
					}
					else
					{
						AX[n] -= 0.5;
						AY[n] -= 0.5;
					}
					SDL_SetRenderDrawColor(renderer, 0x40, 0x8E, 0x91, 0x33);
					SDL_RenderDrawLine(renderer,X[n],Y[n],X[id],Y[id]);
					//printf("%d detected %d\n",n,id);
				}
				if(d <= separate_rad)
				{
					df = dist((long)pxf[n],(long)pyf[n],(long)X[id],(long)Y[id]);
					dr = dist((long)pxr[n],(long)pyr[n],(long)X[id],(long)Y[id]);
					dl = dist((long)pxl[n],(long)pyl[n],(long)X[id],(long)Y[id]);
					if(dr < dl)
						theta[n] += 6;
					else
						theta[n] -= 6;
					if(df < d)
					{
						AX[n] += 0.5;
						AY[n] += 0.5;
					}
					else
					{
						AX[n] -= 0.5;
						AY[n] -= 0.5;
					}
					//SDL_SetRenderDrawColor(renderer, 0xE4, 0x93, 0x93, 0x33);
					//SDL_RenderDrawLine(renderer,X[n],Y[n],X[id],Y[id]);
				}
				if(d <= cohere_rad)
				{
					x_acc += X[n];
					y_acc += Y[n];
					nb++;
				}
			}
		}
	}
	if(nb > 0)
	{
		x_acc /= nb+1;
		y_acc /= nb+1;

		df = dist((long)pxf[n],(long)pyf[n],(long)x_acc,(long)y_acc);
		dr = dist((long)pxr[n],(long)pyr[n],(long)x_acc,(long)y_acc);
		dl = dist((long)pxl[n],(long)pyl[n],(long)x_acc,(long)y_acc);

		if(dr > dl)
			theta[n] += 6;
		else
			theta[n] -= 6;
		if(df > d)
		{
			AX[n] += 0.5;
			AY[n] += 0.5;
		}
		else
		{
			AX[n] -= 0.5;
			AY[n] -= 0.5;
		}
		//SDL_SetRenderDrawColor(renderer, 0xD8, 0xD8, 0xD8, 0x33);
		//SDL_RenderDrawLine(renderer,X[n],Y[n],x_acc,y_acc);
	}
	SDL_SetRenderDrawColor(renderer, 0x1B, 0x1C, 0x22, 0xFF);
}

/*
void align_add(size_t n, size_t id)
{
	long double sin_th, cos_th;
	float rad = theta[id]*PIon180;
	sin_th = sinl(rad);
	cos_th = cosl(rad);
	alignment_VX[n] += VX[id]*sin_th;
	alignment_VY[n] += VY[id]*cos_th;
	alignment_THX[n] += sin(theta[id]*PIon180);
	alignment_THY[n] += cos(theta[id]*PIon180);
}

void align_sub(size_t n, size_t id)
{
	long double sin_th, cos_th;
	float rad = theta[id]*PIon180;
	sin_th = sinl(rad);
	cos_th = cosl(rad);
	alignment_VX[n] -= VX[id]*sin_th;
	alignment_VY[n] -= VY[id]*cos_th;
	alignment_THX[n] -= sin(theta[id]*PIon180);
	alignment_THY[n] -= cos(theta[id]*PIon180);
}

void align_div(size_t n, size_t nb)
{
	alignment_VX[n] /= nb;
	alignment_VY[n] /= nb;
	alignment_THX[n] /= nb;
	alignment_THY[n] /= nb;
}

void align_rule(size_t n)
{
	AX[n] += alignment_VX[n];
	AY[n] += alignment_VY[n];
	theta[n] = atan2(alignment_THX[n]+sin(theta[n]*PIon180),
				alignment_THY[n]+cos(theta[n]*PIon180)) * invPIon180;
	printf("(%d,%d)\n",(int)VX[n],(int)VY[n]);
}
*/
//SDL_RenderDrawLine(renderer,X[n],Y[n],X[id],Y[id]);
/////////////////////////////////////////////////////////

void Kill()
{
	if(end_of_prog == 1)
		return;
	end_of_prog = 1;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void ExitWithError(const char *message)
{
	SDL_Log("ERROR: %s > %s\n",
			message,
			SDL_GetError());
	Kill();
}
