#ifndef SOLUTION_H
#define SOLUTION_H
#include <iostream>
#include <string>
#include <stdio.h>
#include <limits>
#include "Instancia.h"
using namespace std;
class Solution
{
public:
    /*********** SOLUTION VARS ***********/
    int *solution;               // arreglo que representa la solución
    double *CurrentroomCapacity; // arreglo que representa el espacio disponible por pieza

    /*********** CONSTRUCTOR ***********/
    Solution() = default;
    Solution(Instancia &instancia);

    /**************** SOLUCION INICIAL ********************/
    void CrearSolucionInicial(Instancia &inst);
    int SelectBestRoom(Instancia &inst, int ENTITY);

    /*********************** METODOS ***********************/
    void copySolution(Instancia &i, int EntityAux[], double RoomAux[]);
    void ShowSolution(Instancia &inst);
    void ShowSpace(Instancia &inst);
    void setOrden(Instancia &inst);

    /************** FUNCIóN DE EVALUACIóN ******************/
    double Penalty(Instancia &inst);
    double MalUso(Instancia &inst);
    double Calidad(Instancia &inst);
    int Constraints(Instancia &inst, int tipoderestrccion, int param1, int param2);

    /******************** CONSTRAINTS **********************/
    /* 
     * En general retornan 1 si la 
     * restricción NO se cumple, 
     * en otro caso 0
     * */
    int C_ALLOCATION_CONSTRAINT(Instancia &inst, int Entity, int room);
    int C_NONALLOCATION_CONSTRAINT(Instancia &inst, int Entity, int room);
    int C_CAPACITY_CONSTRAINT(Instancia &inst, int Room);
    int C_SAMEROOM_CONSTRAINT(Instancia &inst, int EntityA, int EntityB);
    int C_NOTSAMEROOM_CONSTRAINT(Instancia &inst, int EntityA, int EntityB);
    int C_NOTSHARING_CONSTRAINT(Instancia &inst, int Entity);
    int C_ADJACENCY_CONSTRAINT(Instancia &inst, int EntityA, int EntityB);
    int C_NEARBY_CONSTRAINT(Instancia &inst, int EntityA, int EntityB);
    int C_AWAYFROM_CONSTRAINT(Instancia &inst, int EntityA, int EntityB);

    /*********** DESTRUCTOR ***********/
    ~Solution();
};
#endif