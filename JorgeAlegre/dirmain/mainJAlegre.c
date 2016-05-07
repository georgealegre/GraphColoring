#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

int main() {
    NimheP grafo = NULL;
    grafo = NuevoNimhe();
    if (grafo != NULL) {
        printf("Vertices: %u\n", NumeroDeVertices(grafo));
        printf("Lados %u\n", NumeroDeLados(grafo));
        if (Chidos(grafo)) {
            printf("Grafo es bipartito\n");
        } else {
            printf("Grafo no es bipartito\n");
        }

        printf("Greedy sobre el grafo: %u\n", Greedy(grafo));

        if (DestruirNimhe(grafo)) printf("FIN.\n");
        else printf("Todo mal.\n");
    }

    return 0;
}
