#include <stdio.h>

#include "VerticeSt.h"
#include "NimheP.h"

struct VerticeSt{
    u32 nombre;
    u32 grado;
    u32 color;
    u32* vecinos;
};

/* Devuelve el color con el que está coloreado el vértice x.
* Si el vértice no esta coloreado, devuelve 0.
*/
u32 ColorDelVertice(VerticeSt x) {
    return (x.color);
}

/* Devuelve el grado del vértice x. */
u32 GradoDelVertice(VerticeSt x) {
    return (x.grado);
}

/* Devuelve el nombre real del vértice x. */
u32 NombreDelVertice(VerticeSt x) {
    return (x.nombre);
}

/* Imprime en standard output una lista de los vecinos de x.
* La lista debe estar formada por los nombres reales de los vertices,
* separados por comas, con un punto al final de la lista.
* Ejemplo:
* VecinosDelVertice(x,G);
* debe producir:
* 1,7,10,333,819092.
* (asumiendo que esos son los vecinos de x). No es necesario que la lista esté
* ordenada en ningún orden particular.
*/
void ImprimirVecinosDelVertice(VerticeSt x, NimheP G) {
    u32 i = 0; // Iterador de vecinos.
    struct VerticeSt ivecino; // Iésimo vecino de x.

    if (G != NULL) {
        printf("\n");

        // x.grado es la cantidad de vecinos que x tiene.
        // Imprimir con comma todos menos el último vecino.
        while (i < x.grado - 1) {
            vecino = IesimoVecino(G, x, i);
            printf("%zu,", vecino.nombre);
            i++;
        }

        // Imprimir con punto al último vecino.
        vecino = IesimoVecino(G, x, i);
        printf("%zu.", vecino.nombre);
    }
}
