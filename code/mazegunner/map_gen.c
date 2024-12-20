#include <libdragon.h>
#include "map_gen.h"

 /*************************************************************
 * remove_direction()
 *
 * Description: Given a list of directions and a direction to
 *              remove, remove it.
 *  
 *
 * @param neighborToAdd the neighbor to add to the list
 * @param neighbors the list of neighbors
 * @param numNeighbors the size of neighbors
 * 
 *************************************************************/
void remove_direction(uint8_t *dirs, uint8_t* numDirs, uint8_t dirToRemove)
{
    for (int i = 0; i < *numDirs; i++){
        if (dirs[i] == dirToRemove)
        {
            dirs[i] = dirs[--(*numDirs)];
            break;
        }
    }
}


/**************************************************************
 * add_neighbor()
 *
 * Description: Add currently visited node's neighbors to the
 *              neighbors list.
 *  
 *
 * @param neighborToAdd the neighbor to add to the list
 * @param neighbors the list of neighbors
 * @param numNeighbors the size of neighbors
 * 
 *************************************************************/
void add_neighbor(int neighborToAdd, int* neighbors, int* numNeighbors)
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


/**************************************************************
 * process_node()
 *
 * Description: Given a node being visited, add its
 *              non-visited neighbors to the neighbors list.
 *              Return the direction chosen by the visited
 *              node based on its neighbors.
 *  
 *
 * @param visitedNode the node being visited
 * @param visited the list of visited nodes
 * @param neighbors the list of neighbors
 * @param numNeighbors the size of neighbors
 * @return the direction chosen by visitedNode
 * 
 *************************************************************/
uint8_t process_node(int visitedNode, uint8_t* visited, int* neighbors, int* numNeighbors)
{
    uint8_t numDirs = 4;
    uint8_t dirs[4] = {1, 2, 3, 4};
    
    /* if above node exists and has not been visited, add it to neighbors                 */
    /* also remove direction 1 if no above node exists, or it exists but has been visited */
    if (visitedNode > (MAP_SIZE - 1) )
    {
        if (visited[visitedNode - MAP_SIZE] == 0)
        {
             add_neighbor(visitedNode - MAP_SIZE, neighbors, numNeighbors);
             remove_direction(dirs, &numDirs, 1);
        }
    }
    else
    {
        remove_direction(dirs, &numDirs, 1);
    }

    /* if right node exists and has not been visited, add it to neighbors                 */
    /* also remove direction 2 if no right node exists, or it exists but has been visited */
    if (visitedNode % MAP_SIZE != (MAP_SIZE - 1))
    {
        if (visited[visitedNode + 1] == 0)
        {
            add_neighbor(visitedNode + 1, neighbors, numNeighbors);
            remove_direction(dirs, &numDirs, 2);
        }
    }
    else
    {
        remove_direction(dirs, &numDirs, 2);
    }

    /* if below node exists and has not been visited, add it to neighbors                 */
    /* also remove direction 3 if no below node exists, or it exists but has been visited */
    if (visitedNode < (MAP_SIZE * MAP_SIZE) - MAP_SIZE)
    {
        if (visited[visitedNode + MAP_SIZE] == 0)
        {
            add_neighbor(visitedNode + MAP_SIZE, neighbors, numNeighbors);
            remove_direction(dirs, &numDirs, 3);
        }
    }
    else
    {
        remove_direction(dirs, &numDirs, 3);
    }

    /* if left node exists and has not been visited, add it to neighbors                 */
    /* also remove direction 4 if no left node exists, or it exists but has been visited */
    if (visitedNode % MAP_SIZE != 0)
    {
        if (visited[visitedNode - 1] == 0)
        {
            add_neighbor(visitedNode - 1, neighbors, numNeighbors);
            remove_direction(dirs, &numDirs, 4);
        }
    }
    else 
    {
        remove_direction(dirs, &numDirs, 4);
    }

    if(numDirs)
    {
        return dirs[rand() % numDirs];
    }

    return rand() % 4 + 1;
}


/**************************************************************
 * draw_map()
 *
 * Description: Given the map, draw it to the console.
 *  
 *
 * @param map the game map
 * 
 *************************************************************/
