#include "typedefs.h"

#define MAX_LINE_LENGTH 80

typedef struct _par_t* par_t;

/* Lee de stdin MAX_LINE_LENGTH caracteres.
 * Devuelve string leido, sin '\n' final.
 * Usuario debe liberar memoria para dicho string.
 * Si en una línea hay más de MAX_LINE_LENGTH caracteres, devuelve NULL.
 */
char* readline_from_stdin();

/* Util para leer línea de tipo "p edge n m", donde n y m son números.
 * Si línea respeta formato, devuelve n y m envueltos en struct par_t.
 * Caso contrario, devuelve NULL.
 * Usuario debe liberar memoria para par_t devuelto en caso de éxito.
 */
par_t handle_p_line(char* line);

/* Util para leer línea de tipo "e n m", donde n y m son números.
 * Si línea respeta formato, devuelve n y m envueltos en struct par_t.
 * Caso contrario, devuelve NULL.
 * Usuario debe liberar memoria para par_t devuelto en caso de éxito.
 */
par_t handle_e_line(char* line);

// Devuelve elelemento l del par.
u32 get_l(par_t par);

// Devuelve elelemento r del par.
u32 get_r(par_t par);
