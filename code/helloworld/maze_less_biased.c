// #include <stdio.h>
// #include <stdlib.h>
// #include <limits.h>
// #include <time.h>

// #define MAZE_SIZE 10

// void removeDirection(int *dirs, int* numDirs, int dirToRemove)
// {
//     for (int i = 0; i < *numDirs; i++){
//         if (dirs[i] == dirToRemove)
//         {
//             dirs[i] = dirs[--(*numDirs)];
//             break;
//         }
//     }
// }

// void addNeighbor(int neighborToAdd, int* neighbors, int* numNeighbors)
// {
//     for (int i = 0; i < *numNeighbors; i++)
//     {
//         if (neighbors[i] == neighborToAdd)
//         {
//             return;
//         }
//     }
//     neighbors[(*numNeighbors)++] = neighborToAdd;
// }

// // given a visitedNode, add its non-visited neighbors to neighbors and increment numNeighbors for each neighbor added. Return the direction chosen by visitedNode based on its neighbors
// int processNode(int visitedNode, char* visited, int* neighbors, int* numNeighbors)
// {
//     int numDirs = 4;
//     int dirs[4] = {1, 2, 3, 4};
    
//     // if above node exists and has not been visited, add it to neighbors
//     if (visitedNode > (MAZE_SIZE - 1) )
//     {
//         if (visited[visitedNode - MAZE_SIZE] == 0)
//         {
//              addNeighbor(visitedNode - MAZE_SIZE, neighbors, numNeighbors);
//              removeDirection(dirs, &numDirs, 1);
//         }
//     }
//     else
//     {
//         removeDirection(dirs, &numDirs, 1);
//     }
//     // if right node exists and has not been visited, add it to neighbors
//     if (visitedNode % MAZE_SIZE != (MAZE_SIZE - 1))
//     {
//         if (visited[visitedNode + 1] == 0)
//         {
//             addNeighbor(visitedNode + 1, neighbors, numNeighbors);
//             removeDirection(dirs, &numDirs, 2);
//         }
//     }
//     else
//     {
//         removeDirection(dirs, &numDirs, 2);
//     }
//     // if below node exists and has not been visited, add it to neighbors
//     if (visitedNode < (MAZE_SIZE * MAZE_SIZE) - MAZE_SIZE)
//     {
//         if (visited[visitedNode + MAZE_SIZE] == 0)
//         {
//             addNeighbor(visitedNode + MAZE_SIZE, neighbors, numNeighbors);
//             removeDirection(dirs, &numDirs, 3);
//         }
//     }
//     else
//     {
//         removeDirection(dirs, &numDirs, 3);
//     }
//     // if left node exists and has not been visited, add it to neighbors
//     if (visitedNode % MAZE_SIZE != 0)
//     {
//         if (visited[visitedNode - 1] == 0)
//         {
//             addNeighbor(visitedNode - 1, neighbors, numNeighbors);
//             removeDirection(dirs, &numDirs, 4);
//         }
//     }
//     else 
//     {
//         removeDirection(dirs, &numDirs, 4);
//     }
//     //printf("numdirs: %d\n", numDirs);
//     if(numDirs)
//     {
//         return dirs[rand() % numDirs];
//     }
//     return rand() % 4 + 1;
// }


// int** generate_maze()
// {
//     int** map = malloc(MAZE_SIZE * sizeof(int*));
//     for (int i = 0; i < MAZE_SIZE; i++) {
//         map[i] = malloc(MAZE_SIZE * sizeof(int));
//     }
    
//     int numVisited = 1;
//     char visited[MAZE_SIZE * MAZE_SIZE] = {0};
    
//     int numNeighbors = 0;
//     int neighbors[MAZE_SIZE * MAZE_SIZE] = {0};
    
//     int source = rand() % (MAZE_SIZE * MAZE_SIZE);
//     visited[source] = 1;
    
//     processNode(source, visited, neighbors, &numNeighbors);
    
//     map[source / MAZE_SIZE][source % MAZE_SIZE] = 0;
    
//     while (numNeighbors > 0)
//     {
//         int n = rand() % numNeighbors;
//         int nodeToVisit = neighbors[n];
//         neighbors[n] = neighbors[--numNeighbors];
//         visited[nodeToVisit] = 1;
//         numVisited++;
//         //printf("nodeToVisit: %d\n", nodeToVisit);
//         //printf("%d \n", processNode(nodeToVisit, visited, neighbors, &numNeighbors));
//         map[nodeToVisit / MAZE_SIZE][nodeToVisit % MAZE_SIZE] = processNode(nodeToVisit, visited, neighbors, &numNeighbors);
//     }

//     for (int i = 0; i < MAZE_SIZE * MAZE_SIZE; i++)
//     {
//         //printf("%d ", visited[i]);
//     }
//     //printf("\n%d\n", numVisited);
    
//     for (int i = 0; i < MAZE_SIZE * MAZE_SIZE; i++)
//     {
//        // printf("%d ", neighbors[i]);
//     }
//     //printf("\n%d\n", numNeighbors);
    
//     for (int row = 0; row < MAZE_SIZE; row++)
//     {
//         for (int col = 0; col < MAZE_SIZE; col++)
//         {
//             //printf("%d ", map[row][col]);
//         }
//         //printf("\n");
//     }
    
//     return map;
// }

// void draw_maze(int **edges)
// {
//     int size = (2 * MAZE_SIZE) + 1;
//     char maze[MAZE_SIZE + 1][size];
    
//     // initialize maze printout
//     for (int row = 0; row < MAZE_SIZE + 1; row++)
//     {   
//         for (int col = 0; col < size - 1;)
//         {
//             int i = col;
//             //printf("%d\n", i);
//             maze[row][col] = (row == 0) ? ' ' : ((edges[row-1][i/2 - 1] == 2) || (edges[row-1][i/2] == 4)) && (col != 0) ? ' ' : '|';
//             col+=1;
//             //printf("%d %d\n", row, i);
//             maze[row][col] = (row != 0) && (row != MAZE_SIZE) && ((edges[row-1][i/2] == 3) || (edges[row][i/2] == 1)) ? ' ' : '_';
//             col+=1;
//         }
//         maze[row][size - 1] = (row == 0) ? ' ' : '|';
//     }
    
//     // print maze
//     for (int row = 0; row <  MAZE_SIZE + 1; row++)
//     {
//         for (int col = 0; col < size; col++)
//         {
//             printf("%c", maze[row][col]);
//         }
//         printf("\n");
//     }
// }

// int main()
// {
//     srand(time(0));
//     draw_maze(generate_maze());
//     return 0;
// }