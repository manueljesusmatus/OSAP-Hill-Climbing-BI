#ifndef SALIDA_H
#define SALIDA_H
#include <iostream>     // std::cout, std::endl
#include <fstream>
#include <iomanip>
#include "Instancia.h"
#include "Solution.h"
using namespace std;

class Salida{
public:
    ofstream myfile;


    Salida() = default;
    Salida(Instancia &instancia, Solution &solution, string Filename);

    void Restricciones_blandas(Instancia &instancia, Solution &solution);
    void Espacios(Instancia &instancia, Solution &solution);
    void Habitacion(Instancia &instancia, Solution &solution);

};

#endif