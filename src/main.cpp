#include <iostream>
#include <string>
#include <time.h>
#include <stdio.h>
#include "AlgorithmHC.h"
using namespace std;

int main(int argc, char const *argv[])
{
    /* SEMILLA ALEATORIA PARA LOS RANDOMS QUE SE HAN DE EJECUTAR */
    clock_t t_ini, t_fin;
    double secs;
    t_ini = clock();
    srand(time(NULL));

    string filename = "nott1.txt";

    HillClimbing(filename);

    t_fin = clock();

    secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
    printf("%.2g minutos\n", secs / 60 );

    return 0;
}