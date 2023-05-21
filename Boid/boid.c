#include "boid.h"

int WINDOW_WIDTH;
int WINDOW_HEIGHT;

#define NB_B 50
#define DELAY 1000 / NB_B
#define PI 3.141592653589793238

size_t VALID_BOID = 0;

// running :
//      0 -> STOP
//      1 -> START/RESUME
//      2 -> PAUSE
char running = 0;

// algo :
//      0 -> NO
//      1 -> algo ANT
//      2 -> algo A*
//      3 -> algo pathfinding
size_t algo = 0;
size_t obstacle = 0;

size_t perception_align = 50;
size_t perception_sep = 50;
size_t perception_coh = 50;

size_t speed_boid = 4;
size_t radius_percep = 25;

// PAINT
int PAINT = 0;
// Boolean paint
int DRAW = 0;
int ERASE = 255;

int randint(int min, int max)
{
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

void move_boid(Boid *boid, int x, int y)
{
/*    boid->tr[0].x += boid->step.x * boid->vel;
    boid->tr[0].y += boid->step.y * boid->vel;
    boid->tr[1].x += boid->step.x * boid->vel;
    boid->tr[1].y += boid->step.y * boid->vel;
    boid->tr[2].x += boid->step.x * boid->vel;
    boid->tr[2].y += boid->step.y * boid->vel;
    boid->tr[3] = boid->tr[0];
    boid->ctr.x = (boid->tr[0].x + boid->tr[1].x + boid->tr[2].x) / 3;
    boid->ctr.y = (boid->tr[0].y + boid->tr[1].y + boid->tr[2].y) / 3;
    */

    boid->tr[0].x += x;
    boid->tr[0].y += y;
    boid->tr[1].x += x;
    boid->tr[1].y += y;
    boid->tr[2].x += x;
    boid->tr[2].y += y;
    boid->tr[3] = boid->tr[0];
}

void move_boids(Boid *boid, size_t n)
{
    for(size_t i = 0; i < n; i++)
    {
        move_boid(&boid[i], 0, 0);
    }
}

void init_boids_rot(Boid *boid, size_t n)
{
    for(size_t i = 0; i < n; i++)
    {
        rotate_boid(&boid[i], randint(-180, 180));
    }
}

void wiggle_boid(Boid *boid, int min, int max)
{
    rotate_boid(boid, randint(min, max));
}

void wiggle_boids(Boid *boid, size_t n, int min, int max)
{
    for(size_t i = 0; i < n; i++)
    {
        wiggle_boid(&boid[i], min, max);
    }
}

void set_triangle(Boid *boid)
{
    boid->tr[0].x = randint(3*WINDOW_WIDTH/8, 5*WINDOW_WIDTH/8);//randint(0,WINDOW_WIDTH);
    boid->tr[0].y = randint(4*WINDOW_HEIGHT/8, 5*WINDOW_HEIGHT/8);//randint(0,WINDOW_HEIGHT);
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
    boid->pathf = NULL;
    boid->r = 0;
    boid->g = 0;
    boid->b = 255;
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

int detect_obst(Boid *B, SDL_Point boid, int *grid, int percep, int *xdist, int *ydist)
{
    int dist = -1;
    for (int y = boid.y - percep; y < boid.y + percep; y++){
        if (0 <= y && y < WINDOW_HEIGHT){
            for (int x = boid.x - percep; x < boid.x + percep; x++){
                if (0 <= x && x < WINDOW_WIDTH){
                    if (grid[y * WINDOW_WIDTH + x] == 1){
                        if (dist == -1){
                            int t1 = (boid.x - x);
                            int t2 = (boid.y - y);
                            dist = (int)sqrt(t1*t1 + t2*t2);
                            *xdist = x;
                            *ydist = y;
                        }
                        else{
                            int t1 = (boid.x - x);
                            int t2 = (boid.y - y);
                            int dist2 = (int)sqrt(t1*t1 + t2*t2);
                            if (dist2 < dist){
                                dist = dist2;
                                *xdist = x;
                                *ydist = y;
                            }
                        }
                    }
                    if (B->b != 0 && grid[y * WINDOW_WIDTH + x] == 2){
                        B->r = 255;
                        B->g = 128;
                        B->b = 0;
                        VALID_BOID += 1;
                    }
                }
            }
        }
    }
    return dist;
}

int dist_obst(Boid *boid, SDL_Point obst)
{
    int d0 = dist(boid->tr[0], obst); //front
    int d1 = dist(boid->tr[1], obst) + 10; //left
    int d2 = dist(boid->tr[2], obst) + 10; //right

    if (d0 <= d1 && d0 <= d2){
        if (d1 < d2)
            return 1;
        if (d2 < d1)
            return 2;
        return 0;
    }
    if (d1 < d2)
        return 1;
    if (d2 < d1)
        return 2;
    return -1;
}

// gestionne les obstacles et les esquive s'il est dans le rayon perception
// 1 -> obstacle detecte
// 0 -> pas d'obstacle detecte
int obst_manag(Boid *boid, int *grid, int perception)
{
    int x = 0;
    int y = 0;
    int dist = detect_obst(boid, boid->tr[0], grid, perception, &x, &y);
    if (dist != -1){
        SDL_Point obst = {x, y};
        int dir = dist_obst(boid, obst);
        if(dir != -1)
        {
            int direction = (dir == 2) ? 1 : -1;

            int alpha;// = 90 - (2 * dist) / 3;
            if (dist <= 5){
                alpha = 90;
            }
            else{
                if (dist <= 10){
                    alpha = 25;
                }
                else if (dist <= 20){
                    alpha = 10;
                }
                else{
                    alpha = 3;
                }
            }

            boid->oth = new_th(boid->oth, alpha * direction);
        }
        return 1;
    }
    return 0;
}

/////////////////////////////////////

void wrap_thr_edge(Boid *boid)
{
    if(boid->ctr.x > WINDOW_WIDTH){
        boid->ctr.x = WINDOW_WIDTH - 9;
    }else if (boid->ctr.x < 0){
        boid->ctr.x = 9;
    }
    if(boid->ctr.y > WINDOW_HEIGHT){
        boid->ctr.y = WINDOW_HEIGHT - 9;
    }
    else if (boid->ctr.y < 0){
        boid->ctr.y = 9;
    }
}

void wrap_thr_edges(Boid *boids, size_t n)
{
    for(size_t i = 0; i < n; i++)
    {
        wrap_thr_edge(&boids[i]);
    }
}

//////////////////////////////////////

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
        if (boid->b != 0 && dists[i] <= radius_percep && boids[i].b == 0){
            boid->r = boids[i].r;
            boid->g = boids[i].g;
            boid->b = boids[i].b;
            VALID_BOID += 1;
        }
    }
}

