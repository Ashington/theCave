#define TILES_HEIGHT 49
#define TILES_WIDTH 49
#define WALL '0'
#define FLOOR ' '
#define DEMON 'D'
#define BOY 'B'


// struct for a tree node
typedef struct tree_node {
  // children
  struct tree_node * lChild;
  struct tree_node * rChild;
  // position and size of area
  int x, y, w, h;
  // center of area
  int centerX, centerY;
  // position and size of room
  int roomX, roomY, roomW, roomH;
} treeNode;

treeNode * newNode ();
treeNode * initialiseTree (int w, int h);
void splitNode (treeNode * node);
void splitTree (treeNode * node);
void printLeaves (treeNode * node);
void writeTiles (char tiles[][TILES_WIDTH], treeNode * tree);
void makeRoom (treeNode * node);
void makeRooms (treeNode * node);
void connectRooms (char array[][TILES_WIDTH], treeNode * node);
void generateMap (char array[] [TILES_WIDTH]);
void printMap (char array [][TILES_WIDTH]);
