#ifndef DATAOSAP_H
#define DATAOSAP_H 
#include <iostream>
#include <fstream>
#include <string>


using namespace std;
class DataOSAP
{
public:
    /*********** VARS ***********/
    ifstream input;
    int NoOfEntities;
    int NoOfRooms;
    int NoOfConstraints;
    string linea;

    /*********** ENTITIES VARS ***********/
    int* EID;                   // ID de la entidad
    int* GID;                   // ID del grupo
    double* ESPACE;             // Espacio ocupado por la entidad

    /*********** ROOM VARS ***********/
    int** ADJ_LIST;             // Lista de adyacencia
    int* RID;                   // ID de la habitación
    int* FID;                   // ID del piso
    int ADJ_LIST_SIZE;          // Tamaño de la lista de adyacencia
    double* RSPACE;             // Capacidad de la habitación

    /*********** CONSTRAINT VARS ***********/
    int* CID;                   // ID de la restricción
    int* CTYPE;                 // Tipo de restricción
    int* SorH;                  // Tipo de restricción (Dura o blanda)
    int* C1;                    // param1
    int* C2;                    // param2

    /*********** SOLUTION VARS ***********/
    int* solution;
    int* AuxRoomsN;

    /*********** CONSTRUCTOR Y METODOS ***********/
    DataOSAP(string FileToRead);// constructor
    void initSolution();
    int getRandomNumber( int max );
    void swap(int EntityA, int EntityB);
    void interchange(int RoomA, int RoomB);
    void allocate(int Entity, int room);
    void initialSolution();
    void ShowSolution();
    void FreeData();           
};
#endif