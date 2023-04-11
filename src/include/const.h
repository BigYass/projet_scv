#ifndef CONSTANT
#define CONSTANT

/**
 * @brief Taille standard pour l'allocation sur le tas des buffers
 * 
 */
#define MAX_BUF_SIZE 0x100

#define LINUX //Pour compiler sur Linux
// #define WINDOWS //Pour compiler sur Windows (non fait)

/**
 * @brief Niveau de debug |
 * 0 : Aucun debug |
 * 1 : Erreur seulement | 
 * 2 : Erreur et Warnings |
 * 3 : Tout
 * 
 */
#define DEBUG 3

#define TMP_DIRECTORY ".tmp"

// Réinitialise les formats et la couleur du texte
#define RESET   "\033[0m"

//Mets le texte en noir
#define BLACK   "\033[30m"
//Mets le texte en rouge
#define RED     "\033[31m"
//Mets le texte en vert
#define GREEN   "\033[32m"
//Mets le texte en jaune
#define YELLOW  "\033[33m"
//Mets le texte en bleu
#define BLUE    "\033[34m"
//Mets le texte en magenta
#define MAGENTA "\033[35m"
//Mets le texte en cyan
#define CYAN    "\033[36m"
//Mets le texte en blanc
#define WHITE   "\033[37m"


//Mets le fond en noir
#define BGBLACK   "\033[40m"
//Mets le fond en rouge
#define BGRED     "\033[41m"
//Mets le fond en vert
#define BGGREEN   "\033[42m"
//Mets le fond en jaune
#define BGYELLOW  "\033[43m"
//Mets le fond en bleu
#define BGBLUE    "\033[44m"
//Mets le fond en magenta
#define BGMAGENTA "\033[45m"
//Mets le fond en cyan
#define BGCYAN    "\033[46m"
//Mets le fond en blanc
#define BGWHITE   "\033[47m"

// Mets le texte gras
#define BOLD      "\033[1m"
// Souligne le texte
#define UNDERLINE "\033[4m"
// Fait clignoter le texte
#define BLINK     "\033[5m"
// Inverse le texte
#define INVERT    "\033[7m"
// Cache le texte ?
#define HIDDEN    "\033[8m"

#endif