#include <libdragon.h>
#include "../../minigame.h"
#include "../../core.h"
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
    .instructions = "A to shoot. Stick to move."};

surface_t *depthBuffer;
T3DViewport viewports[MAXPLAYERS];
T3DMat4FP *mapMatFP;
rspq_block_t *dplMap;
T3DModel *modelPlayers[MAXPLAYERS];
T3DModel *modelMap;
T3DVec3 camPos;
T3DVec3 camTarget;
T3DVec3 lightDirVec;
int sizeX;
int sizeY;

typedef struct
{
  PlyNum plynum;
  T3DMat4FP *modelMatFP;
  rspq_block_t *dplSnake;
  T3DVec3 moveDir;
  T3DVec3 playerPos;
  float rotY;
  float currSpeed;
  bool isAttack;
  bool isAlive;
  float attackTimer;
  PlyNum ai_target;
  int ai_reactionspeed;
  color_t color;
} player_data;

player_data players[MAXPLAYERS];

rspq_syncpoint_t syncPoint;

void player_init(player_data *player, color_t color, T3DVec3 position, float rotation)
{
  player->modelMatFP = malloc_uncached(sizeof(T3DMat4FP));

  player->moveDir = (T3DVec3){{0, 0, 0}};
  player->playerPos = position;
  player->color = color;

  rspq_block_begin();
  t3d_matrix_push(player->modelMatFP);
  rdpq_set_prim_color(color);
  t3d_model_draw(modelPlayers[player->plynum]);
  t3d_matrix_pop(1);
  player->dplSnake = rspq_block_end();

  player->rotY = rotation;
  player->currSpeed = 0.0f;
  player->isAttack = false;
  player->isAlive = true;
  player->ai_target = rand() % MAXPLAYERS;
  player->ai_reactionspeed = (2 - core_get_aidifficulty()) * 5 + rand() % ((3 - core_get_aidifficulty()) * 3);
}

void minigame_init(void)
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

  mapMatFP = malloc_uncached(sizeof(T3DMat4FP));
  t3d_mat4fp_from_srt_euler(mapMatFP, (float[3]){0.3f, 0.3f, 0.3f}, (float[3]){0, 0, 0}, (float[3]){0, 0, -10});

  camPos = (T3DVec3){{0, 125.0f, 100.0f}};
  camTarget = (T3DVec3){{0, 0, 40}};

  lightDirVec = (T3DVec3){{1.0f, 1.0f, 1.0f}};
  t3d_vec3_norm(&lightDirVec);

  modelMap = t3d_model_load("rom:/snake3d/map.t3dm");

  modelPlayers[0] = t3d_model_load("rom:/mazegunner/RedPlayerF64.t3dm");
  modelPlayers[1] = t3d_model_load("rom:/mazegunner/BluePlayerF64.t3dm");
  modelPlayers[2] = t3d_model_load("rom:/mazegunner/GreenPlayerF64.t3dm");
  modelPlayers[3] = t3d_model_load("rom:/mazegunner/YellowPlayerF64.t3dm");

  rspq_block_begin();
  t3d_matrix_push(mapMatFP);
  rdpq_set_prim_color(RGBA32(255, 255, 255, 255));
  t3d_model_draw(modelMap);
  t3d_matrix_pop(1);
  dplMap = rspq_block_end();

  float z_start_pos = 5.0f;

  T3DVec3 start_positions[] = {
      (T3DVec3){{-100, z_start_pos, 0}},
      (T3DVec3){{0, z_start_pos, -100}},
      (T3DVec3){{100, z_start_pos, 0}},
      (T3DVec3){{0, z_start_pos, 100}},
  };

  float start_rotations[] = {
    M_PI/2,
    0,
    3*M_PI/2,
    M_PI
  };

  for (size_t i = 0; i < MAXPLAYERS; i++)
  {
    players[i].plynum = i;
    player_init(&players[i], colors[i], start_positions[i], start_rotations[i]);
  }

  syncPoint = 0;
}

bool player_has_control(player_data *player)
{
  return player->isAlive;
}

void player_fixedloop(player_data *player, float deltaTime, joypad_port_t port, bool is_human)
{
}

void player_loop(player_data *player, float deltaTime, joypad_port_t port, bool is_human)
{
  if (is_human)
  {
    joypad_inputs_t joypad = joypad_get_inputs(port);

    player->rotY += joypad.stick_x * -0.001f;
    T3DVec3 moveDir = {{fm_cosf(player->rotY) * (joypad.stick_y * 0.01f), 0.0f,
                        fm_sinf(player->rotY) * -(joypad.stick_y * 0.01f)}};
    t3d_vec3_add(&player->playerPos, &player->playerPos, &moveDir);
  }

  // ...and limit position inside the box
  const float BOX_SIZE = 140.0f;
  if (player->playerPos.v[0] < -BOX_SIZE)
    player->playerPos.v[0] = -BOX_SIZE;
  if (player->playerPos.v[0] > BOX_SIZE)
    player->playerPos.v[0] = BOX_SIZE;
  if (player->playerPos.v[2] < -BOX_SIZE)
    player->playerPos.v[2] = -BOX_SIZE;
  if (player->playerPos.v[2] > BOX_SIZE)
    player->playerPos.v[2] = BOX_SIZE;

  // Update player matrix
  t3d_mat4fp_from_srt_euler(player->modelMatFP,
                            (float[3]){0.125f, 0.125f, 0.125f},
                            (float[3]){0.0f, -player->rotY, 0},
                            player->playerPos.v);

  if (is_human)
  {
    joypad_buttons_t btn = joypad_get_buttons_pressed(port);

    if (btn.start)
      minigame_end();
  }

  if (syncPoint)
    rspq_syncpoint_wait(syncPoint); // wait for the RSP to process the previous frame
}

