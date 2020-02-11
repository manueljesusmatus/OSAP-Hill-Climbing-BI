#include "DataOSAP.h"



DataOSAP::DataOSAP(string FileToRead)
{
    input.open(FileToRead);
    if (!input.is_open())
    {
        cout << "Error al Abrir archivo" << endl;
        exit(10);
    }
    else
    {
        /* LECTURA DE LAS PRIMERAS 3 VARIABLES */
        input >> linea >> NoOfEntities;
        input >> linea >> NoOfRooms;
        input >> linea >> NoOfConstraints;

        /********************* ENTITIES ****************************/
        input >> linea;
        /* ENTITIES VARS */
        EID = new int[NoOfEntities];
        GID = new int[NoOfEntities];
        ESPACE = new double[NoOfEntities];
        /* LECTURA DE TABLA ENTITIES */
        for (int i = 0; i < NoOfEntities; i++)
        {
            input >> EID[i] >> GID[i] >> ESPACE[i];
        }

        /********************* ROOMS ****************************/
        input >> linea;
        /* ROOMS VARS */
        RID = new int[NoOfRooms];
        FID = new int[NoOfRooms];
        RSPACE = new double[NoOfRooms];
        ADJ_LIST = (int **)malloc(NoOfRooms * sizeof(int *));
        /* LECTURA DE TABLA ROOMS */
        for (int i = 0; i < NoOfRooms; i++)
        {
            input >> RID[i] >> FID[i] >> RSPACE[i] >> ADJ_LIST_SIZE;
            ADJ_LIST[i] = (int *)malloc(ADJ_LIST_SIZE * sizeof(int));
            for (int j = 0; j < ADJ_LIST_SIZE; j++)
            {
                input >> ADJ_LIST[i][j];
            }
        }

        /********************* CONSTRAINTS ****************************/
        input >> linea;
        /* ENTITIES VARS */
        CID = new int[NoOfConstraints];
        CTYPE = new int[NoOfConstraints];
        SorH = new int[NoOfConstraints];
        C1 = new int[NoOfConstraints];
        C2 = new int[NoOfConstraints];
        /* LECTURA DE TABLA ENTITIES */
        for (int i = 0; i < NoOfConstraints; i++)
        {
            input >> CID[i] >> CTYPE[i] >> SorH[i] >> C1[i] >> C2[i];
        }
    }
}

void DataOSAP::FreeData()
{
    /* Free memory */
    for (int i = 0; i < this->NoOfRooms; i++)
    {
        free(this->ADJ_LIST[i]);
    }
    free(this->ADJ_LIST);
    this->input.close();
    delete []FID;
    delete []RID;
    delete []EID;
    delete []GID;
    delete []ESPACE;
    delete []RSPACE;
    delete []CID;
    delete []CTYPE;
    delete []SorH;
    delete []C1;
    delete []C2;
}
