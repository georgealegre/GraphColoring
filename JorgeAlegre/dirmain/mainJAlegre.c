/* Matemática Discreta II
 * Proyecto, primera parte.
 * Autor: Alegre, Jorge Facundo <facu.alegre@gmail.com>
 */

#include <stdlib.h>
#include <stdio.h>
#include "Cthulhu.h"

#define NCORRIDAS_RND   10
#define NCORRIDAS       1000

u32 min(u32 a, u32 b) {
    if (a <= b) return a;
    else return b;
}

void swap(u32* array, u32 left, u32 right) {
    u32 temp = array[left];
    array[left] = array[right];
    array[right] = temp;
}

int main() {
    NimheP grafo = NULL; // Ubicación de grafo.
    u32* mejor_orden = NULL; // Arreglo con el orden que dió el mejor coloreo.
    u32* orden_actual = NULL; // Arreglo con el orden usado en el momento.
    u32 nvertices = 0; // Número de vértices en el grafo.
    u32 ncromatico = 0; // Resultado de correr Greedy sobre el grafo.
    // Menor número de colores usados hasta el momento para colorear.
    u32 min_ncromatico = 0;
    // Imprimir por error de lectura.
    char* error_message = "Error en formato de entrada.\n";
    // Para contar cuántas veces ordeno con cada orden.
    int nCG = 0, nGC = 0, nR = 0, nRAR = 0;
    int opcion = 0; // Para elegir orden a usar en iteración.


    // Creacion de nuevo grafo. Recibe entrada por STDIN.
    // Devuelve NULL si hubo error en formato de entrada.
    if ((grafo = NuevoNimhe()) == NULL ) {
        printf("%s", error_message);
        return 0;
    }
    
    nvertices = NumeroDeVertices(grafo);
    min_ncromatico = nvertices + 1;

    // Primero vemos si es bipartito.
    if (Chidos(grafo)) {
        printf("Grafo Bipartito\n");
        // No tengo más nada que hacer.
        // Destruir grafo.
        return !DestruirNimhe(grafo);
    } else printf("Grafo No Bipartito\n");

    // Crear ordenes aleatorios.
    srand((nvertices * NumeroDeLados(grafo)) + 
          NombreDelVertice(IesimoVerticeEnElOrden(grafo, 1)));
    mejor_orden = calloc(nvertices, sizeof(u32));
    orden_actual = calloc(nvertices, sizeof(u32));
    // Cargamos arreglo de orden con números del 0 a nvertices-1.
    for (u32 j = nvertices; j != 0; j--)
        orden_actual[j - 1] = j - 1;

    for (int i = 0; i < NCORRIDAS_RND; i++) {
        // Randomizar orden del grafo.
        for (u32 j = nvertices; j != 0; j--)
	    swap(orden_actual, j - 1, (rand() % nvertices));

        // Ordenar vértices del grafo y correr Greedy.
        OrdenEspecifico(grafo, orden_actual);
        ncromatico = Greedy(grafo);
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
            // Si esta corrida dió mejor coloreo hasta el momento,
            // guardar el orden en mejor_orden.
	    for (u32 i = nvertices; i != 0; i--)
		mejor_orden[i-1] = orden_actual[i-1];
    }

    // 1 vez con orden Welsh-Powell.
    // Ordenar vértices del grafo.
    OrdenWelshPowell(grafo);
    ncromatico = Greedy(grafo);
    // Imprimir resultado.
    printf("\n coloreo con Greedy en WelshPowell: %u colores\n", ncromatico);
    // Si número cromático es 3, detener corrida.
    if (ncromatico == 3) {
        printf("X(G)=3\n");
	if (orden_actual != NULL) {
            free(orden_actual);
	    orden_actual = NULL;
        }
	if (mejor_orden != NULL) {
	    free(mejor_orden);
	    mejor_orden = NULL;
        }
        return !DestruirNimhe(grafo);
    }
    min_ncromatico = min(min_ncromatico, ncromatico);
    if (min_ncromatico != ncromatico) {
        // Si Welsh Powell no dió mejor número cromatico, reordenar.
        OrdenEspecifico(grafo, mejor_orden);
        min_ncromatico = Greedy(grafo);
    }

    // Terminamos con órdenes aleatorios. Liberar memoria.
    free(orden_actual);
    orden_actual = NULL;
    free(mejor_orden);
    mejor_orden = NULL;

    // X(G) > 3. Comenzar Greedy iterado NCORRIDAS + 1 veces.
    printf("\n====Comenzando Greedy Iterado %d veces====\n\n", NCORRIDAS+1);
    // Correr Greedy NCORRIDAS veces.
    for (int i = 0; i < NCORRIDAS; i++) {
        // Elegir orden y ordenar.
        opcion = (rand() % 16);

        if (opcion <= 7) {
            ChicoGrande(grafo);
            nCG++;
        } else if (opcion <= 12) {
            Revierte(grafo);
            nR++;
        } else if (opcion <= 14) {
            GrandeChico(grafo);
            nGC++;
        } else {
           ReordenAleatorioRestringido(grafo);
           nRAR++;
        }

        // Correr Greedy sobre orden.
        ncromatico = Greedy(grafo);
        min_ncromatico = min(min_ncromatico, ncromatico);
    }
    // Correr Greedy 1 vez con orden Revierte.
    Revierte(grafo);
    nR++;
    ncromatico = Greedy(grafo);
    min_ncromatico = min(min_ncromatico, ncromatico);

    // Imprimir resultados a usuario.
    printf("Mejor coloreo con Greedy iterado %d veces: %u colores\n",
           NCORRIDAS + 1, min_ncromatico);
    printf("(%d CG,%d GC,%d R,%d RAR)\n", nCG, nGC, nR, nRAR);

    // Destruir grafo.
    return !DestruirNimhe(grafo);
}

