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
T3DViewport viewports[MAXPLAYERS];
T3DVec3 rotAxis = {{0.5f, 1.0f, 0.0f}};;
float rotAngle;
uint8_t colorAmbient[4] = {50, 50, 50, 0xFF};
uint8_t colorDir[4] = {0xFF, 0xFF, 0xFF, 0xFF};
T3DVec3 lightDirVec = {{0.0f, 0.0f, 1.0f}};
// const T3DVec3 camPos = {{0, 75, 18}};
// const T3DVec3 camTarget = {{10, 0, 10}};
T3DVec3 camPos;
T3DVec3 camTarget;
int sizeX;
int sizeY;

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

    sizeX = display_get_width();
    sizeY = display_get_height();

    viewports[0] = t3d_viewport_create();
    viewports[1] = t3d_viewport_create();
    viewports[2] = t3d_viewport_create();
    viewports[3] = t3d_viewport_create();
    t3d_viewport_set_area(&viewports[0], 0, 0, sizeX / 2, sizeY / 2);                 // top left
    t3d_viewport_set_area(&viewports[1], sizeX / 2, 0, sizeX / 2, sizeY / 2);         // top right
    t3d_viewport_set_area(&viewports[2], 0, sizeY / 2, sizeX / 2, sizeY / 2);         // bottom left
    t3d_viewport_set_area(&viewports[3], sizeX / 2, sizeY / 2, sizeX / 2, sizeY / 2); // bottom right

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
    //rotAngle += 0.03f;

    // we can set up our viewport settings beforehand here
    //t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(60.0f), 10.0f, 100.0f);
    //t3d_viewport_look_at(&viewport, &camPos, &camTarget, &(T3DVec3){{0, 1, 0}});

    // Model-Matrix, t3d offers some basic matrix functions
    t3d_mat4_identity(&(gameMap->modelMat));
    //t3d_mat4_rotate(&(gameMap->modelMat), &rotAxis, rotAngle);
    //t3d_mat4_scale(&(gameMap->modelMat), 0.4f, 0.4f, 0.4f);
    t3d_mat4_to_fixed(gameMap->modelMatFP, &(gameMap->modelMat));

    // Initialize the floor's model matrix
    t3d_mat4_identity(&(gameMap->floorMat));
    //t3d_mat4_rotate(&(gameMap->floorMat), &rotAxis, rotAngle);
    t3d_mat4_translate(&(gameMap->floorMat), 0.0f, -1.0f, 0.0f); // Move the floor into place
    t3d_mat4_to_fixed(gameMap->floorMatFP, &(gameMap->floorMat));          // Update floorMatFP with the new floor matrix

    // ======== Draw (3D) ======== //
    rdpq_attach(display_get(), display_get_zbuf()); // set the target to draw to
    t3d_frame_start();                              // call this once per frame at the beginning of your draw function

    //t3d_viewport_attach(&viewport); // now use the viewport, this applies proj/view matrices and sets scissoring

    rdpq_mode_combiner(RDPQ_COMBINER_SHADE);
    // this cleans the entire screen (even if out viewport is smaller)
    t3d_screen_clear_color(RGBA32(100, 0, 100, 0));
    t3d_screen_clear_depth();

    t3d_light_set_ambient(colorAmbient);                  // one global ambient light, always active
    t3d_light_set_directional(0, colorDir, &lightDirVec); // optional directional light, can be disabled
    t3d_light_set_count(1);

    for (int v = 0; v < MAXPLAYERS; v++)
    {
        T3DViewport *vp = &viewports[v];
        float distance = 50.0f; // Distance behind the player
        float height = 40.0f;   // Height above the player

        // Calculate the position of the camera behind the player based on their rotation
        // camPos = (T3DVec3){{players[v].playerPos.v[0] - fm_cosf(players[v].rotY) * distance,
        //                     players[v].playerPos.v[1] + height,
        //                     players[v].playerPos.v[2] + fm_sinf(players[v].rotY) * distance}};

        camPos = (T3DVec3){{(WALL_THICKNESS + (CELL_SIZE / 2)),
                            WALL_HEIGHT / 2,
                            (WALL_THICKNESS + (CELL_SIZE / 2))}};

        // Calculate the target position slightly ahead of the player based on their rotation
        // camTarget = (T3DVec3){{players[v].playerPos.v[0] + fm_sinf(players[v].rotY),
        //                     players[v].playerPos.v[1] + 9.0f, // Adjust height to center target around player's head/upper body
        //                     players[v].playerPos.v[2] + fm_cosf(players[v].rotY)}};

        camTarget = (T3DVec3){{TOTAL_MAP_SIDE_LENGTH / 2,
                                WALL_HEIGHT / 2,
                                TOTAL_MAP_SIDE_LENGTH / 2}};
        // Like in all other examples, set up the projection (only really need to do it once) and view matrix here
        // after that apply the viewport and draw your scene
        // Since each of the viewport-structs has its own matrices, no conflicts will occur
        t3d_viewport_set_projection(vp, T3D_DEG_TO_RAD(60.0f), 2.0f, 200.0f);
        t3d_viewport_look_at(vp, &camPos, &camTarget, &(T3DVec3){{0, 1, 0}});
        t3d_viewport_attach(vp);

        rspq_block_run(gameMap->dplMap);

    }

    // ======== Draw (2D) ======== //
    rdpq_sync_pipe();
    rdpq_set_scissor(0, 0, sizeX, sizeY);
    rdpq_set_mode_standard();
    rdpq_set_mode_fill(RGBA32(0, 0, 0, 0xFF));

    // draw thick lines between the screens
    rdpq_fill_rectangle(0, sizeY/2-1, sizeX, sizeY/2+1);
    rdpq_fill_rectangle(sizeX/2-1, 0, sizeX/2+1, sizeY);

    t3d_state_set_drawflags(T3D_FLAG_SHADED | T3D_FLAG_DEPTH);

    // t3d_mat4fp_from_srt_euler(gameMap->modelMatFP,
    //                         (float[3]){0.125f, 0.125f, 0.125f},
    //                         (float[3]){0.0f, 0, 0},
    //                         (float[3]){0,0,0});

    //rspq_block_run(gameMap->dplDraw);

    //rspq_block_run(gameMap->dplFloor);



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