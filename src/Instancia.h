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

    /********* DATA RELACIONADA A LA INSTANCIA *********/
    double mean;            // Espacio promedio de las entidades
    double sd;              // Desviación estandar
    int *allocated;         // Entidades relaciones a restricciones
    int *HardEntities;      // Entidades relaciones a restricciones duras
    int EntidadesDuras;     // largo de lista allocated
    int EntidadesAllocated; // largo de lista HardEntities
    int *entidades;         // lista de entidades ordenadas
    int lenEntidades;       // largo de lista entidades
    int EntidadesGrandes;   // cantidad de entidades grandes

    /*********** CONSTRUCTOR ***********/
    Instancia();
    Instancia(string FileToRead);

    /*********** METODOS ***********/
    void SetConstraintPenalties(); // setea las penalizaciones correspondientes a cada tipo
    void EntitiesHardConstraint(); // Obtiene todas las restricciones relacionadas a restricciones duras
    void setEntities();            // setea el arreglo entidades
    void FreeData();               // Libera memoria

    /*********** DESTRUCTOR ***********/
    ~Instancia();
};
#endif