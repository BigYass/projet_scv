#ifndef DEBUG_LIB
#define DEBUG_LIB

#include "../const.h"

#ifdef LANG_FR_FR
#define E_MSG_MKDIR "Problème lors de la création de %s"
#define E_MSG_FILE_SUPPR "Erreur lors de la supression du ficher %s"
#define E_MSG_DIR_OPEN "Erreur lors de l'ouverture du dossier %s"
#define E_MSG_FILE_OPEN "Erreur lors de l'ouverture du ficher %s"
#define E_MSG_FILE_EXIST "Le fichier %s existe déjà"
#define E_MSG_STR_EXIST "%s est déjà existant, ajout impossible"

#define E_MSG_FUNC_NULL RED "%s(\"%s\") " RESET " a renvoyé NULL"
#define E_MSG_MALLOC_NULL "Erreur lors d'un malloc.. Pas assez de RAM ?"
#define E_MSG_PARAM_NULL "La fonction connait un paramètre null.."
#define E_MSG_ALLOC_NULL "Allocation null.."

#else 
#ifdef LANG_EN_US
#define E_MSG_MKDIR "Problem while creating %s"
#define E_MSG_FILE_SUPPR "Error while supressing the file %s"
#define E_MSG_DIR_OPEN "Error while opening the directory %s"
#define E_MSG_FILE_OPEN "Erroe while opening the file %s"
#define E_MSG_FILE_EXIST "The file %s already exist"
#define E_MSG_STR_EXIST "%s already exist, cannot add"

#define E_MSG_FUNC_NULL RED "%s(\"%s\") " RESET " returned NULL"
#define E_MSG_MALLOC_NULL "Error by a malloc.. Not enough RAM ?"
#define E_MSG_PARAM_NULL "The function have a null parameter.."
#define E_MSG_ALLOC_NULL "Allocation null.."
#endif
#endif
/**
 * @brief Affiche un message formatté avec le numéro de la ligne et le fichier
 * 
 */
#define err_logf(err_type, format, ...) __err_logf(__FILE__, __LINE__, __func__, (err_type), (format), __VA_ARGS__)

/**
 * @brief Affiche un message avec le numéro de la ligne et le fichier 
 * 
 */
#define err_log(err_type, s) __err_logf(__FILE__, __LINE__, __func__, (err_type), (s))

typedef enum error_type {
  E_OK, E_WARN, E_ERR
} error_type;

/**
 * @brief Permet de log une phrase en fonction de sa gravité
 * 
 * @param err_type Gravité de l'évenement
 * @param format Text
 * @param ... format
 */
void __err_logf(const char* __file, const int __line, const char* __function, error_type err_type, const char* format, ...);



#endif