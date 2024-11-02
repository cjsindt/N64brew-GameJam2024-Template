#define MAZE_SIZE 10

typedef struct node_t
{
    uint8_t walls[4];
} node;

// Generate a MAZE_SIZE x MAZE_SIZE connected maze
void generateMap(node **map);

void freeMap(node** map);

// Draw a given maze from generateMaze()
void drawMap(node **map);