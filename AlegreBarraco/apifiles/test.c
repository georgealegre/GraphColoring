#include <stdlib.h>
#include <stdio.h>
#include "Cthulhu.h"

int main() {
    NimheP grafo = NULL;
    grafo = NuevoNimhe();
    printf("Vertices: %u\n", NumeroDeVertices(grafo));
    printf("Lados %u\n", NumeroDeLados(grafo));
    if (DestruirNimhe(grafo)) printf("FIN.\n");
    else printf("Todo mal.\n");

    return 0;
}
