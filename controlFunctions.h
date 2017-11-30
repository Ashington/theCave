// initialise the game
void init(char array[49][49]);
// get the input from the keyboard
void input(void);
// move the player if necessary, and also check to see if you've picked up the child
void updateplayer (char array[49][49]);
// end the game
void gameover(void);
// render the drawn objects
void render(void);
// move all the demons
void moveDemons (char array[49][49]);
// check to see if the demons are on the player
void checkDemons (char array[49][49]);
// check to see if the game is over by using up too many lives
void checkLives (void);
