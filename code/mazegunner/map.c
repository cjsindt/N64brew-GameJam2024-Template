#include <libdragon.h>
#include "map.h"
#include "../../core.h"


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
void draw_map(Map_T *map)
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
    for (int i = 0; i < MAP_SIZE; i++)
    {   
        for (int j = 0; j < MAP_SIZE - 1; j++)
        {
            // disp[row-1][col] = map->horizontal[row-1][((col+1)/2)-1] ? '_' : ' ';
            // disp[row][col+1] = map->vertical[row-1][((col+1)/2)-1] ? '|' : ' ';
            // disp[row][col] = map->horizontal[row-1][((col+1)/2)-1] ? '_' : ' ';
            // disp[row][col-1] = map->vertical[row-1][((col+1)/2)-1] ? '|' : ' ';

            //disp[]
            disp[i + 1][(j+1)*2] = map->vertical[i][j] ? '|' : ' ';
        }
    }

    for (int i = 1; i < MAP_SIZE; i++)
    {   
        for (int j = 1; j < size - 1; j++)
        {
            if (j%2 == 1) // odd - horizontal
            {
                disp[i][j] = map->horizontal[i-1][(j-1)/2] ? '_' : ' ';
            }
            else
            {
                disp[i][j] = map->vertical[i-1][(j-2)/2] ? '|' : ' ';
            }
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
 * generate_map()
 *
 * Description: Generate a new map.
 *  
 *
 * @param map where to store the game map
 * @param pruneFactor how strongly to prune the map [0,99]
 * 
 *************************************************************/
void generate_map(Map_T *map, uint8_t pruneFactor)
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
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE - 1; j++)
        {
            map->vertical[i][j] = 1;
            map->horizontal[j][i] = 1;
        }
    }

    for (int row = 0; row < MAP_SIZE; row++)
    {
        for (int col = 0; col < MAP_SIZE; col++)
        {
            switch (maze[row][col])
            {
                case 1:
                    map->horizontal[row - 1][col] = 0;
                    break;

                case 2:
                    map->vertical[row][col] = 0;
                    break;

                case 3:
                    map->horizontal[row ][col] = 0;
                    break;

                case 4:
                    map->vertical[row][col - 1] = 0;
                    break;
            }
        }
    }
    
    /* prune walls to open up map more */
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE - 1; j++)
        {   
            if (rand() % 100 < pruneFactor)
            {
                map->vertical[i][j] = 0;
            }
            if (rand() % 100 < pruneFactor)
            {
                map->horizontal[j][i] = 0;
            }
        }
    }

    
    /* remove line of sight from players on spawn                        */
    /* place a wall on each far wall of the maze at some random distance */
    /* to keep from sealing off players, open the adjacent walls         */

    /* top wall */
    //uint8_t val = rand() % ((MAP_SIZE - (MAP_SIZE / 4)) + 1 - (MAP_SIZE / 4)) + (MAP_SIZE / 4);
    uint8_t val = rand() % (MAP_SIZE - 3) + 2;
    map->vertical[0][val] = 1;
    map->vertical[1][val] = 0;
    map->horizontal[0][val] = 0;
    map->horizontal[0][val+1] = 0;

    // /* bottom wall */ 
    //val = rand() % ((MAP_SIZE - (MAP_SIZE / 4)) + 1 - (MAP_SIZE / 4)) + (MAP_SIZE / 4);
    val = rand() % (MAP_SIZE - 3) + 2;
    map->vertical[MAP_SIZE-1][val] = 1;
    map->vertical[MAP_SIZE-2][val] = 0;
    map->horizontal[MAP_SIZE-2][val] = 0;
    map->horizontal[MAP_SIZE-2][val+1] = 0;

    // /* left wall */
    //val = rand() % ((MAP_SIZE - (MAP_SIZE / 4)) + 1 - (MAP_SIZE / 4)) + (MAP_SIZE / 4);
    val = rand() % (MAP_SIZE - 3) + 2;
    map->horizontal[val][0] = 1;
    map->horizontal[val][1] = 0;
    map->vertical[val][0] = 0;
    map->vertical[val+1][0] = 0;

    // /* right wall */
    //val = rand() % ((MAP_SIZE - (MAP_SIZE / 4)) + 1 - (MAP_SIZE / 4)) + (MAP_SIZE / 4);
    val = rand() % (MAP_SIZE - 3) + 2;
    map->horizontal[val][MAP_SIZE-1] = 1;
    map->horizontal[val][MAP_SIZE-2] = 0;
    map->vertical[val][MAP_SIZE-2] = 0;
    map->vertical[val+1][MAP_SIZE-2] = 0;

}


