/* ------------------------------------------------------------
    helloworld.c
------------------------------------------------------------ */

#include <libdragon.h>
#include "../../core.h"
#include "../../minigame.h"

#include <stdlib.h>

/* ---------- MACROS ---------- */
#define MAZE_SIZE 16                                    // maze is a square of length MAZE_SIZE
#define MIN_NEIGHBOR(a, b, c, d)   \
    ((a) <= (b) && (a) <= (c) && (a) <= (d) ? 1 : \
     (b) <= (a) && (b) <= (c) && (b) <= (d) ? 2 : \
     (c) <= (a) && (c) <= (b) && (c) <= (d) ? 3 : 4)


const MinigameDef minigame_def = {
    .gamename = "Example Game",
    .developername = "Your Name",
    .description = "This is an example game.",
    .instructions = "Press A to win."
};

/*==============================
    generate_maze
    Maze generation function
==============================*/
void generate_maze_edges()
{
    // initialize maze array
    int maze[MAZE_SIZE][MAZE_SIZE] = {0};
    for (int i = 0; i < MAZE_SIZE; i++)
    {
        for (int j = 0; j < MAZE_SIZE; j++)
        {
            maze[i][j] = rand();
        }
    }


}

void draw_maze(int (*edges)[MAZE_SIZE])
{
    int size = (2 * MAZE_SIZE) + 1;
    char maze[MAZE_SIZE + 1][size];
    
    // initialize maze printout
    for (int row = 0; row < MAZE_SIZE + 1; row++)
    {   
        for (int col = 0; col < size - 1;)
        {
            int i = col;
            //printf("%d\n", i);
            maze[row][col++] = (row == 0) ? ' ' : ((edges[row-1][i/2 - 1] == 2) || (edges[row-1][i/2] == 4)) && (col != 0) ? ' ' : '|';
            //printf("%d %d\n", row, i);
            maze[row][col++] = (row != 0) && (row != MAZE_SIZE) && ((edges[row-1][i/2] == 3) || (edges[row][i/2] == 1)) ? ' ' : '_';
        }
        maze[row][size - 1] = (row == 0) ? ' ' : '|';
    }
    
    // print maze
    for (int row = 0; row <  MAZE_SIZE + 1; row++)
    {
        for (int col = 0; col < size; col++)
        {
            printf("%c", maze[row][col]);
        }
        printf("\n");
    }
}

/*==============================
    minigame_init
    The minigame initialization function
==============================*/
void minigame_init()
{

}

/*==============================
    minigame_fixedloop
    Code that is called every loop, at a fixed delta time.
    Use this function for stuff where a fixed delta time is 
    important, like physics.
    @param  The fixed delta time for this tick
==============================*/
void minigame_fixedloop(float deltatime)
{

}

/*==============================
    minigame_loop
    Code that is called every loop.
    @param  The delta time for this tick
==============================*/
void minigame_loop(float deltatime)
{

}

/*==============================
    minigame_cleanup
    Clean up any memory used by your game just before it ends.
==============================*/
void minigame_cleanup()
{

}