void draw_map(MapNode_t **map)
{
    int size = (2 * MAP_SIZE) + 1;
    char disp[MAP_SIZE + 1][size];

    /* initialize disp to all whitespace */
    for (int r = 0; r < MAP_SIZE + 1; r++)
    {
        for (int c = 0; c < size; c++)
        {
            disp[r][c] = ' ';
        }
    }
    
    /* ceiling and floor */
    for (int i = 0; i < size - 2; i+=2)
    {
        disp[0][i] = ' ';
        disp[0][i+1] = '_';
        
        disp[MAP_SIZE][i] = ' ';
        disp[MAP_SIZE][i+1] = '_';
    }
    
    /* add walls to left and right side */
    for (int i = 1; i < MAP_SIZE + 1; i++)
    {
        disp[i][0] = '|';
        disp[i][size-1] = '|';
    }
    
    /* go cell by cell and fill in walls based on map information */
    for (int row = 1; row < MAP_SIZE + 1; row++)
    {   
        for (int col = 1; col < size - 1; col+=2)
        {
            disp[row-1][col] = map[row-1][((col+1)/2)-1].walls[0] ? '_' : ' ';
            disp[row][col+1] = map[row-1][((col+1)/2)-1].walls[1] ? '|' : ' ';
            disp[row][col] = map[row-1][((col+1)/2)-1].walls[2] ? '_' : ' ';
            disp[row][col-1] = map[row-1][((col+1)/2)-1].walls[3] ? '|' : ' ';
        }
    }
    
    /* print out each char */
    for (int row = 0; row <  MAP_SIZE + 1; row++)
    {
        for (int col = 0; col < size; col++)
        {
            printf("%c", disp[row][col]);
        }
        printf("\n");
    }
}

/**************************************************************
 * prune_node()
 *
 * Description: Given a node, prune a random wall from it.
 *  
 *
 * @param n the node to prune
 * 
 *************************************************************/
void prune_node(MapNode_t* n)
{
    int validWalls[4];
    int numOfValidWalls = 0;
    
    /* list each valid wall */
    for (int i = 0; i < 4; i++)
    {
        if (n->walls[i])
        {
            validWalls[numOfValidWalls++] = i;
        }
    }
    
    /* choose a random valid wall and remove it */
    if (numOfValidWalls)
    {
        n->walls[validWalls[rand() % numOfValidWalls]] = 0;
    }
}

/**************************************************************
 * generate_map()
 *
 * Description: Generate a new map.
 *  
 *
 * @param map where to store the game map
 * @param pruneFactor how strongly to prune the map [0,99]
 * 
 *************************************************************/
