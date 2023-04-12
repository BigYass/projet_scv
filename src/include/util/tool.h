#ifndef TOOL
#define TOOL

#include "../const.h"

#define TO_BOOL(NUM) ((NUM) ? RED"false"RESET : GREEN"true"RESET)

#define sizeof_a(array) (sizeof((array)) / sizeof((array[0])))

/**
 * @brief Initialise les ressources nésséssaire pour tool.h (notamment mettre la graine du générateur de nombre)
 * 
 */
void init_tool();

/**
 * @brief Génère une chaine charactère aléatoire
 * 
 * @param len Longueur de la chaine charactère 
 * @return char* La chaine de charactère générée
 */
char *random_str(unsigned long len);

#endif