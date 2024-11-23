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

surface_t *depthBuffer;
Map_T *gameMap;
T3DViewport viewport;
T3DVec3 rotAxis = {{0.5f, 1.0f, 0.0f}};;
float rotAngle;
uint8_t colorAmbient[4] = {50, 50, 50, 0xFF};
uint8_t colorDir[4] = {0xFF, 0xFF, 0xFF, 0xFF};
T3DVec3 lightDirVec = {{0.0f, 0.0f, 1.0f}};
const T3DVec3 camPos = {{0, 75, 18}};
const T3DVec3 camTarget = {{10, 0, 10}};

/*==============================
    minigame_init
    The minigame initialization function
==============================*/
void minigame_init()
{
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_RESAMPLE_ANTIALIAS);
    depthBuffer = display_get_zbuf();

    t3d_init((T3DInitParams){});

    T3DMat4 modelMat; // matrix for our model, this is a "normal" float matrix
    t3d_mat4_identity(&modelMat);
    
    viewport = t3d_viewport_create();

    //console_init();
    gameMap = malloc_uncached(sizeof(Map_T));
    map_init(gameMap);
    
    rotAngle = 0.0f;
    //T3DVec3 rotAxisVal = {{-1.0f, 2.5f, 0.25f}};
    //rotAxis = &rotAxisVal;
    t3d_vec3_norm(&rotAxis);
    // gameMap = malloc_uncached(MAP_SIZE * sizeof(MapNode_t*));
    // for (int i = 0; i < MAP_SIZE; i++) {
    //     gameMap[i] = malloc_uncached(MAP_SIZE * sizeof(MapNode_t*));
    // }
    //printf("Press A to draw new maze\n\n");
    //generate_map(gameMap, (core_get_aidifficulty()+1)*30);
    // draw_map(gameMap);
    // printf("\nTotal Walls: %d\nPrune Factor: %d", total_walls(gameMap), (core_get_aidifficulty()+1)*30);
    // printf("\nPress B to go back to menu\n");
    //printf("Total Walls: %d", total_walls(gameMap));
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
        // ======== Update ======== //
    rotAngle += 0.03f;

    // we can set up our viewport settings beforehand here
    t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(85.0f), 10.0f, 100.0f);
    t3d_viewport_look_at(&viewport, &camPos, &camTarget, &(T3DVec3){{0, 1, 0}});

    // Model-Matrix, t3d offers some basic matrix functions
    t3d_mat4_identity(&(gameMap->modelMat));
    t3d_mat4_rotate(&(gameMap->modelMat), &rotAxis, rotAngle);
    //t3d_mat4_scale(&(gameMap->modelMat), 0.4f, 0.4f, 0.4f);
    t3d_mat4_to_fixed(gameMap->modelMatFP, &(gameMap->modelMat));

    // Initialize the floor's model matrix
    t3d_mat4_identity(&(gameMap->floorMat));
    t3d_mat4_rotate(&(gameMap->floorMat), &rotAxis, rotAngle);
    t3d_mat4_translate(&(gameMap->floorMat), 0.0f, -1.0f, 0.0f); // Move the floor into place
    t3d_mat4_to_fixed(gameMap->floorMatFP, &(gameMap->floorMat));          // Update floorMatFP with the new floor matrix

    // ======== Draw (3D) ======== //
    rdpq_attach(display_get(), display_get_zbuf()); // set the target to draw to
    t3d_frame_start();                              // call this once per frame at the beginning of your draw function

    t3d_viewport_attach(&viewport); // now use the viewport, this applies proj/view matrices and sets scissoring

    rdpq_mode_combiner(RDPQ_COMBINER_SHADE);
    // this cleans the entire screen (even if out viewport is smaller)
    t3d_screen_clear_color(RGBA32(100, 0, 100, 0));
    t3d_screen_clear_depth();

    t3d_light_set_ambient(colorAmbient);                  // one global ambient light, always active
    t3d_light_set_directional(0, colorDir, &lightDirVec); // optional directional light, can be disabled
    t3d_light_set_count(1);

    t3d_state_set_drawflags(T3D_FLAG_SHADED | T3D_FLAG_DEPTH);

    // t3d_mat4fp_from_srt_euler(gameMap->modelMatFP,
    //                         (float[3]){0.125f, 0.125f, 0.125f},
    //                         (float[3]){0.0f, 0, 0},
    //                         (float[3]){0,0,0});

    //rspq_block_run(gameMap->dplDraw);

    //rspq_block_run(gameMap->dplFloor);

    rspq_block_run(gameMap->dplMap);

    rdpq_detach_show();

    //printf("Total Walls: %d", total_walls(gameMap));

    for (size_t i = 0; i < core_get_playercount(); i++)
    {
        // For human players, check if the physical A button on the controller was pressed
        joypad_buttons_t btn = joypad_get_buttons_pressed(core_get_playercontroller(i));
        // if (btn.a)
        // {
        //     console_clear();
        //     printf("Press A to draw new maze\n\n");
        //     generate_map(gameMap, (core_get_aidifficulty()+1)*30);
        //     draw_map(gameMap);
        //     printf("\nTotal Walls: %d\nPrune Factor: %d", total_walls(gameMap), (core_get_aidifficulty()+1)*30);
        //     printf("\nPress B to go back to menu\n");
        // }

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
    destroy_map(gameMap);
    t3d_destroy();
    display_close();
}