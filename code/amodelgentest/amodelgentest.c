#include <libdragon.h>
#include "../../minigame.h"
#include "../../core.h"
#include <t3d/t3d.h>
#include <t3d/t3dmath.h>
#include <t3d/t3dmodel.h>
#include <t3d/t3dskeleton.h>
#include <t3d/t3danim.h>
#include <t3d/t3ddebug.h>

surface_t *depthBuffer;
T3DMat4 modelMat; // matrix for our model, this is a "normal" float matrix
T3DMat4 floorMat;
T3DMat4FP *modelMatFP;
T3DMat4FP *floorMatFP;
rspq_block_t *dplDraw;
rspq_block_t *dplFloor;
T3DViewport viewport;
float rotAngle;
T3DVec3 rotAxis = {{-1.0f, 2.5f, 0.25f}};;
T3DVertPacked *wallVertices;
T3DVertPacked *floorVertices;
uint8_t colorAmbient[4] = {50, 50, 50, 0xFF};
uint8_t colorDir[4] = {0xFF, 0xFF, 0xFF, 0xFF};
T3DVec3 lightDirVec = {{0.0f, 0.0f, 1.0f}};
const T3DVec3 camPos = {{0, -100, -18}};
const T3DVec3 camTarget = {{0, 0, 0}};

const MinigameDef minigame_def = {
    .gamename = "Model Gen Test",
    .developername = "Taco Bell Labs",
    .description = "Test out generating models to easily make auto-generated maps.",
    .instructions = "Enjoy"};

void minigame_init(void)
{
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_RESAMPLE_ANTIALIAS);
    depthBuffer = display_get_zbuf();

    t3d_init((T3DInitParams){});
    
    t3d_mat4_identity(&modelMat);
    t3d_mat4_identity(&floorMat);
    // Now allocate a fixed-point matrix, this is what t3d uses internally.
    modelMatFP = malloc_uncached(sizeof(T3DMat4FP));
    floorMatFP = malloc_uncached(sizeof(T3DMat4FP));

    t3d_vec3_norm(&lightDirVec);

    // Allocate vertices (make sure to have an uncached pointer before passing it to the API!)
    // For performance reasons, 'T3DVertPacked' contains two vertices at once in one struct.
    wallVertices = malloc_uncached(sizeof(T3DVertPacked) * 4);
    floorVertices = malloc_uncached(sizeof(T3DVertPacked) * 2);

    uint32_t wall_color = 0xAAAA88FF;
    uint16_t norm = t3d_vert_pack_normal(&(T3DVec3){{0, 0, 1}}); // normals are packed in a 5.6.5 format
    wallVertices[0] = (T3DVertPacked){
        .posA = {-24, -16, 0},
        .rgbaA = wall_color,
        .normA = norm,
        .posB = {24, -16, 0},
        .rgbaB = wall_color,
        .normB = norm,
    };
    wallVertices[1] = (T3DVertPacked){
        .posA = {24, 16, 0},
        .rgbaA = wall_color,
        .normA = norm,
        .posB = {-24, 16, 0},
        .rgbaB = wall_color,
        .normB = norm,
    };
    wallVertices[2] = (T3DVertPacked){
        .posA = {-24, -16, 5},
        .rgbaA = wall_color,
        .normA = norm,
        .posB = {24, -16, 5},
        .rgbaB = wall_color,
        .normB = norm,
    };
    wallVertices[3] = (T3DVertPacked){
        .posA = {24, 16, 5},
        .rgbaA = wall_color,
        .normA = norm,
        .posB = {-24, 16, 5},
        .rgbaB = wall_color,
        .normB = norm,
    };
    floorVertices[0] = (T3DVertPacked) {
        .posA = {-100, 0, -100},
        .rgbaA = 0x555555FF,
        .normA = norm,
        .posB = {100, 0, -100},
        .rgbaB = 0x555555FF,
        .normB = norm
    };
    floorVertices[1] = (T3DVertPacked) {
        .posA = {-100, 0, 100},
        .rgbaA = 0x555555FF,
        .normA = norm,
        .posB = {100, 0, 100},
        .rgbaB = 0x555555FF,
        .normB = norm
    };

    rotAngle = 0.0f;
    //T3DVec3 rotAxisVal = {{-1.0f, 2.5f, 0.25f}};
    //rotAxis = &rotAxisVal;
    t3d_vec3_norm(&rotAxis);

    // create a viewport, this defines the section to draw to (by default the whole screen)
    // and contains the projection & view (camera) matrices
    viewport = t3d_viewport_create();

    //dplDraw = NULL;
    dplFloor = NULL;
}

