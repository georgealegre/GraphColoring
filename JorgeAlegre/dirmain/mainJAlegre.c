#include <stdlib.h>
#include <stdio.h>
#include "Cthulhu.h"

#include <time.h>

#define NCORRIDAS_RND 10
#define NCORRIDAS 1000

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
    u32* mejor_orden = NULL;
    u32* orden_actual = NULL;
    u32 nvertices = 0;
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
    //orden = calloc(NCORRIDAS_RND, sizeof(u32*));
    nvertices = NumeroDeVertices(grafo);
    min_ncromatico = nvertices + 1;
    //srand(time(NULL));

    // Correr Greedy sobre grafo.
    // NCORRIDAS_RND veces con orden aleatorio.
    
    // Crear primer orden.
    mejor_orden = calloc(nvertices, sizeof(u32));
    orden_actual = calloc(nvertices, sizeof(u32));
    for (u32 j = nvertices; j != 0; j--)
        orden_actual[j - 1] = j - 1;

    for (int i = 0; i < NCORRIDAS_RND; i++) {
        for (u32 j = nvertices; j != 0; j--)
	    swap(orden_actual, j, (rand() % nvertices));

        // Ordenar vértices del grafo.
        OrdenEspecifico(grafo, orden_actual);

        // Obtenemos el coloreo devuelvo por Greedy.
        ncromatico = Greedy(grafo);

        // Imprimir resultado de Greedy.
        printf("coloreo aleatorio numero %d: %u colores\n", i + 1, ncromatico);

        // Si número cromático es 3, detener corrida.
        if (ncromatico == 3) {
            printf("X(G)=3\n");
	    free(orden_actual);
	    orden_actual = NULL;
	    if (mejor_orden != NULL)
	        free(mejor_orden);
	    mejor_orden = NULL;
            return !DestruirNimhe(grafo);
        }

        min_ncromatico = min(min_ncromatico, ncromatico);
        if (min_ncromatico == ncromatico)
	    for (u32 i = nvertices; i != 0; i--)
		mejor_orden[i] = orden_actual[i];
    }
    free(orden_actual);
    orden_actual = NULL;

    // 1 vez con orden Welsh-Powell.
    // Ordenar vértices del grafo.
    OrdenWelshPowell(grafo);

    // Obtener número cromático.
    ncromatico = Greedy(grafo);

    // Imprimir resultado.
    printf("\n coloreo con Greedy en WelshPowell: %u colores\n", ncromatico);

    // Si número cromático es 3, detener corrida.
    if (ncromatico == 3) {
        printf("X(G)=3\n");
	free(orden_actual);
	orden_actual = NULL;
	if (mejor_orden != NULL)
	    free(mejor_orden);
	mejor_orden = NULL;
        return !DestruirNimhe(grafo);
    }

    min_ncromatico = min(min_ncromatico, ncromatico);

    // ncorrida_min_coloreo es el indice del arreglo orden donde está guardado
    // el orden que dió el menor coloreo de las primeras 10 iteraciones.
    if (min_ncromatico != ncromatico) {
        // Si Welsh Powell no dió mejor número cromatico, reordenar.
        OrdenEspecifico(grafo, mejor_orden);
        min_ncromatico = Greedy(grafo);
    }

    // Ya no necesito más el orden de las corridas. Liberar memoria.
    free(mejor_orden);
    mejor_orden = NULL;

    // X(G) > 3. Comenzar Greedy iterado NCORRIDAS + 1 veces.
    printf("\n====Comenzando Greedy Iterado %d veces====\n\n", NCORRIDAS+1);

    // Correr Greedy NCORRIDAS veces.
    for (int i = 0; i < NCORRIDAS; i++) {
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
        printf("%d. Greedy sobre el grafo: %u.\n", i+1, ncromatico);
        min_ncromatico = min(min_ncromatico, ncromatico);
    }

    // Correr Greedy 1 vez con orden Revierte.
    Revierte(grafo);
    ncromatico = Greedy(grafo);
    min_ncromatico = min(min_ncromatico, ncromatico);
    nR++;


    // Imprimir resultados a usuario.
    printf("Mejor coloreo con Greedy iterado %d veces: %u colores\n", NCORRIDAS+1, min_ncromatico);
    printf("(%d CG,%d GC,%d R,%d RAR)\n", nCG, nGC, nR, nRAR);

    // Destruir grafo.
    return !DestruirNimhe(grafo);
}