/**************************************************************
 * destroy_map()
 *
 * Description: Free the memory used by the game map
 *  
 *
 * @param map the game map to destroy
 * 
 *************************************************************/
void destroy_map(Map_T *map)
{
    free_uncached(map->wallVertices);
    map->wallVertices = NULL;

    free_uncached(map->floorVertices);
    map->floorVertices = NULL;

    free_uncached(map->floorMatFP);
    map->floorMatFP = NULL;

    free_uncached(map->modelMatFP);
    map->modelMatFP = NULL;

    //rspq_block_free(map->dplDraw);
    //map->dplDraw = NULL;

    //rspq_block_free(map->dplFloor);
    //map->dplFloor = NULL;

    rspq_block_free(map->dplMap);
    map->dplMap = NULL;

    free_uncached(map);
    map = NULL;
}


/**************************************************************
 * total_walls()
 *
 * Description: Calculates the total number of walls in a map
 *  
 *
 * @param map the game map to count walls of
 * 
 *************************************************************/
int total_walls(Map_T *map)
{
    int total_walls = 0;    //MAP_SIZE * 4;

    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE - 1; j++)
        {
            total_walls += map->vertical[i][j];
            total_walls += map->horizontal[j][i];
        }
    }
    return total_walls;
}

/**************************************************************
 * map_init()
 *
 * Description: Initialize game map
 *  
 *
 * @param map where to store the game map
 * 
 *************************************************************/