int new_th(int x, int a)
{
    int y = x + a;
    if(y >= 180)
    {
        y = -(180 - (y % 180)) / 3;
    }
    else if(y <= -180)
    {
        y = (180 - ((-y) % 180)) / 3;
    }
    return y;
}

//////////////////////////////////////


void align(Boid *boid, Boid *boids, size_t id, size_t *dists, size_t n)
{
    int dir;
    for(size_t i = 0; i < n; i++)
    {
        dir = detect_point(boid, boids[i].ctr);
        if((i != id) && (dists[i] <= perception_align) && (dir != 0))
        {
            if(dir == 1)
                boid->oth = new_th(boid->oth, +speed_boid);
            else if(dir == -1)
                boid->oth = new_th(boid->oth, -speed_boid);
        }
    }
}
void set_coef_align(size_t coef)
{
    perception_align = coef;
}

void separate(Boid *boid, Boid *boids, size_t id, size_t *dists, size_t n)
{
    int dir;
    for(size_t i = 0; i < n; i++)
    {
        dir = detect_point(boid, boids[i].ctr);
        if((i != id) && (dists[i] <= perception_sep) && (dir != 0))
        {
            if(dir == 1)
                boid->oth = new_th(boid->oth, -speed_boid);
            else if(dir == -1)
                boid->oth = new_th(boid->oth, +speed_boid);
        }
    }
}
void set_coef_sep(size_t coef)
{
    perception_sep = coef;
}

