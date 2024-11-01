/* ------------------------------------------------------------
    helloworld.c
------------------------------------------------------------ */

#include <libdragon.h>
#include "../../core.h"
#include "../../minigame.h"
#include "maze.h"

const MinigameDef minigame_def = {
    .gamename = "Maze Gunner",
    .developername = "Taco Bell Labs",
    .description = "Find your opponents and be the last one standing!",
    .instructions = "A to shoot. Stick to move."
};

node** GameMap;

/*==============================
    minigame_init
    The minigame initialization function
==============================*/
void minigame_init()
{
    GameMap = malloc_uncached(MAP_SIZE * sizeof(node*));
    for (int i = 0; i < MAP_SIZE; i++) {
        GameMap[i] = malloc_uncached(MAP_SIZE * sizeof(node*));
    }
    console_init();
    printf("Press A to draw new maze\n\n");
    generateMap(GameMap, 60);
    drawMap(GameMap);
    printf("\nPress B to go back to menu\n");
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
    for (size_t i = 0; i < core_get_playercount(); i++)
    {
        // For human players, check if the physical A button on the controller was pressed
        joypad_buttons_t btn = joypad_get_buttons_pressed(core_get_playercontroller(i));
        if (btn.a)
        {
            console_clear();
            printf("Press A to draw new maze\n\n");
            generateMap(GameMap, 60);
            drawMap(GameMap);
            printf("\nPress B to go back to menu\n");
        }

        if (btn.b)
        {
            minigame_end();
        }
    }
}

/*==============================
    minigame_cleanup
    Clean up any memory used by your game just before it ends.
==============================*/
void minigame_cleanup()
{
    freeMap(GameMap);
    console_clear();
    console_close();
}