#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "gameData.h"
#include "gameFunctions.h"
#include "controlFunctions.h"
#include "map.h"

// initialise the game data
void init(char array[MAX_Y][MAX_X]) {
    SDL_Window *window = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());

    atexit(SDL_Quit); // set for clean-up on exit

    SDL_CreateWindowAndRenderer(MAX_Y * TILE_SIZE, MAX_X * TILE_SIZE, 0, &window, &renderer);
    SDL_SetWindowTitle( window, "Enter the Cave...");
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    player_surface = SDL_LoadBMP("images/player.bmp");
    boy_surface = SDL_LoadBMP("images/child.bmp");
    demon_guard_surface = SDL_LoadBMP("images/demonGuard.bmp");
    demon_surface = SDL_LoadBMP("images/demon.bmp");
    floor_surface = SDL_LoadBMP("images/floor.bmp");
    wall_surface = SDL_LoadBMP("images/wall.bmp");

    player_texture = SDL_CreateTextureFromSurface(renderer, player_surface);
    boy_texture = SDL_CreateTextureFromSurface(renderer, boy_surface);
    demon_texture = SDL_CreateTextureFromSurface(renderer, demon_surface);
    demon_guard_texture = SDL_CreateTextureFromSurface(renderer, demon_guard_surface);
    floor_texture = SDL_CreateTextureFromSurface(renderer, floor_surface);
    wall_texture = SDL_CreateTextureFromSurface(renderer, wall_surface);

    // draw wall tiles where there is a wall in the array
    for (int y = 0; y < MAX_Y; y++) {
      for (int x = 0; x < MAX_X; x++) {
        wall.x = x;
        wall.y = y;
        if (array[y][x] == WALL) {
          draw_object(wall, wall_texture); // draw a wall there
        } else {
          draw_object(wall, floor_texture); // draw a floor there
        }
      }
    }

    placeNode (&demon1, array);
    placeNode (&demon2, array);
    placeNode (&demon3, array);
    placeNode (&demon4, array);

    // place boy and set his square in the array to be inaccessible to demons
    // also make sure he's not in the same quarter of the map as the player
    do {placeNode (&boy, array);} while (boy.y < MAX_Y/2 && boy.x < MAX_X/2);
    array[boy.y][boy.x] = BOY;
    draw_object(boy, boy_texture);   // draw son
    if (array[boy.y][boy.x+1] == FLOOR) {
      demonGuard.x = boy.x+1;
      demonGuard.y = boy.y;
    } else if (array[boy.y][boy.x-1] == FLOOR) {
      demonGuard.x = boy.x-1;
      demonGuard.y = boy.y;
    } else if (array[boy.y+1][boy.x] == FLOOR) {
      demonGuard.x = boy.x;
      demonGuard.y = boy.y+1;
    } else if (array[boy.y-1][boy.x] == FLOOR) {
      demonGuard.x = boy.x;
      demonGuard.y = boy.y-1;
    } else { // this shouldn't happen ever but just in case put it in the middle
      demonGuard.x = MAX_X / 2;
      demonGuard.y = MAX_Y / 2;
    }

    // set the player's location at the mouth of the cave
    player.x = 0;
    player.y = 0;
    oldplayer = player;

    dir = STOP;
    eaten = true;
    number = 0;

    return;
}

// read keyboard input
void input(void) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    SDL_PumpEvents();

    if (state[SDL_SCANCODE_UP]) {
        dir = UP;
    }
    else if (state[SDL_SCANCODE_DOWN]) {
        dir = DOWN;
    }
    else if (state[SDL_SCANCODE_LEFT]) {
        dir = LEFT;
    }
    else if (state[SDL_SCANCODE_RIGHT]) {
        dir = RIGHT;
    }
    else if (state[SDL_SCANCODE_ESCAPE]) {
        gameover();
    }
    else {
      dir = STOP;
    }
    return;
}

