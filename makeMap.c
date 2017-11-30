#include <stdlib.h>
#include <stdio.h>
#include "map.h"

// return an initalised tree node
treeNode * newNode (int x, int y, int w, int h) {
  treeNode * node = malloc (sizeof(treeNode));

  node->lChild = NULL;
  node->rChild = NULL;

  node->x = x;
  node->y = y;
  node->w = w;
  node->h = h;

  node->centerX = x + w/2;
  node->centerY = y + h/2;

  node->roomX = 0;
  node->roomY = 0;
  node->roomW = 0;
  node->roomH = 0;

  return node;
}

// initialise a tree
treeNode * initialiseTree (int w, int h) {
  treeNode * node = newNode(0, 0, w, h);

  return node;
}

// give a node a pair of children, with a random horizontal or vertical split
void splitNode (treeNode * node) {
  int lChildX;
  int lChildY;
  int lChildW = 0; // so it doesn't skip through the process of choosing a new
  int lChildH = 0; // child height and width (see while loops below)

  int rChildX;
  int rChildY;
  int rChildW;
  int rChildH;

  float rRatio = 0.0;
  float lRatio = 0.0;

  // to make sure we don't get rubbishly long or wide rooms
  while ((rRatio < 0.45 || rRatio > 2.2) || (lRatio < 0.45 || lRatio > 2.2)) {
    // horizontal split
    if (rand() % 2 == 0) {
      lChildX = node->x;
      lChildY = node->y;
      lChildW = node->w;
      lChildH = rand() % node->h;
      lRatio = (float) lChildH/lChildW;

      rChildX = node->x;
      rChildY = node->y + lChildH;
      rChildW = node->w;
      rChildH = node->h - lChildH;
      rRatio = (float) rChildH/rChildW;
    }
    // vertical split
    else {
      lChildX = node->x;
      lChildY = node->y;
      lChildH = node->h;
      lChildW = rand() % node->w;
      lRatio = (float) lChildH/lChildW;

      rChildX = node->x + lChildW;
      rChildY = node->y;
      rChildW = node->w - lChildW;
      rChildH = node->h;
      rRatio = (float) rChildH/rChildW;
    }
  }

  node->lChild = newNode(lChildX, lChildY, lChildW, lChildH);
  node->rChild = newNode(rChildX, rChildY, rChildW, rChildH);
  return;
}

// make a single room for a space
void makeRoom (treeNode * node) {
  int minW = 3 * node->w / 4;
  int minH = 3 * node->h / 4;

  int roomW = 0;
  int roomH = 0;

  while (roomW < minW || roomH < minH) {
    roomW = rand() % node->w;
    roomH = rand() % node->h;
  }

  node->roomW = roomW;
  node->roomH = roomH;

  node->roomX = node->x + (rand() % (node->w - roomW));
  node->roomY = node->y + (rand() % (node->h - roomH));
}

// make rooms for all the leaves of the tree
void makeRooms (treeNode * node) {
  if (node->lChild == NULL) {
    makeRoom (node);
  } else {
    makeRooms (node->lChild);
    makeRooms (node->rChild);
  }
}

// recursively call splitNode on all the leaves of the tree
void splitTree (treeNode * node) {
  if (node->lChild == NULL) {
    splitNode(node);
  } else {
    splitTree (node->lChild);
    splitTree (node->rChild);
  }
}

// prints out the dimensions of a tree's leaves for debugging purposes
void printLeaves (treeNode * node) {
  // if a leaf print "leaf found"
  if (node->lChild == NULL) {
    printf("%d %d %d %d\n", node->x, node->y, node->w, node->h);
  }
  // if not a leaf, call this function on the children
  else {
    printLeaves(node->lChild);
    printLeaves(node->rChild);
  }
  return;
}

// recursively connect all the rooms
void connectRooms (char array[][TILES_WIDTH], treeNode * node) {
  if (node->lChild != NULL) {
    // connect the children
    connectRooms (array, node->lChild);
    connectRooms (array, node->rChild);
    // work out if it was a horizontal or vertical split
    if (node->lChild->centerX == node->rChild->centerX) {
      for (int i = node->lChild->centerY; i <= node->rChild->centerY; i++) {
        array [i][node->lChild->centerX] = FLOOR;
      }
    } else {
      for (int j = node->lChild->centerX; j <= node->rChild->centerX; j++) {
        array [node->lChild->centerY][j] = FLOOR;
      }
    }
  }
  return;
}

void generateMap (char array[] [TILES_WIDTH]) {
  int x = TILES_WIDTH;
  int y = TILES_HEIGHT;

  // initialise values
  for (int i = 0; i < y; ++i) {
    for (int j = 0; j < x; ++j) {
      array [i][j] = WALL;
    }
  }

  treeNode * head = initialiseTree(x, y);
  splitTree   (head);
  splitTree   (head);
  splitTree   (head);
  splitTree   (head);
  makeRooms   (head);
  writeTiles  (array, head);
  connectRooms(array, head);

  // make sure there are no open ends
  for (int i = 0; i < TILES_HEIGHT; i++) {
    array[i][0] = WALL;
  }
  for (int j = 0; j < TILES_WIDTH; j++) {
    array[0][j] = WALL;
  }
  // add an exit
  for (int i = 0; i < 5; i++){
    for (int j = 0; j < 5; j++) {
      array[i][j] = FLOOR;
    }
  }
  return;
}

void printMap (char array [][TILES_WIDTH]) {
  // print all values
  for (int i = 0; i < TILES_HEIGHT; ++i) {
    for (int j = 0; j < TILES_WIDTH; ++j) {
      printf("%c", array [i][j]);
      printf("%c", array [i][j]);
    }
    printf("\n");
  }
}

void writeTiles (char tiles[][TILES_WIDTH], treeNode * node) {
  if (node->lChild == NULL) {
    for (int i = node->roomY; i < (node->roomY + node->roomH); i++) {
      for (int j = node->roomX; j < (node->roomX + node->roomW); j++) {
        tiles[i][j] = FLOOR;
      }
    }
  }
  else {
    writeTiles (tiles, node->lChild);
    writeTiles (tiles, node->rChild);
  }
}
