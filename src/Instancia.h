#ifndef INSTANCIA_H
#define INSTANCIA_H
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <limits>
#include <math.h>  
using namespace std;
class Instancia
{
public:
    ifstream input;
    string linea;
    /*********** VARS ***********/
    int NoOfEntities;
    int NoOfRooms;
    int NoOfConstraints;
    int NoOfFloors;
    int NoOfHardConstraints;
    int NoOfSoftConstraints;
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


    /*********** CONSTRUCTOR ***********/
    Instancia(string FileToRead);


    /*********** METODOS ***********/
    void SetConstraintPenalties();
    void FreeData();

    /*********** DESTRUCTOR ***********/
    ~Instancia();
};
#endif