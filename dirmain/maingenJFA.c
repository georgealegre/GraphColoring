/* Matemática Discreta II
 * Proyecto, segunda parte.
 * Autor: Alegre, Jorge Facundo <facu.alegre@gmail.com>
 */

#include <stdlib.h>
#include <stdio.h>
#include "Cthulhu.h"

struct IndividualSt {
    u32* orden;
    u32 size;
    u32 fitness;
}

struct PopulationSt {
    IndividualSt* individuals;
    u32 nindividuals;
    u32 const_fitness;
};

u32 min(u32 a, u32 b) {
    if (a <= b) return a;
    else return b;
}

void swap(u32* array, u32 left, u32 right) {
    u32 temp = array[left];
    array[left] = array[right];
    array[right] = temp;
}

u32* CrearOrdenAleatorio(u32 n) {
    u32* orden = calloc(n, sizeof(u32));    // Crear arreglo de 0 a n-1.

    for (u32 j = n; j != 0; j--)            // Inicializar.
        orden[j - 1] = j - 1;
    
    for (u32 j = n; j != 0; j--)            // Randomizar.
        swap(orden, j - 1, (rand() % n));

    return orden;
}

IndividualSt CrearIndividuo(u32 nvertices) {
    IndividualSt individuo;                 // Crear individuo.

    individuo.orden = CrearOrdenAleatorio(nvertices);
    individuo.size = nvertices;

    return individuo;
}

IndividualSt GenerarFitnessIndividuo(IndividualSt individuo, NimheP grafo, u32 const_fitness) {
    OrdenEspecifico(grafo, individuo.orden);
    individuo.fitness = individuo.size + 1 - Greedy(grafo);

    return individuo;
}

PopulationSt CrearPoblacion(u32 nvertices, u32 nindividuos, u32 const_fitness) {
    PopulationSt pop;

    pop.individuals = calloc(nindividuos, sizeof(struct IndividualSt);
    pop.nindividuals = nindividuos;
    pop.const_fitness = const_fitness;

    for (u32 i = 0; i < nindividuos; i++)
        pop.individuals[i] = CrearIndividuo(nvertices);

    return pop;
}

PopulationSt GenerarFitnessPoblacion(PopulationSt pop, NimheP grafo) {
    for (u32 i = 0; i < pop.nindividuals; i++)
        pop.individuals[i] = GenerarFitnessIndividuo(pop.individuals[i],
                                                     grafo,
                                                     pop.const_fitness);

    return pop;
}

int main(int argc, const char* argv[]) {
    NimheP grafo = NULL; // Ubicación de grafo.
    u32 nvertices = 0; // Número de vértices en el grafo.
    u32 nindividuos = 0; // Numero de individuos en la problacion.
    u32 ngeneraciones = 0; // Numero de generaciones para reproducir la poblacion.
    u32 const_fitness = 0; // Constante para multiplicar fitness.
    // Imprimir por error de lectura.
    char* error_message = "Error en formato de entrada.\n";
    char* error_parameter = "Parametros invalidos.\n";
    PopulationSt poblacion; // Estructura poblacion con individuos.

    // Verificar parametros.
    if (argc == 4) {
        nindividuos = strtoul(argv[1], NULL, 10);
        ngeneraciones = strtoul(argv[2], NULL, 10);
        const_fitness = strtoul(argv[3], NULL, 10);

        if (nindividuos < 2) {
            printf("Poblacion muy pequeña. Son necesarios, como minimo, 2 individuos.\n");
            return 0;
        } else if (ngeneraciones < 1) {
            printf("Numero de generaciones debe ser mayor a 0.\n");
            return 0;
        } else if (const_fitness < 1) {
            printf("Constante multiplicativa para fitness debe ser mayor a 0.\n");
            return 0;
        }
    } else {
        printf("%s", error_parameter);
        return 0;
    }

    // Creacion de nuevo grafo. Recibe entrada por STDIN.
    // Devuelve NULL si hubo error en formato de entrada.
    if ((grafo = NuevoNimhe()) == NULL ) {
        printf("%s", error_message);
        return 0;
    }
    
    // Generar semilla para numeros aleatorios y obtener datos esenciales.
    srand((nvertices * NumeroDeLados(grafo)) + 
          NombreDelVertice(IesimoVerticeEnElOrden(grafo, 1)));
    nvertices = NumeroDeVertices(grafo);

    // Primero vemos si es bipartito.
    if (Chidos(grafo)) {
        printf("Grafo Bipartito\n");
        // No tengo más nada que hacer.
        // Destruir grafo.
        return !DestruirNimhe(grafo);
    } else printf("Grafo No Bipartito\n");

    // Crear poblacion.
    poblacion = CrearPoblacion(nvertices, nindividuos, const_fitness);

    // Evolucionar poblacion ngeneraciones veces.
    for (u32 i = 0; i < ngeneraciones; i++) {
        poblacion = GenerarFitnessPoblacion(poblacion, grafo);

    }

    // Destruir grafo.
    return !DestruirNimhe(grafo);
}