void player_draw(player_data *player)
{
  if (player->isAlive)
  {
    rspq_block_run(player->dplSnake);
  }
}

void minigame_fixedloop(float deltaTime)
{
  uint32_t playercount = core_get_playercount();
  for (size_t i = 0; i < MAXPLAYERS; i++)
  {
    player_fixedloop(&players[i], deltaTime, core_get_playercontroller(i), i < playercount);
  }
}

void minigame_loop(float deltaTime)
{
  uint8_t colorAmbient[4] = {0xAA, 0xAA, 0xAA, 0xFF};
  uint8_t colorDir[4] = {0xFF, 0xAA, 0xAA, 0xFF};

  uint32_t playercount = core_get_playercount();
  for (size_t i = 0; i < MAXPLAYERS; i++)
  {
    player_loop(&players[i], deltaTime, core_get_playercontroller(i), i < playercount);
  }

  // ======== Draw (3D) ======== //
  rdpq_attach(display_get(), depthBuffer);
  t3d_frame_start();

  t3d_screen_clear_color(RGBA32(224, 180, 96, 0xFF));
  t3d_screen_clear_depth();

  t3d_light_set_ambient(colorAmbient);
  t3d_light_set_directional(0, colorDir, &lightDirVec);
  t3d_light_set_count(1);

  for (int v = 0; v < MAXPLAYERS; v++)
  {
    T3DViewport *vp = &viewports[v];
    float distance = 50.0f; // Distance behind the player
    float height = 40.0f;   // Height above the player

    // Calculate the position of the camera behind the player based on their rotation
    camPos = (T3DVec3){{players[v].playerPos.v[0] - fm_cosf(players[v].rotY) * distance,
                        players[v].playerPos.v[1] + height,
                        players[v].playerPos.v[2] + fm_sinf(players[v].rotY) * distance}};

    // Calculate the target position slightly ahead of the player based on their rotation
    camTarget = (T3DVec3){{players[v].playerPos.v[0] + fm_sinf(players[v].rotY),
                           players[v].playerPos.v[1] + 9.0f, // Adjust height to center target around player's head/upper body
                           players[v].playerPos.v[2] + fm_cosf(players[v].rotY)}};

    // Like in all other examples, set up the projection (only really need to do it once) and view matrix here
    // after that apply the viewport and draw your scene
    // Since each of the viewport-structs has its own matrices, no conflicts will occur
    t3d_viewport_set_projection(vp, T3D_DEG_TO_RAD(60.0f), 2.0f, 200.0f);
    t3d_viewport_look_at(vp, &camPos, &camTarget, &(T3DVec3){{0, 1, 0}});
    t3d_viewport_attach(vp);

    // if you need directional light, re-apply it here after a new viewport has been attached
    // t3d_light_set_directional(0, colorDir, &lightDirVec);
    // t3d_matrix_push_pos(1);

    for (int p=0; p<MAXPLAYERS; p++)
    {
      // if(p == v)continue;
      // t3d_matrix_set(players[p].modelMatFP, true);
      rdpq_set_prim_color(players[p].color);
      // t3d_matrix_set(players[p].modelMatFP, true);
      player_draw(&players[p]);
    }

    rdpq_set_prim_color(RGBA32(0xFF, 0xFF, 0xFF, 0xFF));
    rspq_block_run(dplMap);

    t3d_matrix_pop(1);

    t3d_matrix_push_pos(1);

    syncPoint = rspq_syncpoint_new();
  }

  rdpq_sync_tile();
  rdpq_sync_pipe(); // Hardware crashes otherwise

  // draw thick lines between the screens
	rdpq_fill_rectangle(0, sizeY / 2 - 1, sizeX, sizeY / 2 + 1); // horizontal line
	rdpq_fill_rectangle(sizeX / 2 - 1, 0, sizeX / 2 + 1, sizeY); // vertical line

  rdpq_detach_show();
}

void player_cleanup(player_data *player)
{
  rspq_block_free(player->dplSnake);

  free_uncached(player->modelMatFP);
}

void minigame_cleanup(void)
{
  for (size_t i = 0; i < MAXPLAYERS; i++)
  {
    player_cleanup(&players[i]);
    t3d_model_free(modelPlayers[i]);
  }

  rspq_block_free(dplMap);

  
  t3d_model_free(modelMap);

  free_uncached(mapMatFP);

  t3d_destroy();

  display_close();
}
