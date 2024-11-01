#define MAZE_SIZE 10

// Generate a MAZE_SIZE x MAZE_SIZE connected maze
void generateMaze(uint8_t **maze);

void freeMaze(uint8_t** edges);

// Draw a given maze from generateMaze()
void drawMaze(uint8_t** edges);