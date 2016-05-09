#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

int main() {
    NimheP grafo = NULL;
    grafo = NuevoNimhe();
    if (grafo != NULL) {
        printf("Vertices: %u\n", NumeroDeVertices(grafo));
        printf("Lados %u\n", NumeroDeLados(grafo));

        OrdenWelshPowell(grafo);

        if (Chidos(grafo)) {
            printf("Grafo es bipartito\n");
        } else {
            printf("Grafo no es bipartito\n");
        }

        //for (int i = 0; i < 1000; i++) {
        printf("Greedy sobre el grafo: %u\n", Greedy(grafo));
        //}

        if (DestruirNimhe(grafo)) printf("FIN.\n");
        else printf("Todo mal.\n");
    }

    return 0;
}
