#include <libdragon.h>
#include "maze.h"

void removeDirection(int *dirs, int* numDirs, int dirToRemove)
{
    for (int i = 0; i < *numDirs; i++){
        if (dirs[i] == dirToRemove)
        {
            dirs[i] = dirs[--(*numDirs)];
            break;
        }
    }
}

void addNeighbor(int neighborToAdd, int* neighbors, int* numNeighbors)
{
    for (int i = 0; i < *numNeighbors; i++)
    {
        if (neighbors[i] == neighborToAdd)
        {
            return;
        }
    }
    neighbors[(*numNeighbors)++] = neighborToAdd;
}

// given a visitedNode, add its non-visited neighbors to neighbors and increment numNeighbors for each neighbor added. Return the direction chosen by visitedNode based on its neighbors
uint8_t processNode(int visitedNode, uint8_t* visited, int* neighbors, int* numNeighbors)
{
    int numDirs = 4;
    int dirs[4] = {1, 2, 3, 4};
    
    // if above node exists and has not been visited, add it to neighbors
    // also remove direction 1 if no above node exists, or it exists but has been visited
    if (visitedNode > (MAZE_SIZE - 1) )
    {
        if (visited[visitedNode - MAZE_SIZE] == 0)
        {
             addNeighbor(visitedNode - MAZE_SIZE, neighbors, numNeighbors);
             removeDirection(dirs, &numDirs, 1);
        }
    }
    else
    {
        removeDirection(dirs, &numDirs, 1);
    }

    // if right node exists and has not been visited, add it to neighbors
    // also remove direction 2 if no right node exists, or it exists but has been visited
    if (visitedNode % MAZE_SIZE != (MAZE_SIZE - 1))
    {
        if (visited[visitedNode + 1] == 0)
        {
            addNeighbor(visitedNode + 1, neighbors, numNeighbors);
            removeDirection(dirs, &numDirs, 2);
        }
    }
    else
    {
        removeDirection(dirs, &numDirs, 2);
    }

    // if below node exists and has not been visited, add it to neighbors
    // also remove direction 3 if no below node exists, or it exists but has been visited
    if (visitedNode < (MAZE_SIZE * MAZE_SIZE) - MAZE_SIZE)
    {
        if (visited[visitedNode + MAZE_SIZE] == 0)
        {
            addNeighbor(visitedNode + MAZE_SIZE, neighbors, numNeighbors);
            removeDirection(dirs, &numDirs, 3);
        }
    }
    else
    {
        removeDirection(dirs, &numDirs, 3);
    }

    // if left node exists and has not been visited, add it to neighbors
    // also remove direction 4 if no left node exists, or it exists but has been visited
    if (visitedNode % MAZE_SIZE != 0)
    {
        if (visited[visitedNode - 1] == 0)
        {
            addNeighbor(visitedNode - 1, neighbors, numNeighbors);
            removeDirection(dirs, &numDirs, 4);
        }
    }
    else 
    {
        removeDirection(dirs, &numDirs, 4);
    }

    if(numDirs)
    {
        return dirs[rand() % numDirs];
    }

    return rand() % 4 + 1;
}

void drawMap(node **map)
{
    int size = (2 * MAZE_SIZE) + 1;
    char disp[MAZE_SIZE + 1][size];

    for (int r = 0; r < MAZE_SIZE + 1; r++)
    {
        for (int c = 0; c < size; c++)
        {
            disp[r][c] = ' ';
        }
    }
    
    // ceiling and floor
    for (int i = 0; i < size - 2; i+=2)
    {
        disp[0][i] = ' ';
        disp[0][i+1] = '_';
        
        disp[MAZE_SIZE][i] = ' ';
        disp[MAZE_SIZE][i+1] = '_';
    }
    
    for (int i = 1; i < MAZE_SIZE + 1; i++)
    {
        disp[i][0] = '|';
        disp[i][size-1] = '|';
    }
    
    for (int row = 1; row < MAZE_SIZE + 1; row++)
    {   
        for (int col = 1; col < size - 2; col+=2)
        {
            disp[row-1][col] = map[row-1][((col+1)/2)-1].walls[0] ? '_' : ' ';
            disp[row][col+1] = map[row-1][((col+1)/2)-1].walls[1] ? '|' : ' ';
            disp[row][col] = map[row-1][((col+1)/2)-1].walls[2] ? '_' : ' ';
            disp[row][col-1] = map[row-1][((col+1)/2)-1].walls[3] ? '|' : ' ';
        }
    }
    
    for (int row = 0; row <  MAZE_SIZE + 1; row++)
    {
        for (int col = 0; col < size; col++)
        {
            printf("%c", disp[row][col]);
        }
        printf("\n");
    }
}

