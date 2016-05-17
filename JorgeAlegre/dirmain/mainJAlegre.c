#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
#include <time.h>

u32 min(u32 a, u32 b) {
    if (a <= b) return a;
    else return b;
}

void DestruirOrden(u32** orden) {
    for (int i = 0; i < 10; i++) {
        if (orden[i] != NULL) {
            free(orden[i]);
            orden[i] = NULL;
        }
    }
    free(orden);
    orden = NULL;
}

void swap(u32* array, u32 left, u32 right) {
    u32 temp = array[left];
    array[left] = array[right];
    array[right] = temp;
}

int main() {
    NimheP grafo = NULL; // Ubicación de grafo.
    u32** orden = NULL;
    u32 nvertices = 0;
    int ncorrida_min_coloreo = 0;
    u32 ncromatico = 0; // Resultado de correr Greedy sobre el grafo.
    u32 min_ncromatico = 0;
    char* error_message = "Error en formato de entrada.\n"; // Imprimir por error de lectura.
    int bipartito = 0; // 1 si el grafo es bipartito.
    int nCG = 0, nGC = 0, nR = 0, nRAR = 0;
    int opcion = 0;


    // Creacion de nuevo grafo. Recibe entrada por STDIN.
    // Devuelve NULL si hubo error en formato de entrada.
    if ((grafo = NuevoNimhe()) == NULL ) {
        printf("%s", error_message);
        return 0;
    }

    // Primero vemos si es bipartito.
    if ((bipartito = Chidos(grafo)) == 1) {
        printf("Grafo Bipartito\n");

        // No tengo más nada que hacer.
        // Destruir grafo.
        return !DestruirNimhe(grafo);
    } else printf("Grafo No Bipartito\n");


    // Crear ordenes aleatorios.
    orden = calloc(10, sizeof(u32*));
    nvertices = NumeroDeVertices(grafo);
    min_ncromatico = nvertices + 1;
    //srand(time(NULL));

    for (int i = 0; i < 10; i++) {
        orden[i] = calloc(nvertices, sizeof(u32));
        for (u32 j = 0; j < nvertices; j++) {
            orden[i][j] = j;
        }
        for (u32 j = 0; j < nvertices; j++) {
            swap(orden[i], j, (rand() % nvertices));
        }
    }

    // Correr Greedy sobre grafo.
    // 10 veces con orden aleatorio.
    for (int i = 0; i < 10; i++) {
        // Ordenar vértices del grafo.
        OrdenEspecifico(grafo, orden[i]);

        // Obtenemos el coloreo devuelvo por Greedy.
        ncromatico = Greedy(grafo);

        // Imprimir resultado de Greedy.
        printf("coloreo aleatorio numero %d: %u colores\n", i + 1, ncromatico);

        // Si número cromático es 3, detener corrida.
        if (ncromatico == 3) {
            printf("X(G)=3\n");
            DestruirOrden(orden);
            return !DestruirNimhe(grafo);
        }

        min_ncromatico = min(min_ncromatico, ncromatico);
        if (min_ncromatico == ncromatico) ncorrida_min_coloreo = i;
    }


    // 1 vez con orden Welsh-Powell.
    // Ordenar vértices del grafo.
    printf("Empece WelshPowel\n");
    OrdenWelshPowell(grafo);
    printf("Termine WelshPowel\n");

    // Obtener número cromático.
    ncromatico = Greedy(grafo);

    // Imprimir resultado.
    printf(" coloreo con Greedy en WelshPowell:%u colores\n", ncromatico);

    // Si número cromático es 3, detener corrida.
    if (ncromatico == 3) {
        printf("X(G)=3\n");
        DestruirOrden(orden);
        return !DestruirNimhe(grafo);
    }

    min_ncromatico = min(min_ncromatico, ncromatico);

    // ncorrida_min_coloreo es el indice del arreglo orden donde está guardado
    // el orden que dió el menor coloreo de las primeras 10 iteraciones.
    if (min_ncromatico != ncromatico) {
        // Si Welsh Powell no dió mejor número cromatico, reordenar.
        OrdenEspecifico(grafo, orden[ncorrida_min_coloreo]);
        min_ncromatico = Greedy(grafo);

        //DEBUG
        printf("Numero de corrida elegida %d.\n", ncorrida_min_coloreo);
        printf("Numero cromatico que dió: %u.\n", min_ncromatico);
    }

    // Ya no necesito más el orden de las corridas. Liberar memoria.
    DestruirOrden(orden);

    // X(G) > 3. Comenzar Greedy iterado 1001 veces.
    printf("\n====Comenzando Greedy Iterado 1001 veces====\n\n");

    // Correr Greedy 1000.
    for (int i = 0; i < 1000; i++) {
        // Elegir orden y ordenar.
        opcion = (rand() % 16);

        if (opcion <= CHICOGRANDE) {
            ChicoGrande(grafo);
            nCG++;
        } else if (opcion <= REVIERTE) {
            Revierte(grafo);
            nR++;
        } else if (opcion <= GRANDECHICO) {
            GrandeChico(grafo);
            nGC++;
        } else {
           ReordenAleatorioRestringido(grafo);
           nRAR++;
        }

        // Correr Greedy sobre orden.
        ncromatico = Greedy(grafo);
        printf("%d. Greedy sobre el grafo: %u\n", i + 1, ncromatico);
        min_ncromatico = min(min_ncromatico, ncromatico);
    }

    // Correr Greedy 1 vez con orden Revierte.
    Revierte(grafo);
    ncromatico = Greedy(grafo);
    printf("1001. Greedy sobre el grafo: %u\n", ncromatico);
    min_ncromatico = min(min_ncromatico, ncromatico);
    nR++;


    // Imprimir resultados a usuario.
    printf("Mejor coloreo con Greedy iterado 1001 veces: %u colores\n", min_ncromatico);
    printf("(%d CG,%d GC,%d R,%d RAR)\n", nCG, nGC, nR, nRAR);

    // Destruir grafo.
    if (!DestruirNimhe(grafo)) printf("Todo mal.\n");
    return 0;
}
