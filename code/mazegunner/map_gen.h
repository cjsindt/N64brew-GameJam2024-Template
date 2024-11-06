/* ------------------------ MACROS ------------------------- */
#define MAP_SIZE 10

/* ------------------- TYPE DEFINITIONS -------------------- */
typedef struct node_t
{
    uint8_t walls[4];
} node;

/* ----------------- FUNCTION DEFINITIONS ------------------ */

/**************************************************************
 * generateMap()
 *
 * Description: Generate a new map.
 *  
 *
 * @param map where to store the game map
 * @param pruneFactor how strongly to prune the map [0,99]
 * 
 *************************************************************/
void generateMap(node **map, uint8_t pruneFactor);

/**************************************************************
 * freeMap()
 *
 * Description: Free the memory used by the game map
 *  
 *
 * @param map the game map to free
 * 
 *************************************************************/
void freeMap(node** map);

/**************************************************************
 * drawMap()
 *
 * Description: Given the map, draw it to the console.
 *  
 *
 * @param map the game map
 * 
 *************************************************************/
void drawMap(node **map);