void cohere(Boid *boid, Boid *boids, size_t id, size_t *dists, size_t n)
{
    int dir, count;
    SDL_Point point;
    count = 0;
    for(size_t i = 0; i < n; i++)
    {
        dir = detect_point(boid, boids[i].ctr);
        if((i != id) && (dists[i] <= perception_coh) && (dir != 0))
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
        boid->oth = new_th(boid->oth, +speed_boid);
    else if(dir == -1)
        boid->oth = new_th(boid->oth, -speed_boid);
}
void set_coef_coh(size_t coef)
{
    perception_coh = coef;
}


void boid_behavior(Boid* boids, size_t i, int* grid, size_t* dists)
{
    boids[i].pathf = NULL;
    if (!obst_manag(&boids[i], grid, 2*radius_percep/3)){
        set_dists(&boids[i], boids, dists, NB_B);
        separate(&boids[i], boids, i, dists, NB_B);
        cohere(&boids[i], boids, i, dists, NB_B);
        align(&boids[i], boids, i, dists, NB_B);
    }
}
void boids_behavior(Boid* boids, int* grid, size_t* dists)
{
    for(size_t i = 0; i < NB_B; i++)
    {
        boid_behavior(boids, i, grid, dists);
    }
}

void pathfinding_behavior(Boid* boid, int* grid, int* x_pos, int* y_pos, int status)
{
    if (status == 3 || (status == 2 && boid->r != 255)){
        int depth = 1;
        if (boid->pathf == NULL){
            boid->pathf = A_star(boid->ctr.x,
                    boid->ctr.y, *x_pos, *y_pos, 500, grid,
                    WINDOW_WIDTH, WINDOW_HEIGHT);
        }
        else if (DELAY >= 10)
            SDL_Delay(DELAY/NB_B);

        while (boid->pathf != NULL && (
                    abs(boid->pathf->x) > WINDOW_WIDTH ||
                    abs(boid->pathf->y) > WINDOW_HEIGHT)){
             boid->pathf = boid->pathf->next;
        }

        if (boid->pathf != NULL &&
                abs(boid->pathf->x - boid->ctr.x) < 5 &&
                abs(boid->pathf->y - boid->ctr.y) < 5 &&
                !(abs(boid->ctr.x-(*x_pos)) < 10 && abs(boid->ctr.y-(*y_pos)) < 10)){
            while (depth > 0 && boid->pathf != NULL){
                boid->pathf = boid->pathf->next;
                depth--;
            }
        }

        if (boid->pathf != NULL &&
                !(abs(boid->ctr.x-(*x_pos)) <= 10 &&
                abs(boid->ctr.y-(*y_pos)) <= 10)){
            int x_path = boid->pathf->x;
            int y_path = boid->pathf->y;
            int x_t = abs(boid->ctr.x-x_path);
            int y_t = abs(boid->ctr.y-y_path);
            int deg = (int)(acos(x_t/sqrt(x_t*x_t + y_t*y_t))*180/PI);
            deg = (boid->ctr.y < y_path) ? 90 + deg : 90 - deg;
            deg = (deg == 0) ? 1 : deg;
            deg *= (boid->ctr.x < x_path) ? 1 : -1;
            boid->oth = deg;
        }
        else {
            boid->pathf = NULL;
            *x_pos = -1;
            *y_pos = -1;
        }
    }
}

void search_path(Boid* boid, int* grid, int* pathx, int* pathy,
        size_t grid_pattern, int* x, int* y)
{
    int distance = WINDOW_WIDTH * WINDOW_HEIGHT;
    for (size_t i = 0; i < grid_pattern; i++){
        for (size_t j = 0; j < grid_pattern; j++){
            int pos = i * grid_pattern + j;
            if (pathx[pos] != -1 && pathy[pos] != -1){
                if (grid[pathy[pos] * WINDOW_WIDTH + pathx[pos]] != 1){
                    int distx = abs(boid->ctr.x - pathx[pos]);
                    int disty = abs(boid->ctr.y - pathy[pos]);
                    int dist = (int)sqrt(distx*distx + disty*disty);
                    if (dist < distance){
                        *x = (int)j;
                        *y = (int)i;
                        distance = dist;
                    }
                }
                else{
                    pathx[pos] = -1;
                    pathy[pos] = -1;
                }
            }
        }
    }
    if (distance > 1000){
        *x = -1;
        *y = -1;
    }
}
void algo_search(Boid* boid, int* grid, int* path_boids, int* pathx, int* pathy,
        int x, int y, int grid_pattern, size_t i)
{
    int pos = y * grid_pattern + x;
    x = pathx[pos];
    y = pathy[pos];

    obst_manag(boid, grid, radius_percep);
    pathfinding_behavior(boid, grid, &x, &y, algo);

    if (x == -1){
        pathx[pos] = -1;
        pathy[pos] = -1;
        path_boids[i*2] = -1;
        path_boids[i*2 + 1] = -1;
    }
}

/////////////////////////////////////

// draw_b boolean -> 1 si ce n'est pas un recouvrement un blanc, 0 sinon
void draw_boid(SDL_Renderer *renderer, Boid *boid, int draw_b)
{
    if (draw_b)
        SDL_SetRenderDrawColor(renderer, boid->r, boid->g, boid->b, 255);
    else
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    if(SDL_RenderDrawLines(renderer,boid->tr,4))
        SDL_ExitWithError("SDL_RenderDrawLines");
}
// input
// 0 -> case libre
// 1 -> obstacle
// 2 -> cible
// -2 -> efface la cible
void draw_obst(int *grid, int X, int Y, int radius, int input)
{
    for (int y = Y - radius; y < Y + radius; y++){
        if (2 <= y && y < WINDOW_HEIGHT-2){
            for (int x = X - radius; x < X + radius; x++){
                if (2 <= x && x < WINDOW_WIDTH){
                    if (grid[y * WINDOW_WIDTH + x] != 2 || input == 2){
                        grid[y * WINDOW_WIDTH + x] = input;
                    }
                    else if (input == -2){
                        grid[y * WINDOW_WIDTH + x] = 0;
                    }
                }
            }
        }
    }
}

/////////////////////////////////////

void init_path(int* pathx, int* pathy, size_t grid_pattern)
{
    pathx[0] = WINDOW_WIDTH / (grid_pattern * 2);
    pathy[0] = WINDOW_HEIGHT / (grid_pattern * 2);
    for (size_t i = 1; i < grid_pattern; i++){
        pathx[i] = pathx[i-1] + WINDOW_WIDTH / grid_pattern;
        pathy[i * grid_pattern] = pathy[(i-1) * grid_pattern] + WINDOW_HEIGHT / grid_pattern;
    }
    for (size_t i = 1; i < grid_pattern; i++){
        for (size_t j = 0; j < grid_pattern; j++){
            pathx[i * grid_pattern + j] = pathx[(i-1) * grid_pattern + j];
            pathy[j * grid_pattern + i] = pathy[j * grid_pattern + i-1];
        }
    }
}

void init_boids(Boid *boid, size_t n)
{
    for(size_t i = 0; i < n; i++)
    {
        init_boid(&boid[i]);
    }
}

void draw_boids(SDL_Renderer *renderer, Boid *boid, size_t n, int draw_b)
{
    for(size_t i = 0; i < n; i++)
    {
        draw_boid(renderer, &boid[i], draw_b);
    }
}

void SDL_ExitWithError(const char *message)
{
    SDL_Log("ERROR: %s > %s\n",
            message,
            SDL_GetError());
    SDL_Quit();
}


void set_active_obstacle(size_t set)
{
    obstacle = set;
}
void set_active_algo(size_t set)
{
    algo = set;
}
void set_run(char run)
{
    running = run;
}
void main_loop(SDL_Renderer *renderer, int Window_Width, int Window_Height)
{
    WINDOW_WIDTH = Window_Width;
    WINDOW_HEIGHT = Window_Height;

    int ray_paint = 20;
    int ray_erase = 30;
    int x_pos = -1;
    int y_pos = -1;

    srand(time(NULL));
    Boid boids[NB_B];
    size_t dists[NB_B];

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Rect of the target
    SDL_Rect target = {0, 0, 20, 20};
    drawtarget(renderer, &target, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Initialisation de la grid obstacle (qui contient la position des
    // obstacles) 
    // placer des obstcales aux limites de la fenêtre
    // et placer la target
    int* grid = malloc(sizeof(int) * WINDOW_WIDTH * WINDOW_HEIGHT);
    for (int y = 0; y < WINDOW_HEIGHT; y++){
        grid[y * WINDOW_WIDTH] = 1;
        grid[y * WINDOW_WIDTH + 1] = 1;
        for (int x = 2; x < WINDOW_WIDTH-2; x++){
            grid[y * WINDOW_WIDTH + x] = 0;
            if (y <= 1 || y >= WINDOW_HEIGHT-2)
                grid[y * WINDOW_WIDTH + x] = 1;
        }
        grid[y * WINDOW_WIDTH + WINDOW_WIDTH-2] = 1;
        grid[y * WINDOW_WIDTH + WINDOW_WIDTH-1] = 1;
    }
    draw_obst(grid, target.x+target.w/2, target.y+target.w/2, target.w/2, 2);

    // Initialisation des deux listes pathx/pathy qui sont les différentes
    // coordonnées que les boids vont devoir aller pour algo == 2
    size_t grid_pattern = 20;
    int* pathx = malloc(sizeof(int) * grid_pattern * grid_pattern);
    int* pathy = malloc(sizeof(int) * grid_pattern * grid_pattern);
    init_path(pathx, pathy, grid_pattern);

    int* path_boids = malloc(sizeof(int) * 2 * NB_B);
    for (size_t i = 0; i < 2 * NB_B; i++){
        path_boids[i] = -1;
    }

    // Initialisation des boids
    init_boids(boids, NB_B);
    SDL_Event event;

    while(0 < running)
    {
        SDL_WaitEventTimeout(&event, 0);
        switch (event.type)
        {
            case SDL_QUIT:
                free(path_boids);
                free(pathx);
                free(pathy);
                free(grid);
                return;
            case SDL_KEYUP:
                draw_obst(grid, target.x+target.w/2, target.y+target.w/2, target.w/2, -2);
                drawtarget(renderer, &target, WINDOW_WIDTH, WINDOW_HEIGHT);
                draw_obst(grid, target.x+target.w/2, target.y+target.w/2, target.w/2, 2);
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (obstacle){
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        if (drawrect(renderer, DRAW, event.motion.x, event.motion.y,
                                    ray_paint, &target)){
                            draw_obst(grid, event.motion.x, event.motion.y,
                                    2*ray_paint/3, 1);
                        }
                        PAINT = 1;
                    }
                    else if (event.button.button == SDL_BUTTON_RIGHT)
                    {
                        if (drawrect(renderer, ERASE, event.motion.x, event.motion.y,
                                    ray_erase, &target)){
                            draw_obst(grid, event.motion.x, event.motion.y,
                                    ray_erase/2, 0);
                        }
                        PAINT = -1;
                    }
                    x_pos = -1;
                    y_pos = -1;
                }
                else
                {
                    x_pos = event.motion.x;
                    y_pos = event.motion.y;
                    printf("%i -- x:%i -- y:%i\n",
                            grid[y_pos * WINDOW_WIDTH + x_pos], x_pos, y_pos);
                }
                //printf("x:%i -- y:%i\n", x_pos, y_pos);
                break;
            case SDL_MOUSEBUTTONUP:
                PAINT = 0;
                break;
                // If the mouse is moving, updates the position of the cursor.
            case SDL_MOUSEMOTION:
                if (obstacle)
                {
                    if (PAINT == 1)
                    {
                        if (drawrect(renderer, DRAW, event.motion.x,
                                    event.motion.y, ray_paint, &target)){
                            draw_obst(grid, event.motion.x,
                                    event.motion.y, 2*ray_paint/3, 1);
                        }
                    }
                    else if (PAINT == -1)
                    {
                        if (drawrect(renderer, ERASE, event.motion.x,
                                    event.motion.y, ray_erase, &target)){
                            draw_obst(grid, event.motion.x, event.motion.y,
                                    ray_erase/2, 0);
                        }
                    }
                }
                break;
            //default:
              //  break;
        }
        // running == 1 -> Start/Resume
        if (running == 1)
        {
            draw_boids(renderer, boids, NB_B, 0);
            refresh_boids_rot(boids, NB_B);
            wrap_thr_edges(boids, NB_B);

            int delay_algo2 = 0;
            if (algo == 0 || algo == 1 ||
                   (algo == 3 && x_pos == -1 && y_pos == -1)){
                x_pos = -1;
                y_pos = -1;
                if (DELAY >= 10)
                    SDL_Delay(DELAY);

                wiggle_boids(boids, NB_B, -3, +3);
                boids_behavior(boids, grid, dists);
            }
            else if (algo == 2){
                for (size_t i = 0; i < NB_B; i++){
                    int x = -1;
                    int y = -1;
                    if (path_boids[i*2] == -1 && boids[i].r != 255){
                        search_path(&boids[i], grid, pathx, pathy, grid_pattern, &x, &y);
                    }

                    if (x != -1 && y != -1){
                        path_boids[i*2] = x;
                        path_boids[i*2 + 1] = y;
                        algo_search(&boids[i], grid, path_boids, pathx, pathy,
                                x, y, grid_pattern, i);
                        set_dists(&boids[i], boids, dists, NB_B);
                    }
                    else if (path_boids[i*2] != -1){
                        x = path_boids[i*2];
                        y = path_boids[i*2 + 1];
                        algo_search(&boids[i], grid, path_boids, pathx, pathy,
                                x, y, grid_pattern, i);
                        set_dists(&boids[i], boids, dists, NB_B);
                    }
                    else{
                        boids[i].pathf = NULL;
                        delay_algo2++;
                        /*if (DELAY >= 10){
                            if (DELAY / NB_B == 0)
                                SDL_Delay(0);
                            else
                                SDL_Delay(DELAY/NB_B);
                            //printf("DELAY  %i  DELAY\n", DELAY / NB_B);
                        }*/
                        wiggle_boid(&boids[i], -3, +3);
                        boid_behavior(boids, i, grid, dists);
                    }

                }

            }
                /*for (size_t i = 0; i < grid_pattern; i++){
                    for (size_t j = 0; j < grid_pattern; j++){
                        printf("%i--", pathx[i * grid_pattern + j]);
                    }
                    printf("\n");
                }
                printf("\n");*/
            else if (algo == 3 && x_pos != -1 && y_pos != -1){
                for (size_t i = 0; i < NB_B; i++){
                    set_dists(&boids[i], boids, dists, NB_B);
                    obst_manag(&boids[i], grid, radius_percep);
                    pathfinding_behavior(&boids[i], grid, &x_pos, &y_pos, algo);
                }
            }
            if (delay_algo2 > (2*NB_B) / 3){
                algo = 0;
            }

            draw_boids(renderer, boids, NB_B, 1);
            SDL_RenderPresent(renderer);
        }
        else if (algo == 0 || algo == 1){ //algo !=algo != 3 || x_pos == -1 || y_pos == -1){
            for (size_t i = 0; i < NB_B; i++){
                boids[i].pathf = NULL;
            }
            x_pos = -1;
            y_pos = -1;
        }
    }
    free(path_boids);
    free(pathx);
    free(pathy);
    free(grid);
}