void map_init(Map_T *map)
{
    /* Build the map */
    generate_map(map, (core_get_aidifficulty()+1)*30);

    /* T3D */
    t3d_mat4_identity(&(map->floorMat));
    t3d_mat4_identity(&(map->modelMat));
    map->modelMatFP = malloc_uncached(sizeof(T3DMat4FP));
    map->floorMatFP = malloc_uncached(sizeof(T3DMat4FP));
    map->floorVertices = malloc_uncached(sizeof(T3DVertPacked) * 2);

    /* Wall Vertices = 4 * (4 external walls) + 4 * (number of walls) */
    int wallVerticesSize = 16 + (total_walls(map) * 4);
    map->wallVertices = malloc_uncached(sizeof(T3DVertPacked) * wallVerticesSize);
    uint32_t wall_color = 0xAAAA88FF;
    uint16_t norm = t3d_vert_pack_normal(&(T3DVec3){{0, 0, 1}}); // normals are packed in a 5.6.5 format
    
    /* external walls */
    map->wallVertices[0] = (T3DVertPacked){
        .posA = {0, 0, 0},
        .rgbaA = wall_color,
        .normA = norm,
        .posB = {TOTAL_MAP_SIDE_LENGTH, 0, 0},
        .rgbaB = wall_color,
        .normB = norm,
    };
    map->wallVertices[1] = (T3DVertPacked){
        .posA = {TOTAL_MAP_SIDE_LENGTH, WALL_HEIGHT, 0},
        .rgbaA = wall_color,
        .normA = norm,
        .posB = {0, WALL_HEIGHT, 0},
        .rgbaB = wall_color,
        .normB = norm,
    };
    map->wallVertices[2] = (T3DVertPacked){
        .posA = {0, 0, WALL_THICKNESS},
        .rgbaA = wall_color,
        .normA = norm,
        .posB = {TOTAL_MAP_SIDE_LENGTH, 0, WALL_THICKNESS},
        .rgbaB = wall_color,
        .normB = norm,
    };
    map->wallVertices[3] = (T3DVertPacked){
        .posA = {TOTAL_MAP_SIDE_LENGTH, WALL_HEIGHT, WALL_THICKNESS},
        .rgbaA = wall_color,
        .normA = norm,
        .posB = {0, WALL_HEIGHT, WALL_THICKNESS},
        .rgbaB = wall_color,
        .normB = norm,
    };
    /* * */
    map->wallVertices[4] = (T3DVertPacked){
        .posA = {0, 0, 0},
        .rgbaA = wall_color,
        .normA = norm,
        .posB = {WALL_THICKNESS, 0, 0},
        .rgbaB = wall_color,
        .normB = norm,
    };
    map->wallVertices[5] = (T3DVertPacked){
        .posA = {WALL_THICKNESS, WALL_HEIGHT, 0},
        .rgbaA = wall_color,
        .normA = norm,
        .posB = {0, WALL_HEIGHT, 0},
        .rgbaB = wall_color,
        .normB = norm,
    };
    map->wallVertices[6] = (T3DVertPacked){
        .posA = {0, 0, TOTAL_MAP_SIDE_LENGTH},
        .rgbaA = wall_color,
        .normA = norm,
        .posB = {WALL_THICKNESS, 0, TOTAL_MAP_SIDE_LENGTH},
        .rgbaB = wall_color,
        .normB = norm,
    };
    map->wallVertices[7] = (T3DVertPacked){
        .posA = {WALL_THICKNESS, WALL_HEIGHT, TOTAL_MAP_SIDE_LENGTH},
        .rgbaA = wall_color,
        .normA = norm,
        .posB = {0, WALL_HEIGHT, WALL_THICKNESS},
        .rgbaB = wall_color,
        .normB = norm,
    };
    /* * */
    map->wallVertices[8] = (T3DVertPacked){
        .posA = {TOTAL_MAP_SIDE_LENGTH - WALL_THICKNESS, 0, 0},
        .rgbaA = wall_color,
        .normA = norm,
        .posB = {TOTAL_MAP_SIDE_LENGTH, 0, 0},
        .rgbaB = wall_color,
        .normB = norm,
    };
    map->wallVertices[9] = (T3DVertPacked){
        .posA = {TOTAL_MAP_SIDE_LENGTH, WALL_HEIGHT, 0},
        .rgbaA = wall_color,
        .normA = norm,
        .posB = {TOTAL_MAP_SIDE_LENGTH - WALL_THICKNESS, WALL_HEIGHT, 0},
        .rgbaB = wall_color,
        .normB = norm,
    };
    map->wallVertices[10] = (T3DVertPacked){
        .posA = {TOTAL_MAP_SIDE_LENGTH - WALL_THICKNESS, 0, TOTAL_MAP_SIDE_LENGTH},
        .rgbaA = wall_color,
        .normA = norm,
        .posB = {TOTAL_MAP_SIDE_LENGTH, 0, WALL_THICKNESS},
        .rgbaB = wall_color,
        .normB = norm,
    };
    map->wallVertices[11] = (T3DVertPacked){
        .posA = {TOTAL_MAP_SIDE_LENGTH, WALL_HEIGHT, TOTAL_MAP_SIDE_LENGTH},
        .rgbaA = wall_color,
        .normA = norm,
        .posB = {TOTAL_MAP_SIDE_LENGTH - WALL_THICKNESS, WALL_HEIGHT, WALL_THICKNESS},
        .rgbaB = wall_color,
        .normB = norm,
    };
    /* * */
    map->wallVertices[12] = (T3DVertPacked){
        .posA = {0, 0, TOTAL_MAP_SIDE_LENGTH},
        .rgbaA = wall_color,
        .normA = norm,
        .posB = {TOTAL_MAP_SIDE_LENGTH, 0, TOTAL_MAP_SIDE_LENGTH},
        .rgbaB = wall_color,
        .normB = norm,
    };
    map->wallVertices[13] = (T3DVertPacked){
        .posA = {TOTAL_MAP_SIDE_LENGTH, WALL_HEIGHT, TOTAL_MAP_SIDE_LENGTH},
        .rgbaA = wall_color,
        .normA = norm,
        .posB = {0, WALL_HEIGHT, TOTAL_MAP_SIDE_LENGTH},
        .rgbaB = wall_color,
        .normB = norm,
    };
    map->wallVertices[14] = (T3DVertPacked){
        .posA = {0, 0, TOTAL_MAP_SIDE_LENGTH - WALL_THICKNESS},
        .rgbaA = wall_color,
        .normA = norm,
        .posB = {TOTAL_MAP_SIDE_LENGTH, 0, TOTAL_MAP_SIDE_LENGTH - WALL_THICKNESS},
        .rgbaB = wall_color,
        .normB = norm,
    };
    map->wallVertices[15] = (T3DVertPacked){
        .posA = {TOTAL_MAP_SIDE_LENGTH, WALL_HEIGHT, TOTAL_MAP_SIDE_LENGTH - WALL_THICKNESS},
        .rgbaA = wall_color,
        .normA = norm,
        .posB = {0, WALL_HEIGHT, TOTAL_MAP_SIDE_LENGTH - WALL_THICKNESS},
        .rgbaB = wall_color,
        .normB = norm,
    };

    int wallIndex = 16;

    /* Vertical Walls */
    for (int row = 0; row < MAP_SIZE; row++)
    {
        for (int col = 0; col < MAP_SIZE - 1; col++)
        {
            if (map->vertical[row][col])
            {
                map->wallVertices[wallIndex++] = (T3DVertPacked){
                    .posA = {(WALL_THICKNESS + CELL_SIZE) * (col + 1), 0, ((WALL_THICKNESS + CELL_SIZE) * row)},
                    .rgbaA = wall_color,
                    .normA = norm,
                    .posB = {((WALL_THICKNESS + CELL_SIZE) * (col + 1)) + WALL_THICKNESS, 0, ((WALL_THICKNESS + CELL_SIZE) * row)},
                    .rgbaB = wall_color,
                    .normB = norm,
                };
                map->wallVertices[wallIndex++] = (T3DVertPacked){
                    .posA = {((WALL_THICKNESS + CELL_SIZE) * (col + 1)) + WALL_THICKNESS, WALL_HEIGHT, ((WALL_THICKNESS + CELL_SIZE) * row)},
                    .rgbaA = wall_color,
                    .normA = norm,
                    .posB = {(WALL_THICKNESS + CELL_SIZE) * (col + 1), WALL_HEIGHT, ((WALL_THICKNESS + CELL_SIZE) * row)},
                    .rgbaB = wall_color,
                    .normB = norm,
                };
                map->wallVertices[wallIndex++] = (T3DVertPacked){
                    .posA = {(WALL_THICKNESS + CELL_SIZE) * (col + 1), 0, ((WALL_THICKNESS + CELL_SIZE) * (row + 1) + WALL_THICKNESS)},
                    .rgbaA = wall_color,
                    .normA = norm,
                    .posB = {(WALL_THICKNESS + CELL_SIZE) * (col + 1) + WALL_THICKNESS, 0, ((WALL_THICKNESS + CELL_SIZE) * (row + 1) + WALL_THICKNESS)},
                    .rgbaB = wall_color,
                    .normB = norm,
                };
                map->wallVertices[wallIndex++] = (T3DVertPacked){
                    .posA = {(WALL_THICKNESS + CELL_SIZE) * (col + 1) + WALL_THICKNESS, WALL_HEIGHT, ((WALL_THICKNESS + CELL_SIZE) * (row + 1) + WALL_THICKNESS)},
                    .rgbaA = wall_color,
                    .normA = norm,
                    .posB = {(WALL_THICKNESS + CELL_SIZE) * (col + 1), WALL_HEIGHT, ((WALL_THICKNESS + CELL_SIZE) * (row + 1) + WALL_THICKNESS)},
                    .rgbaB = wall_color,
                    .normB = norm,
                };
            }
        }
    }

    /* Horizontal Walls */
    for (int row = 0; row < MAP_SIZE - 1; row++)
    {
        for (int col = 0; col < MAP_SIZE; col++)
        {
            if (map->horizontal[row][col])
            {
                map->wallVertices[wallIndex++] = (T3DVertPacked){
                    .posA = {((WALL_THICKNESS + CELL_SIZE) * col), 0, (WALL_THICKNESS + CELL_SIZE) * (row + 1)},
                    .rgbaA = wall_color,
                    .normA = norm,
                    .posB = {((WALL_THICKNESS + CELL_SIZE) * (col + 1)) + WALL_THICKNESS, 0, (WALL_THICKNESS + CELL_SIZE) * (row + 1)},
                    .rgbaB = wall_color,
                    .normB = norm,
                };
                map->wallVertices[wallIndex++] = (T3DVertPacked){
                    .posA = {((WALL_THICKNESS + CELL_SIZE) * (col + 1)) + WALL_THICKNESS, WALL_HEIGHT, (WALL_THICKNESS + CELL_SIZE) * (row + 1)},
                    .rgbaA = wall_color,
                    .normA = norm,
                    .posB = {((WALL_THICKNESS + CELL_SIZE) * col), WALL_HEIGHT, (WALL_THICKNESS + CELL_SIZE) * (row + 1)},
                    .rgbaB = wall_color,
                    .normB = norm,
                };
                map->wallVertices[wallIndex++] = (T3DVertPacked){
                    .posA = {((WALL_THICKNESS + CELL_SIZE) * col), 0, ((WALL_THICKNESS + CELL_SIZE) * (row + 1)) + WALL_THICKNESS},
                    .rgbaA = wall_color,
                    .normA = norm,
                    .posB = {((WALL_THICKNESS + CELL_SIZE) * (col + 1)) + WALL_THICKNESS, 0, ((WALL_THICKNESS + CELL_SIZE) * (row + 1)) + WALL_THICKNESS},
                    .rgbaB = wall_color,
                    .normB = norm,
                };
                map->wallVertices[wallIndex++] = (T3DVertPacked){
                    .posA = {((WALL_THICKNESS + CELL_SIZE) * (col + 1)) + WALL_THICKNESS, WALL_HEIGHT, ((WALL_THICKNESS + CELL_SIZE) * (row + 1)) + WALL_THICKNESS},
                    .rgbaA = wall_color,
                    .normA = norm,
                    .posB = {((WALL_THICKNESS + CELL_SIZE) * col), WALL_HEIGHT, ((WALL_THICKNESS + CELL_SIZE) * (row + 1)) + WALL_THICKNESS},
                    .rgbaB = wall_color,
                    .normB = norm,
                };
            }
        }
    }

    /* floor */
    map->floorVertices[0] = (T3DVertPacked) {
        .posA = {0, 0, 0},
        .rgbaA = 0x555555FF,
        .normA = norm,
        .posB = {TOTAL_MAP_SIDE_LENGTH, 0, 0},
        .rgbaB = 0x555555FF,
        .normB = norm
    };
    map->floorVertices[1] = (T3DVertPacked) {
        .posA = {0, 0, TOTAL_MAP_SIDE_LENGTH},
        .rgbaA = 0x555555FF,
        .normA = norm,
        .posB = {TOTAL_MAP_SIDE_LENGTH, 0, TOTAL_MAP_SIDE_LENGTH},
        .rgbaB = 0x555555FF,
        .normB = norm
    };

    rspq_block_begin();

    int wallsRemaining = total_walls(map);
    int wallVerticesIndex = 0;
    int vertLoad = 0;
    int triCounter = 0;
    int iteration = 0;
    while (wallsRemaining)
    {
        wallVerticesIndex = iteration * 32;
        /* 8 or more walls left to draw */
        if (wallsRemaining > 8)
        {
            vertLoad = 64;
            triCounter = 8;
            wallsRemaining -= 8;
        }
        else
        {
            vertLoad = wallsRemaining * 8;
            triCounter = wallsRemaining;
            wallsRemaining = 0;
        }
        t3d_matrix_push(map->modelMatFP);
        t3d_vert_load(&(map->wallVertices[wallVerticesIndex]), 0, vertLoad);
        t3d_matrix_pop(1);

        for (int i = 0; i < triCounter; i++)
        {
            t3d_tri_draw((i*8)+0, (i*8)+1, (i*8)+2); t3d_tri_draw((i*8)+0, (i*8)+2, (i*8)+3); // Front face
            t3d_tri_draw((i*8)+4, (i*8)+5, (i*8)+6); t3d_tri_draw((i*8)+4, (i*8)+6, (i*8)+7); // Back face
            t3d_tri_draw((i*8)+0, (i*8)+1, (i*8)+4); t3d_tri_draw((i*8)+1, (i*8)+4, (i*8)+5); // Bottom face
            t3d_tri_draw((i*8)+2, (i*8)+3, (i*8)+7); t3d_tri_draw((i*8)+2, (i*8)+6, (i*8)+7); // Top face
            t3d_tri_draw((i*8)+0, (i*8)+3, (i*8)+4); t3d_tri_draw((i*8)+3, (i*8)+4, (i*8)+7); // Left face
            t3d_tri_draw((i*8)+1, (i*8)+2, (i*8)+5); t3d_tri_draw((i*8)+2, (i*8)+5, (i*8)+6); // Right face
        }
        t3d_tri_sync();
        iteration++;
    }

    t3d_matrix_push(map->floorMatFP);
    t3d_vert_load(map->floorVertices, 0, 4);
    t3d_matrix_pop(1);

    t3d_tri_draw(0,1,2); t3d_tri_draw(1,2,3);

    t3d_tri_sync();

    map->dplMap = rspq_block_end();
}