// work out if a node can move a certain direction, then move there
// returns true if the node moves, false if not
bool moveNode (node * movingNode, char moveDirection, char array[49][49], bool isDemon) {
  if (moveDirection == 'r') {
    if (array[movingNode->y][movingNode->x + 1] == FLOOR || array[movingNode->y][movingNode->x + 1] == BOY) {
      if (isDemon) {
        if (array[movingNode->y][movingNode->x + 1] == BOY) {
          return false; // demons can't walk over the boy but the player can
        }
        array[movingNode->y][movingNode->x] = FLOOR;
        array[movingNode->y][movingNode->x + 1] = DEMON;
      }
      movingNode->x += 1;
      return true;
    }
  } else if (moveDirection == 'l') {
    if (array[movingNode->y][movingNode->x - 1] == FLOOR || array[movingNode->y][movingNode->x - 1] == BOY) {
      if (isDemon) {
        if (array[movingNode->y][movingNode->x - 1] == BOY) {
          return false; // demons can't walk over the boy but the player can
        }
        array[movingNode->y][movingNode->x] = FLOOR;
        array[movingNode->y][movingNode->x - 1] = DEMON;
      }
      movingNode->x -= 1;
      return true;
    }
  } else if (moveDirection == 'd') {
    if (array[movingNode->y + 1][movingNode->x] == FLOOR || array[movingNode->y + 1][movingNode->x] == BOY) {
      if (isDemon) {
        if (array[movingNode->y + 1][movingNode->x] == BOY) {
          return false; // demons can't walk over the boy but the player can
        }
        array[movingNode->y][movingNode->x] = FLOOR;
        array[movingNode->y + 1][movingNode->x] = DEMON;
      }
      movingNode->y += 1;
      return true;
    }
  } else if (moveDirection == 'u') {
    if (array[movingNode->y - 1][movingNode->x] == FLOOR || array[movingNode->y - 1][movingNode->x] == BOY) {
      if (isDemon) {
        if (array[movingNode->y - 1][movingNode->x] == BOY) {
          return false; // demons can't walk over the boy but the player can
        }
        array[movingNode->y][movingNode->x] = FLOOR;
        array[movingNode->y - 1][movingNode->x] = DEMON;
      }
      movingNode->y -= 1;
      return true;
    }
  } else {
    printf("invalid move direction, use u, d, l or r\n");
  }
  // if something goes wrong return false
  return false;
}

// update player position
void updateplayer (char array[MAX_Y][MAX_X]) {
    oldplayer = player;

  switch (dir) {
    case UP:
      moveNode (&player, 'u', array, false);
      break;
    case DOWN:
      moveNode (&player, 'd', array, false);
      break;
    case LEFT:
      moveNode (&player, 'l', array, false);
      break;
    case RIGHT:
      moveNode (&player, 'r', array, false);
      break;
    case STOP:
      break;
    }

    if (player.x == boy.x && player.y == boy.y) {
      array[boy.y][boy.x] = FLOOR;
      boyIsFound = true;
    }

    if (player.x == 0 || player.y == 0) {
      if (boyIsFound) {
        win = true;
        gameover();
      }
    }
}

// generate new node location
void placeNode (node * nodeToPlace, char array[MAX_Y][MAX_X]) {
  // clear the location on the array
  array[nodeToPlace->y][nodeToPlace->x] = FLOOR;
  // place the node
  do {
    nodeToPlace->x = rand() % MAX_X;
    nodeToPlace->y = rand() % MAX_Y;
  } while (array[nodeToPlace->y][nodeToPlace->x] != FLOOR);
}

// check to see if the death limit has been reached
void checkLives (void) {
  if (number >= 5) {
    gameover();
  }
}

// end of game
void gameover(void) {
  printf("\n\n");
  if (win == true) {
    printf("You saved the child! Well done!\n");
  } else {
    printf("Oh no! You lose! The child is lost forever!\n");
  }
  printf("Deaths: %d\n\n\n", number);
  exit(0);
}

// render current game state
void render(void) {
    draw_object(oldplayer, floor_texture);   // redraw old locations
    draw_object(olddemonGuard, floor_texture);
    draw_object(olddemon1, floor_texture);
    draw_object(olddemon2, floor_texture);
    draw_object(olddemon3, floor_texture);
    draw_object(olddemon4, floor_texture);

    draw_object(player, player_texture);   // draw player

    draw_object(demonGuard, demon_texture); // draw new demons if required
    draw_object(demon1, demon_texture);
    draw_object(demon2, demon_texture);
    draw_object(demon3, demon_texture);
    draw_object(demon4, demon_texture);
    SDL_RenderPresent(renderer);

    return;
}