void minigame_fixedloop(float deltaTime)
{
}

void minigame_loop(float deltaTime)
{
    // ======== Update ======== //
    rotAngle += 0.03f;

    // we can set up our viewport settings beforehand here
    t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(85.0f), 10.0f, 100.0f);
    t3d_viewport_look_at(&viewport, &camPos, &camTarget, &(T3DVec3){{0, 1, 0}});

    // Model-Matrix, t3d offers some basic matrix functions
    t3d_mat4_identity(&modelMat);
    t3d_mat4_rotate(&modelMat, &rotAxis, rotAngle);
    t3d_mat4_scale(&modelMat, 0.4f, 0.4f, 0.4f);
    t3d_mat4_to_fixed(modelMatFP, &modelMat);

    // Initialize the floor's model matrix
    t3d_mat4_identity(&floorMat);
    t3d_mat4_rotate(&floorMat, &rotAxis, rotAngle);
    t3d_mat4_translate(&floorMat, 0.0f, -1.0f, 0.0f); // Move the floor into place
    t3d_mat4_to_fixed(floorMatFP, &floorMat);          // Update floorMatFP with the new floor matrix

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

    // t3d functions can be recorded into a display list:
    if (!dplDraw)
    {
        rspq_block_begin();

        t3d_matrix_push(modelMatFP);   // Matrix load can be recorded as they DMA the data in internally
        t3d_vert_load(wallVertices, 0, 8); // load 4 vertices...
        t3d_matrix_pop(1);             // ...and pop the matrix, this can be done as soon as the vertices are loaded...
        //t3d_tri_draw(0, 1, 2);         // ...then draw 2 triangles
        //t3d_tri_draw(2, 3, 0);
        // Draw the triangles for each face
        t3d_tri_draw(0, 1, 2); t3d_tri_draw(0, 2, 3); // Front face
        t3d_tri_draw(4, 5, 6); t3d_tri_draw(4, 6, 7); // Back face
        t3d_tri_draw(0, 1, 4); t3d_tri_draw(1, 4, 5); // Bottom face
        t3d_tri_draw(2, 3, 7); t3d_tri_draw(2, 6, 7); // Top face
        t3d_tri_draw(0, 3, 4); t3d_tri_draw(3, 4, 7); // Left face
        t3d_tri_draw(1, 2, 5); t3d_tri_draw(2, 5, 6); // Right face

        // NOTE: if you use the builtin model format, syncs are handled automatically!
        t3d_tri_sync(); // after each batch of triangles, a sync is needed
        // technically, you only need a sync before any new 't3d_vert_load', rdpq call, or after the last triangle
        // for safety, just call it after you are done with all triangles after a load

        dplDraw = rspq_block_end();
    }

    t3d_mat4fp_from_srt_euler(modelMatFP,
                            (float[3]){0.125f, 0.125f, 0.125f},
                            (float[3]){0.0f, 0, 0},
                            (float[3]){0,0,0});

    rspq_block_run(dplDraw);

    if (!dplFloor) {
        rspq_block_begin();
        t3d_matrix_push(floorMatFP);
        t3d_vert_load(floorVertices, 0, 4);
        t3d_matrix_pop(1);

        t3d_tri_draw(0,1,2); t3d_tri_draw(1,2,3);

        t3d_tri_sync();

        dplFloor = rspq_block_end();
        
    }

    rspq_block_run(dplFloor);

    rdpq_detach_show();
}

void minigame_cleanup(void)
{
    
}