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
    }

    /********************* ROOMS ****************************/
    /* LECTURA DE ETIQUETA "ROOMS" */
    input >> linea;

    /* ROOMS VARS */
    int RID[NoOfRooms];
    int FID[NoOfRooms];
    double RSPACE[NoOfRooms];
    int ADJ_LIST_SIZE;
    // revisar si se puede hacer sin mallo
    int** ADJ_LIST = (int**) malloc(NoOfRooms*sizeof(int*));

    /* LECTURA DE TABLA ROOMS */
    for (int i = 0; i < NoOfRooms; i++)
    {
        input >> RID[i] >> FID[i] >> RSPACE[i] >> ADJ_LIST_SIZE;
        ADJ_LIST[i] = (int*) malloc(ADJ_LIST_SIZE*sizeof(int));;
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

    /* Free memory */
    for(int i = 0; i < NoOfRooms; i++){
        free(ADJ_LIST[i]);
    }
    free(ADJ_LIST);
    input.close();
    return 0;
}