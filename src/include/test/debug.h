#ifndef DEBUG_LIB
#define DEBUG_LIB

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