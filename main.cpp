#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <list>
using namespace std;

int main(int argc, char const *argv[])
{
    string filename = "nott1.txt";
    ifstream input;

    /* VARS */
    int NoOfEntities, NoOfRooms, NoOfConstraints;
    string linea;

    input.open(filename);
    if (!input.is_open())
    {
        cout << "Error al Abrir archivo" << endl;
        return 1;
    }
    /* LECTURA DE LAS PRIMERAS 3 VARIABLES */
    input >> linea >> NoOfEntities;
    input >> linea >> NoOfRooms;
    input >> linea >> NoOfConstraints;

    /********************* ENTITIES ****************************/
    /* LECTURA DE ETIQUETA "ENTITIES"*/
    input >> linea;

    /* ENTITIES VARS */
    int EID[NoOfEntities];
    int GID[NoOfEntities];
    double ESPACE[NoOfEntities];

    /* LECTURA DE TABLA ENTITIES */
    for (int i = 0; i < NoOfEntities; i++)
    {
        input >> EID[i] >> GID[i] >> ESPACE[i];
        // cout << EID[i] << "--" << GID[i] << "--" << ESPACE[i] << endl;
    }

    /********************* ROOMS ****************************/
    /* LECTURA DE ETIQUETA "ROOMS" */
    input >> linea;

    /* ROOMS VARS */
    int RID[NoOfRooms];
    int FID[NoOfRooms];
    double RSPACE[NoOfRooms];
    int ADJ_LIST_SIZE;
    
    // SOLUCI'ON PARCHE, Basado en q no puede haber una lista más larga (probablemente haya q usar listas dinamicas)
    int* ADJ_LIST [NoOfRooms];
    //int ADJ_LIST [NoOfRooms][NoOfRooms];

    /* LECTURA DE TABLA ROOMS */
    for (int i = 0; i < NoOfRooms; i++)
    {
        input >> RID[i] >> FID[i] >> RSPACE[i] >> ADJ_LIST_SIZE;
        ADJ_LIST[i] = new int(ADJ_LIST_SIZE);
        cout << RID[i] << endl;
        for(int j = 0; j < ADJ_LIST_SIZE; j++){
            input >> ADJ_LIST[i][j];
        }
    }


    /********************* CONSTRAINTS ****************************/
    /* LECTURA DE ETIQUETA "ENTITIES"*/
    input >> linea;

    /* ENTITIES VARS */
    int CID[NoOfConstraints];
    int CTYPE[NoOfConstraints];
    int SorH[NoOfConstraints];
    int C1[NoOfConstraints];
    int C2[NoOfConstraints];

    /* LECTURA DE TABLA ENTITIES */
    for (int i = 0; i < NoOfConstraints; i++)
    {
        input >> CID[i] >> CTYPE[i] >> SorH[i] >> C1[i] >> C2[i];
    }


    input.close();
    return 0;
}