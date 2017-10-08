#include "testing.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/

void pruebas_hash_catedra(void);
void pruebas_volumen_catedra(size_t);

int main(int argc, char *argv[])
{
    clock_t t_ini, t_fin;
    double secs;
  
    t_ini = clock();
    if (argc > 1) {
        // Asumimos que nos están pidiendo pruebas de volumen.
        long largo = strtol(argv[1], NULL, 10);
        pruebas_volumen_catedra((size_t) largo);
        return 0;
    }

    printf("~~~ PRUEBAS CÁTEDRA ~~~\n");
    pruebas_hash_catedra();
   
    
    t_fin = clock();
    secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
    printf("%.16g milisegundos\n", secs * 1000.0);
    
    return failure_count() > 0;
}