// draw game components
void draw_object( node object, SDL_Texture *texture ) {
    SDL_Rect rect;
    rect.h = TILE_SIZE;
    rect.w = TILE_SIZE;
    rect.x = object.x * TILE_SIZE;
    rect.y = object.y * TILE_SIZE;
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    return;
}

// function to tell if two nodes are near to each other
bool isNearTo (node node1, node node2) {
  if (node1.x > (node2.x - PROX_LIMIT) && node1.x < (node2.x + PROX_LIMIT)) {
    if (node1.y > (node2.y - PROX_LIMIT) && node1.y < (node2.y + PROX_LIMIT)) {
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

// function to tell if a demon is close to a player
void demonMove (node * demonNode, node * oldDemonNode, node * playerNode, char array[MAX_Y][MAX_X]) {
  *oldDemonNode = *demonNode;

  if (isNearTo (*demonNode, *playerNode)) {
    int xDiff = playerNode->x - demonNode->x;
    int yDiff = playerNode->y - demonNode->y;
    // if we're further away on the x-axis than the x-axis
    if (abs (xDiff) >= abs (yDiff)) {
      if (xDiff > 0) {
        // attempt to move the demon, and if it doesn't work try another way
        if (moveNode (demonNode, 'r', array, true) == false) {
          // move up or down depending on yDiff
          yDiff > 0 ? moveNode (demonNode, 'd', array, true) : moveNode (demonNode, 'u', array, true);
        }
      }
      else if (xDiff <= 0) {
        // attempt to move the demon, and if it doesn't work try another way
        if (moveNode (demonNode, 'l', array, true) == false) {
          // move up or down depending on yDiff
          yDiff > 0 ? moveNode (demonNode, 'd', array, true) : moveNode (demonNode, 'u', array, true);
        }
      }
    }
    else { // if we're further away on the y axis than the x axis
      if (yDiff > 0) {
        // attempt to move the demon, and if it doesn't work try another way
        if (moveNode (demonNode, 'd', array, true) == false) {
          // move up or down depending on xDiff
          xDiff > 0 ? moveNode (demonNode, 'r', array, true) : moveNode (demonNode, 'l', array, true);
        }
      }
      else  if (yDiff <= 0){
        // attempt to move the demon, and if it doesn't work try another way
        if (moveNode (demonNode, 'u', array, true) == false) {
          // move up or down depending on xDiff
          xDiff > 0 ? moveNode (demonNode, 'r', array, true) : moveNode (demonNode, 'l', array, true);
        }
      }
    }
  } else {
    // if the player is not near to the demon, should move approx every 8 cycles
    int randomMove = rand() % 16;

    if (randomMove == 0) {
      moveNode (demonNode, 'u', array, true);
    } else if (randomMove == 1) {
      moveNode (demonNode, 'd', array, true);
    } else if (randomMove == 2) {
      moveNode (demonNode, 'l', array, true);
    } else if (randomMove == 3) {
      moveNode (demonNode, 'r', array, true);
    }
  }
  return;
}

// move all the demons
void moveDemons (char array[MAX_Y][MAX_X]) {
  demonMove (&demonGuard, &olddemonGuard, &player, array);
  demonMove (&demon1, &olddemon1, &player, array);
  demonMove (&demon2, &olddemon2, &player, array);
  demonMove (&demon3, &olddemon3, &player, array);
  demonMove (&demon4, &olddemon4, &player, array);
  return;
}

// check if a demon is on the player
void demonCheck (node * demonNode, node * playerNode, char array[MAX_Y][MAX_X]) {
  if (playerNode->x == demonNode->x && playerNode->y == demonNode->y) {
      playerNode->x = playerNode->y = 0;
      if (boyIsFound) {
        // place boy and set his position to be inaccessible to demons, also make sure he's not in the same quarter as the player
        do {placeNode (&boy, array);} while (boy.y < MAX_Y/2 && boy.x < MAX_X/2);
        array[boy.y][boy.x] = BOY;
        draw_object(boy, boy_texture);   // draw son
        boyIsFound = false;
      }
      number++;
      eaten = true;  // demon eats you
  } else {
      eaten = false; // continue
  }
}

// check no demons are on the player
void checkDemons (char array[MAX_Y][MAX_X]) {
  demonCheck (&demonGuard, &player, array);
  demonCheck (&demon1, &player, array);
  demonCheck (&demon2, &player, array);
  demonCheck (&demon3, &player, array);
  demonCheck (&demon4, &player, array);
}
