#include <iostream>
#include <fstream>
#include <string>
#include "DataOSAP.h"
using namespace std;



int main(int argc, char const *argv[])
{
    /* SEMILLA ALEATORIA PARA LOS RANDOMS QUE SE HAN DE EJECUTAR */
    srand(time(NULL));

    string filename = "nott1.txt";
    DataOSAP d(filename);
    d.initSolution();
    d.initialSolution();
    d.ShowSolution();

    d.FreeData();
    return 0;
}