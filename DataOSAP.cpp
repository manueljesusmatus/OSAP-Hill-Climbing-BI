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
        cout << "E" << endl;
        /* LECTURA DE LAS PRIMERAS 3 VARIABLES */
        input >> linea >> NoOfEntities;
        cout << NoOfEntities << endl;
        input >> linea >> NoOfRooms;
        cout << NoOfRooms << endl;
        input >> linea >> NoOfConstraints;
        cout << NoOfConstraints << endl;

        /********************* ENTITIES ****************************/
        input >> linea;
        /* ENTITIES VARS */
        EID = new int(NoOfEntities);
        GID = new int(NoOfEntities);
        ESPACE = new double(NoOfEntities);
        /* LECTURA DE TABLA ENTITIES */
        cout << "E" << endl;
        for (int i = 0; i < NoOfEntities; i++)
        {
            input >> EID[i] >> GID[i] >> ESPACE[i];
            cout << EID[i] << "-" << GID[i] << "-" << ESPACE[i] << endl;
        }
        cout << "E" << endl;


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
}
