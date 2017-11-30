/* gameData.h serves as an easy space to modify key variables in theCave. it
   is used by theCave.c */

// map info
#define MAX_X 49
#define MAX_Y 49
#define TILE_SIZE 20

// for control info
#define LEFT  1
#define UP    2
#define DOWN  3
#define RIGHT 4
#define STOP  0

// maximum allowed distance from the player before the demon starts moving towards the player
#define PROX_LIMIT 10

// struct for the game nodes below
typedef struct game_node {
    int x;
    int y;
} node;

// player and son
node player;
node boy;
// demons
node demonGuard;
node demon1;
node demon2;
node demon3;
node demon4;
// to paint over old locations
node oldplayer;
node olddemonGuard;
node olddemon1;
node olddemon2;
node olddemon3;
node olddemon4;
// wall node (not a specific wall node, this is used to draw all the walls)
node wall;

// variables for the game needed by many functions
int dir;
bool eaten;
bool boyIsFound;
int number;
bool win = false; // initialise false and only make true when you've won


SDL_Renderer* renderer = NULL;

SDL_Surface*  player_surface = NULL;
SDL_Surface*  boy_surface = NULL;
SDL_Surface*  demon_surface = NULL;
SDL_Surface*  demon_guard_surface = NULL;
SDL_Surface*  floor_surface = NULL;
SDL_Surface*  wall_surface = NULL;

SDL_Texture*  player_texture = NULL;
SDL_Texture*  boy_texture = NULL;
SDL_Texture*  demon_texture = NULL;
SDL_Texture*  demon_guard_texture = NULL;
SDL_Texture*  floor_texture = NULL;
SDL_Texture*  wall_texture = NULL;
