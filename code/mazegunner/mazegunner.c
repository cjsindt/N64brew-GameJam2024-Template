/* ------------------------------------------------------------
    helloworld.c
------------------------------------------------------------ */

#include <libdragon.h>
#include "../../core.h"
#include "../../minigame.h"
#include "map.h"

const MinigameDef minigame_def = {
    .gamename = "Maze Gunner",
    .developername = "Taco Bell Labs",
    .description = "Find your opponents and be the last one standing!",
    .instructions = "A to shoot. Stick to move."
};

Map_T *GameMap;

/*==============================
    minigame_init
    The minigame initialization function
==============================*/
void minigame_init()
{
    // display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_RESAMPLE_ANTIALIAS);
    // depthBuffer = display_get_zbuf();

    // t3d_init((T3DInitParams){});

    // T3DMat4 modelMat; // matrix for our model, this is a "normal" float matrix
    // t3d_mat4_identity(&modelMat);
    
    // viewport = t3d_viewport_create();

    console_init();
    gameMap = malloc_uncached(sizeof(Map_T*));
    // gameMap = malloc_uncached(MAP_SIZE * sizeof(MapNode_t*));
    // for (int i = 0; i < MAP_SIZE; i++) {
    //     gameMap[i] = malloc_uncached(MAP_SIZE * sizeof(MapNode_t*));
    // }
    printf("Press A to draw new maze\n\n");
    generate_map(gameMap, 30);
    draw_map(gameMap);
    printf("\nTotal Walls: %d\n", total_walls(gameMap));
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
            generate_map(gameMap, 30);
            draw_map(gameMap);
            printf("\nTotal Walls: %d\n", total_walls(gameMap));
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
    free_map(gameMap);
    console_clear();
    console_close();
}