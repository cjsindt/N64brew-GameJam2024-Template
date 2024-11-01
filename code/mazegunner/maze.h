#define MAZE_SIZE 24

// Generate a MAZE_SIZE x MAZE_SIZE connected maze
int** generateMaze();

void freeMaze(int** edges);

// Draw a given maze from generateMaze()
void drawMaze(int** edges);