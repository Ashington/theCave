// place a node on the map, but make sure that it is on floor instead of wall
void placeNode (node * nodeToPlace, char array[MAX_Y][MAX_Y]);
// move a node in a certain direction if the place on the map is free
bool moveNode (node * movingNode, char moveDirection, char array[MAX_Y][MAX_Y], bool isDemon);
// draw a rectangular shape of a given texture
void draw_object(node object, SDL_Texture *texture);