// given a node, prune it
void pruneNode(node* n)
{
    int validWalls[4];
    int numOfValidWalls = 0;
    
    for (int i = 0; i < 4; i++)
    {
        if (n->walls[i])
        {
            validWalls[numOfValidWalls++] = i;
        }
    }
    
    if (numOfValidWalls)
    {
        n->walls[validWalls[rand() % numOfValidWalls]] = 0;
    }
}

void generateMap(node **map)
{   
    int numVisited = 1;
    uint8_t visited[MAZE_SIZE * MAZE_SIZE] = {0};
    
    int numNeighbors = 0;
    int neighbors[MAZE_SIZE * MAZE_SIZE] = {0};
    
    int source = rand() % (MAZE_SIZE * MAZE_SIZE);
    visited[source] = 1;
    
    processNode(source, visited, neighbors, &numNeighbors);
    
    uint8_t maze[MAZE_SIZE][MAZE_SIZE];
    maze[source / MAZE_SIZE][source % MAZE_SIZE] = 0;
    
    while (numNeighbors > 0)
    {
        int n = rand() % numNeighbors;
        int nodeToVisit = neighbors[n];
        neighbors[n] = neighbors[--numNeighbors];
        visited[nodeToVisit] = 1;
        numVisited++;
        maze[nodeToVisit / MAZE_SIZE][nodeToVisit % MAZE_SIZE] = processNode(nodeToVisit, visited, neighbors, &numNeighbors);
    }
    
    //node map[MAZE_SIZE][MAZE_SIZE];
    
    // convert connected maze to a map of nodes
    node n;
    for (int row = 0; row < MAZE_SIZE; row++)
    {
        for (int col = 0; col < MAZE_SIZE; col++)
        {
            // reset n
            for (int i = 0; i < 4; i++)
            {
                n.walls[i] = 1;
            }
            // start by removing wall known to maze location
            if (maze[row][col])
            {
                n.walls[maze[row][col] - 1] = 0;
            }
            
            // find neighboring walls to remove
            n.walls[0] = n.walls[0] ? ((row != 0) && (maze[row-1][col] == 3) ? 0 : 1) : 0;
            n.walls[1] = n.walls[1] ? ((col != (MAZE_SIZE - 1)) && (maze[row][col+1] == 4) ? 0 : 1) : 0;
            n.walls[2] = n.walls[2] ? ((row != (MAZE_SIZE - 1)) && (maze[row+1][col] == 1) ? 0 : 1) : 0;
            n.walls[3] = n.walls[3] ? ((col != 0) && (maze[row][col-1] == 2) ? 0 : 1) : 0;
            
            map[row][col] = n;
        }
    }
    
    //drawMap(map);
    
    int pruneFactor = 60;
    /* prune walls to open up map more                                             */
    /* only prune nodes not on the edge of the maze to avoid:                      */
    /*      1. array overflow/underflow                                            */
    /*      2. potential line of sight to opponent at start of match               */
    /*         (still possible by random chance, but less likely when not pruning) */
    for (int row = 1; row < MAZE_SIZE - 1; row++)
    {
        for (int col = 1; col < MAZE_SIZE - 1; col++)
        {
            /* randomly determine if node will be pruned */
            if ((rand() % 100) < pruneFactor)
            {
                pruneNode(&map[row][col]);
            }
        }
    }
    
    //drawMap(map);
    
    for (int row = 0; row < MAZE_SIZE; row++)
    {
        for (int col = 0; col < MAZE_SIZE; col++)
        {
            //printf("%d ", maze[row][col]);
        }
        //printf("\n");
    }
    
    //printf("\n");
    
    for (int row = 0; row < MAZE_SIZE; row++)
    {
        for (int col = 0; col < MAZE_SIZE; col++)
        {
            //printf("{");
            for (int i = 0; i < 4; i++)
            {
                //printf("%d,", map[row][col].walls[i]);
            }
            //printf("}");
        }
        //printf("\n");
    }
}

void freeMap(node** map)
{
    for (int i = 0; i < MAZE_SIZE; i++) {
        free_uncached(map[i]);
        map[i] = NULL;
    }
    free_uncached(map);
    map = NULL;
}

// void drawMaze(uint8_t **edges)
// {
//     int size = (2 * MAZE_SIZE) + 1;
//     char maze[MAZE_SIZE + 1][size];
    
//     // initialize maze printout
//     for (int row = 0; row < MAZE_SIZE + 1; row++)
//     {   
//         for (int col = 0; col < size - 1;)
//         {
//             maze[row][col] = (row == 0) ? ' ' : ((edges[row-1][col/2 - 1] == 2) || (edges[row-1][col/2] == 4)) && (col != 0) ? ' ' : '|';
//             col+=1;
//             maze[row][col] = (row != 0) && (row != MAZE_SIZE) && ((edges[row-1][col/2] == 3) || (edges[row][col/2] == 1)) ? ' ' : '_';
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