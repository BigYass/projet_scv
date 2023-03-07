#ifndef CONSTANT
#define CONSTANT

/**
 * @brief Taille standard pour l'allocation sur le tas
 * 
 */
#define MAX_BUF_SIZE 0x100

#define LINUX //Pour compiler sur Linux
// #define WINDOWS //Pour compiler sur Windows

#define DEBUG //Beaucoup de Verbose

// Réinitialise les formats et la couleur du texte
#define RESET   "\033[0m"

// Couleur du texte
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

// Couleur de fond
#define BGBLACK   "\033[40m"
#define BGRED     "\033[41m"
#define BGGREEN   "\033[42m"
#define BGYELLOW  "\033[43m"
#define BGBLUE    "\033[44m"
#define BGMAGENTA "\033[45m"
#define BGCYAN    "\033[46m"
#define BGWHITE   "\033[47m"

// Attribut du texte
#define BOLD      "\033[1m"
#define UNDERLINE "\033[4m"
#define BLINK     "\033[5m"
#define INVERT    "\033[7m"
#define HIDDEN    "\033[8m"

#endif