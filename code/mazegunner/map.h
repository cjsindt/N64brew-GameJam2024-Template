#ifndef __MAP_GEN_H__
#define __MAP_GEN_H__

/* ------------------------ MACROS ------------------------- */
#define MAP_SIZE 10

/* ------------------- TYPE DEFINITIONS -------------------- */
typedef struct MapNode
{
    uint8_t walls[4];
} MapNode_t;

/* ----------------- FUNCTION DEFINITIONS ------------------ */

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
void generate_map(MapNode_t **map, uint8_t pruneFactor);

/**************************************************************
 * free_map()
 *
 * Description: Free the memory used by the game map
 *  
 *
 * @param map the game map to free
 * 
 *************************************************************/
void free_map(MapNode_t** map);

/**************************************************************
 * draw_map()
 *
 * Description: Given the map, draw it to the console.
 *  
 *
 * @param map the game map
 * 
 *************************************************************/
void draw_map(MapNode_t **map);

/**************************************************************
 * gamemap_init()
 *
 * Description: Initialize game map
 *  
 *
 * @param map where to store the game map
 * 
 *************************************************************/
void map_init(MapNode_t **map);

/**************************************************************
 * total_walls()
 *
 * Description: Calculates the total number of walls in a map
 *  
 *
 * @param map the game map to count walls of
 * 
 *************************************************************/
int total_walls(MapNode_t **map);

#endif
