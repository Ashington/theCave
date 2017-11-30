
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <time.h>

#include "controlFunctions.h"
#include "map.h"

int main(int argc, char const * argv[]) {
  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n**** WELCOME TO THE CAVE ****\n\n");
  printf("Your child has been taken by demons into a mysterious cave!\n");
  printf("You must rescue your child from the demons! Are you ready to enter?\n\n");
  printf("Press enter to start.\n");
  // for waiting for the player to start
  getchar();


  // to generate the map
  srand(time(NULL));
  char tiles [TILES_HEIGHT][TILES_WIDTH];
  generateMap(tiles);
  // printMap (tiles); // for debugging

  int delay = 80;

  init(tiles);   // setup the game data
  render(); // render initial state

  int count = 0; // time measure to make sure the demons aren't faster that the player

  while (true) { // game loop

    input();           // keyboard input
    updateplayer(tiles);    // update the player

    if (count % 2 == 0) {
      moveDemons(tiles);       // move the demons every second turn
    }

    checkDemons (tiles);

    render();          // render new state
    SDL_Delay (delay); // wait for next iteration of loop

    // if we reach the maximum number of lives
    checkLives();

    count++;
    // printMap (tiles); // for debugging
  }
  return 0; // we're done!
}
