#ifndef _NIMHESORT_H
#define _NIMHESORT_H

#include "NimheP";

void OrdenNatural(NimheP G);
/* Ordena los vertices en orden creciente de sus “nombres” reales.
 * (recordar que el nombre real de un vértice es un u32)
 */

void OrdenWelshPowell(NimheP G);
/* Ordena los vertices de G de acuerdo con el orden Welsh-Powell, es decir, con
 * los grados en orden no creciente.
 */

void ReordenAleatorioRestringido(NimheP G);
/* Si G esta coloreado con r colores y W1 son los vertices coloreados con 1, W2
 * los coloreados con 2, etc, entonces esta función ordena los vertices poniendo
 * primero los vertices de Wj1 (en algún orden) luego los de Wj2
 * (en algún orden), etc, donde j1,j2,... son aleatorios (pero distintos).
 */

void GrandeChico(NimheP G);
/* Si G esta coloreado con r colores y W1 son los vertices coloreados con 1,
 * W2 los coloreados con 2, etc, entonces esta función ordena los vertices
 * poniendo primero los vertices de Wj1 (en algún orden) luego los de Wj2
 * (en algún orden), etc, donde j1, j2, ... son tales
 * que |Wj1 | ≥ |Wj2 | ≥ ... ≥ |Wjr | .
 */

void ChicoGrande(NimheP G);
/* Idem que el anterior excepto que ahora el orden es tal
 * que |Wj1 | ≤ |Wj2 | ≤ ... ≤ |Wjr | .
 */

void Revierte(NimheP G);
/* Si G esta coloreado con r colores y W1 son los vertices coloreados con 1,
 * W2 los coloreados con 2, etc, entonces esta función ordena los vertices
 * poniendo primero los vertices de Wr (en algún orden) luego los de Wr−1
 * (en algún orden), etc.
 */

void OrdenEspecifico(NimheP G,u32* x);
/* La imagen de x deben ser todos los numeros entre 0 y n − 1, sin repetir.
 * La función primero chequea que x cumpla esa condición. Si no lo hace, la
 * función deja el orden de G como estaba.
 * Si x cumple la condición, entonces la función ordena los vertices con
 * OrdenNatural(), luego lee el orden dado en la string x y los ordena de
 * acuerdo a lo que lee. Es decir, si luego de OrdenNatural los vertices
 * quedaron ordenados como, por ejemplo, V[0],V[1],...,V[n-1], luego de
 * OrdenEspecifico el orden debe ser V[x[0]],V[x[1]],...,V[x[n-1]].
 * Ejemplo:
 * Supongamos que los vertices son 1, 10, 22, 5, 64, 7, 1001. OrdenNatural()
 * los ordena como 1,5,7,10,22,64,1001. (i.e. V[0]=1,V[1]=5, etc).
 * Supongamos que x=4,0,2,6,5,3,1. Entonces OrdenEspecifico(G,x) ordenará los
 * vértices como V[4],V[0],V[2],etc, es decir, 22,1,7,1001,64,10,5.
 */

#endif
