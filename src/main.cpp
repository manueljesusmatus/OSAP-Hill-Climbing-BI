#include <iostream>
#include <string>
#include "AlgorithmHC.h"
using namespace std;

int main(int argc, char const *argv[])
{
    /* SEMILLA ALEATORIA PARA LOS RANDOMS QUE SE HAN DE EJECUTAR */
    srand(time(NULL));

    string filename = "nott1.txt";

    HillClimbing(filename);

    return 0;
}