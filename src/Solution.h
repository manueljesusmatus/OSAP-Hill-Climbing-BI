#ifndef SOLUTION_H
#define SOLUTION_H
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <limits>

using namespace std;
class Solution
{
public:
    ifstream input;
    string linea;
    /*********** VARS ***********/
    int NoOfEntities;
    int NoOfRooms;
    int NoOfConstraints;
    int *TypeConstraints;

    /*********** ENTITIES VARS ***********/
    int *EID;       // ID de la entidad
    int *GID;       // ID del grupo
    double *ESPACE; // Espacio ocupado por la entidad

    /*********** ROOM VARS ***********/
    int **ADJ_LIST;     // Lista de adyacencia
    int *RID;           // ID de la habitación
    int *FID;           // ID del piso
    int *ADJ_LIST_SIZE; // Tamaño de la lista de adyacencia
    double *RSPACE;     // Capacidad de la habitación

    /*********** CONSTRAINT VARS ***********/
    int *CID;   // ID de la restricción
    int *CTYPE; // Tipo de restricción
    int *SorH;  // Tipo de restricción (Dura o blanda)
    int *C1;    // param1
    int *C2;    // param2

    /*********** SOLUTION VARS ***********/
    int *solution;
    int *AuxArray;
    double *CurrentroomCapacity;
    int *Krooms;

    /*********** CONSTRUCTOR Y METODOS ***********/
    Solution(string FileToRead);

    /* SOLUCION INICIAL*/
    void CrearSolucionInicial();
    int SelectBestRoom( int NEIGHBORHOOD_SIZE, int ENTITY  );
    int setKRooms( int ENTITY, int K );
    int XD( int tipo );

    /**/
    int Penalty();
    double MalUso();
    double MAL();
    double wea( int room);
    int Constraints(int tipoderestrccion, int param1, int param2);

    /* METODOS */
    void SetConstraintPenalties();
    void swap(int EntityA, int EntityB);
    void interchange(int RoomA, int RoomB);
    double reallocate(int ENTITY, double calidad);
    void ShowSolution();

    /* CONSTRAINTS */
    /* 
     * En general retornan 1 si la 
     * restricción NO se cumple, 
     * en otro caso 0
     * 
     * */
    int C_ALLOCATION_CONSTRAINT(int Entity, int room);
    int C_NONALLOCATION_CONSTRAINT(int Entity, int room);
    int C_CAPACITY_CONSTRAINT(int Room);
    int C_SAMEROOM_CONSTRAINT(int EntityA, int EntityB);
    int C_NOTSAMEROOM_CONSTRAINT(int EntityA, int EntityB);
    int C_NOTSHARING_CONSTRAINT(int Entity);
    int C_ADJACENCY_CONSTRAINT(int EntityA, int EntityB);
    int C_NEARBY_CONSTRAINT(int EntityA, int EntityB);
    int C_AWAYFROM_CONSTRAINT(int EntityA, int EntityB);
    void FreeData();
    ~Solution();
};
#endif