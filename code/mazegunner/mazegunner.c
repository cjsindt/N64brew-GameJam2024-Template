/* ------------------------------------------------------------
    helloworld.c
------------------------------------------------------------ */

#include <libdragon.h>
#include "../../core.h"
#include "../../minigame.h"
#include "maze.h"
#include <t3d/t3d.h>
#include <t3d/t3dmath.h>
#include <t3d/t3dmodel.h>
#include <t3d/t3dskeleton.h>
#include <t3d/t3danim.h>
#include <t3d/t3ddebug.h>

const MinigameDef minigame_def = {
    .gamename = "Maze Gunner",
    .developername = "Taco Bell Labs",
    .description = "Find your opponents and be the last one standing!",
    .instructions = "A to shoot. Stick to move."
};

uint8_t** maze;
surface_t *depthBuffer;
T3DViewport viewport;
T3DMat4FP* mapMatFP;
rspq_block_t *dplMap;
T3DModel *model;
T3DVec3 camPos;
T3DVec3 camTarget;
T3DVec3 lightDirVec;

/*==============================
    minigame_init
    The minigame initialization function
==============================*/
void minigame_init()
{
	const color_t colors[] = {
		PLAYERCOLOR_1,
		PLAYERCOLOR_2,
		PLAYERCOLOR_3,
		PLAYERCOLOR_4,
	};

	display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_RESAMPLE_ANTIALIAS);
	depthBuffer = display_get_zbuf();

	t3d_init((T3DInitParams){});

	viewport = t3d_viewport_create();

	mapMatFP = malloc_uncached(sizeof(T3DMat4FP));
	t3d_mat4fp_from_srt_euler(mapMatFP, (float[3]){0.3f, 0.3f, 0.3f}, (float[3]){0, 0, 0}, (float[3]){0, 0, -10});

	camPos = (T3DVec3){{125.0f, 125.0f, 200.0f}};
	camTarget = (T3DVec3){{0, 0, 40}};

	lightDirVec = (T3DVec3){{1.0f, 1.0f, 1.0f}};
	t3d_vec3_norm(&lightDirVec);

	model = t3d_model_load("rom:/mazegunner/RedPlayerF64.t3dm");
	t3d_model_draw(model);
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
	uint8_t colorAmbient[4] = {0xAA, 0xAA, 0xAA, 0xFF};
	uint8_t colorDir[4]     = {0xFF, 0xAA, 0xAA, 0xFF};

	t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(90.0f), 20.0f, 160.0f);
	t3d_viewport_look_at(&viewport, &camPos, &camTarget, &(T3DVec3){{0,1,0}});

    for (size_t i = 0; i < core_get_playercount(); i++)
    {
        // For human players, check if the physical A button on the controller was pressed
        joypad_buttons_t btn = joypad_get_buttons_pressed(core_get_playercontroller(i));
        if (btn.a)
        {
			minigame_end();
        }

        if (btn.b)
        {
            minigame_end();
        }
    }

	rdpq_attach(display_get(), depthBuffer);
	t3d_frame_start();
	t3d_viewport_attach(&viewport);

	t3d_screen_clear_color(RGBA32(224, 180, 96, 0xFF));
	t3d_screen_clear_depth();

	t3d_light_set_ambient(colorAmbient);
	t3d_light_set_directional(0, colorDir, &lightDirVec);
	t3d_light_set_count(1);

	model = t3d_model_load("rom:/mazegunner/RedPlayerF64.t3dm");
	t3d_model_draw(model);

	rdpq_sync_tile();
	rdpq_sync_pipe();

	rdpq_detach_show();
}

/*==============================
    minigame_cleanup
    Clean up any memory used by your game just before it ends.
==============================*/
void minigame_cleanup()
{
    freeMaze(maze);
    console_clear();
    console_close();
}
