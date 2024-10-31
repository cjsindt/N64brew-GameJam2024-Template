// #include <stdio.h>
// #include <stdlib.h>
// #include <limits.h>
// #include <time.h>

// #define MAZE_SIZE 5                                
// #define MIN_NEIGHBOR(a, b, c, d)   
//     ((a) <= (b) && (a) <= (c) && (a) <= (d) ? 1 : 
//      (b) <= (a) && (b) <= (c) && (b) <= (d) ? 2 : 
//      (c) <= (a) && (c) <= (b) && (c) <= (d) ? 3 : 4)

// typedef struct node_t {
//     int id;
//     int weight;
//     char active;
// } node;

// int ** gen_maze()
// {
//     node nodes[MAZE_SIZE * MAZE_SIZE];
    
//     int** map = malloc(MAZE_SIZE * sizeof(int*));
//     for (int i = 0; i < MAZE_SIZE; i++) {
//         map[i] = malloc(MAZE_SIZE * sizeof(int));
//     }
    
//     for (int i = 0; i < MAZE_SIZE * MAZE_SIZE; i++)
//     {
//         nodes[i].id = i;
//         nodes[i].weight = rand() % 100;
//         nodes[i].active = 1;
//     }
    
//     for (int row = 0; row < MAZE_SIZE; row++)
//     {
//         for (int col = 0; col < MAZE_SIZE; col++)
//         {
//             printf("%d ", nodes[(row*MAZE_SIZE) + col].weight);
//         }
//         printf("\n");
//     }
//     printf("\n");
    
//     int source = rand() % (MAZE_SIZE * MAZE_SIZE);
    
//     nodes[source].active = 0;
//     int min_weight = INT_MAX;
//     int min_id = INT_MAX;
//     int map_val = 0;
    
//     for (int i = 0; i < (MAZE_SIZE * MAZE_SIZE) - 1; i++)
//     {
//         //printf("i: %d\n", i);
//         min_weight = INT_MAX;
//         min_id = INT_MAX;
//         map_val = 0;
//         for (int j = 0; j < MAZE_SIZE * MAZE_SIZE; j++)
//         {
//             //printf("j: %d\n", j);
//             if (nodes[j].active == 0)
//             {
//                 // check weight "above"
//                 //printf("above map_val: %d\n", map_val);
//                 if ((j > MAZE_SIZE - 1) && nodes[j - MAZE_SIZE].active && nodes[j - MAZE_SIZE].weight < min_weight)
//                 {
//                     //printf("piss\n");
//                     min_id = j - MAZE_SIZE;
//                     min_weight = nodes[j - MAZE_SIZE].weight;
//                     map_val = 3;
//                 }
//                 //printf("right map_va: %d\n", map_val);
//                 if ((j % (MAZE_SIZE) != (MAZE_SIZE - 1)) && nodes[j+1].active && nodes[j+1].weight < min_weight)
//                 {
//                     //printf("piss1\n");
//                     min_id = j+1;
//                     min_weight = nodes[j+1].weight;
//                     map_val = 4;
//                 }
//                 //printf("below map_val: %d\n", map_val);
//                 if ((j < (MAZE_SIZE * MAZE_SIZE) - MAZE_SIZE) && nodes[j + MAZE_SIZE].active && nodes[j + MAZE_SIZE].weight < min_weight)
//                 {
//                     //printf("piss2\n");
//                     min_id = j + MAZE_SIZE;
//                     min_weight = nodes[j + MAZE_SIZE].weight;
//                     map_val = 1;
//                 }
//                 //printf("left map_val: %d\n", map_val);
//                 if ((j % MAZE_SIZE != 0) && nodes[j-1].active && nodes[j-1].weight < min_weight)
//                 {
//                     //printf("piss3\n");
//                     min_id = j-1;
//                     min_weight = nodes[j-1].weight;
//                     map_val = 2;
//                 }
//             }
//         }
//         //printf("here %d %d\n", min_id / MAZE_SIZE, min_id % MAZE_SIZE);
//         nodes[min_id].active = 0;
//         map[min_id / MAZE_SIZE][min_id % MAZE_SIZE] = map_val;
//         //printf("here also\n");
//     }
    
//     int dist[4] = {0};
    
//     for (int row = 0; row < MAZE_SIZE; row++)
//     {
//         for (int col = 0; col < MAZE_SIZE; col++)
//         {
//             dist[map[row][col]-1]++;
//             printf("%d ", map[row][col]);
//         }
//         printf("\n");
//     }
    
//     printf("1: %d\t2: %d\t3: %d\t4: %d\n", dist[0], dist[1], dist[2], dist[3]);
    
//     return map;
    
// }

// void free_maze(int** maze) {
//     for (int i = 0; i < MAZE_SIZE; i++) {
//         free(maze[i]);
//     }
//     free(maze);
// }

// /*==============================
//     generate_maze
//     Maze generation function
// ==============================*/
// int (*generate_maze())[MAZE_SIZE]
// {
//     // initialize maze array
//     int weights[MAZE_SIZE][MAZE_SIZE] = {0};
//     static int edges[MAZE_SIZE][MAZE_SIZE] = {0};
    
//     for (int row = 0; row < MAZE_SIZE; row++)
//     {
//         for (int col = 0; col < MAZE_SIZE; col++)
//         {
//             weights[row][col] = rand() % INT_MAX;   // assure range is [0, INT_MAX)
//             //printf("%d\n", weights[row][col]);
//         }
//     }
    
//     for (int row = 0; row < MAZE_SIZE; row++)
//     {
//         for (int col = 0; col < MAZE_SIZE; col++)
//         {
//             int a = (row > 0) ? weights[row - 1][col] : INT_MAX;
//             int b = (col < MAZE_SIZE - 1) ? weights[row][col + 1] : INT_MAX;
//             int c = (row < MAZE_SIZE - 1) ? weights[row + 1][col] : INT_MAX;
//             int d = (col > 0) ? weights[row][col - 1] : INT_MAX;
            
//             edges[row][col] = MIN_NEIGHBOR(a, b, c, d);
//         }
//     }
    
//     for (int row = 0; row < MAZE_SIZE; row++)
//     {
//         for (int col = 0; col < MAZE_SIZE; col++)
//         {
//             printf("%d ", edges[row][col]);
//         }
//         printf("\n");
//     }
    
//     return edges;

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
//             maze[row][col++] = (row == 0) ? ' ' : ((edges[row-1][i/2 - 1] == 2) || (edges[row-1][i/2] == 4)) && (col != 0) ? ' ' : '|';
//             //printf("%d %d\n", row, i);
//             maze[row][col++] = (row != 0) && (row != MAZE_SIZE) && ((edges[row-1][i/2] == 3) || (edges[row][i/2] == 1)) ? ' ' : '_';
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

// int main() {
//     srand(time(0));
//     int **edges = gen_maze();
//     // Write C code here
//     //int (*edges)[MAZE_SIZE] = generate_maze();
    
    
//     draw_maze(edges);
//     return 0;
// }