void generate_map(MapNode_t **map, uint8_t pruneFactor)
{   
    /* keep a list of whether each node has been visited */
    /* 0 - not visited                                   */
    /* 1 - visited                                       */
    int numVisited = 1;
    uint8_t visited[MAP_SIZE * MAP_SIZE] = {0};
    
    /* keep a list of neighbors */
    int numNeighbors = 0;
    int neighbors[MAP_SIZE * MAP_SIZE] = {0};
    
    /* choose a random node to start from and process it */
    int source = rand() % (MAP_SIZE * MAP_SIZE);
    visited[source] = 1;
    
    process_node(source, visited, neighbors, &numNeighbors);
    
    /* store the maze for further processing */
    uint8_t maze[MAP_SIZE][MAP_SIZE];
    maze[source / MAP_SIZE][source % MAP_SIZE] = 0;
    
    /* keep visiting nodes until no more neighbors remain */
    while (numNeighbors > 0)
    {
        int n = rand() % numNeighbors;
        int nodeToVisit = neighbors[n];
        neighbors[n] = neighbors[--numNeighbors];
        visited[nodeToVisit] = 1;
        numVisited++;
        maze[nodeToVisit / MAP_SIZE][nodeToVisit % MAP_SIZE] = process_node(nodeToVisit, visited, neighbors, &numNeighbors);
    }
    
    
    /* convert connected maze to a map of nodes */
    MapNode_t n;
    for (int row = 0; row < MAP_SIZE; row++)
    {
        for (int col = 0; col < MAP_SIZE; col++)
        {
            /* reset n */
            for (int i = 0; i < 4; i++)
            {
                n.walls[i] = 1;
            }

            /* start by removing wall known to maze location */
            if (maze[row][col])
            {
                n.walls[maze[row][col] - 1] = 0;
            }
            
            /* find neighboring walls to remove */
            n.walls[0] = n.walls[0] ? ((row != 0) && (maze[row-1][col] == 3) ? 0 : 1) : 0;
            n.walls[1] = n.walls[1] ? ((col != (MAP_SIZE - 1)) && (maze[row][col+1] == 4) ? 0 : 1) : 0;
            n.walls[2] = n.walls[2] ? ((row != (MAP_SIZE - 1)) && (maze[row+1][col] == 1) ? 0 : 1) : 0;
            n.walls[3] = n.walls[3] ? ((col != 0) && (maze[row][col-1] == 2) ? 0 : 1) : 0;
            
            map[row][col] = n;
        }
    }
    
    /* prune walls to open up map more                                             */
    /* only prune nodes not on the edge of the maze to avoid:                      */
    /*      1. array overflow/underflow                                            */
    /*      2. potential line of sight to opponent at start of match               */
    /*         (still possible by random chance, but less likely when not pruning) */
    for (int row = 1; row < MAP_SIZE - 1; row++)
    {
        for (int col = 1; col < MAP_SIZE - 1; col++)
        {
            /* randomly determine if node will be pruned */
            if ((rand() % 100) < pruneFactor)
            {
                prune_node(&map[row][col]);
            }
        }
    }
    
    /* remove line of sight from players on spawn                        */
    /* place a wall on each far wall of the maze at some random distance */
    /* to keep from sealing off players, open the adjacent walls         */

    /* top wall */
    uint8_t val = rand() % ((MAP_SIZE - (MAP_SIZE / 4)) + 1 - (MAP_SIZE / 2)) + (MAP_SIZE / 4);
    map[0][val].walls[1] = 1;
    map[0][val].walls[2] = 0;
    map[0][val + 1].walls[3] = 1;
    map[0][val + 1].walls[2] = 0;
    map[1][val].walls[0] = 0;
    map[1][val].walls[1] = 0;
    map[1][val + 1].walls[0] = 0;
    map[1][val + 1].walls[3] = 0;

    /* bottom wall */
    val = rand() % ((MAP_SIZE - (MAP_SIZE / 4)) + 1 - (MAP_SIZE / 2)) + (MAP_SIZE / 4);
    map[MAP_SIZE - 1][val].walls[1] = 1;
    map[MAP_SIZE - 1][val].walls[0] = 0;
    map[MAP_SIZE - 1][val + 1].walls[3] = 1;
    map[MAP_SIZE - 1][val + 1].walls[0] = 0;
    map[MAP_SIZE - 2][val].walls[2] = 0;
    map[MAP_SIZE - 2][val].walls[1] = 0;
    map[MAP_SIZE - 2][val + 1].walls[2] = 0;
    map[MAP_SIZE - 2][val + 1].walls[3] = 0;

    /* left wall */
    val = rand() % ((MAP_SIZE - (MAP_SIZE / 4)) + 1 - (MAP_SIZE / 2)) + (MAP_SIZE / 4);
    map[val][0].walls[2] = 1;
    map[val][0].walls[1] = 0;
    map[val + 1][0].walls[0] = 1;
    map[val + 1][0].walls[1] = 0;
    map[val][1].walls[3] = 0;
    map[val][1].walls[2] = 0;
    map[val + 1][1].walls[0] = 0;
    map[val + 1][1].walls[3] = 0;

    /* right wall */
    val = rand() % ((MAP_SIZE - (MAP_SIZE / 4)) + 1 - (MAP_SIZE / 2)) + (MAP_SIZE / 4);
    map[val][MAP_SIZE - 1].walls[2] = 1;
    map[val][MAP_SIZE - 1] .walls[3] = 0;
    map[val + 1][MAP_SIZE - 1].walls[0] = 1;
    map[val + 1][MAP_SIZE - 1].walls[3] = 0;
    map[val][MAP_SIZE - 2].walls[1] = 0;
    map[val][MAP_SIZE - 2].walls[2] = 0;
    map[val + 1][MAP_SIZE - 2].walls[0] = 0;
    map[val + 1][MAP_SIZE - 2].walls[1] = 0;
}

void map_init(MapNode_t **map)
{
    map = malloc_uncached(MAP_SIZE * sizeof(MapNode_t*));
    for (int i = 0; i < MAP_SIZE; i++) {
        map[i] = malloc_uncached(MAP_SIZE * sizeof(MapNode_t*));
    }
    generate_map(map, 60);
}

/**************************************************************
 * free_map()
 *
 * Description: Free the memory used by the game map
 *  
 *
 * @param map the game map to free
 * 
 *************************************************************/
void free_map(MapNode_t** map)
{
    for (int i = 0; i < MAP_SIZE; i++) {
        free_uncached(map[i]);
        map[i] = NULL;
    }
    free_uncached(map);
    map = NULL;
}
