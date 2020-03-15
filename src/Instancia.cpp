#include "Instancia.h"

Instancia::Instancia(string FileToRead)
{
    /* LECTURA DE ARCHIVO */
    this->input.open(FileToRead);
    if (!this->input.is_open())
    {
        cout << "Error al Abrir archivo" << endl;
        exit(10);
    }
    /* LECTURA DE LAS PRIMERAS 5 VARIABLES */
    this->input >> this->linea >> this->NoOfEntities;
    this->input >> this->linea >> this->NoOfRooms;
    this->input >> this->linea >> this->NoOfFloors;
    this->input >> this->linea >> this->NoOfConstraints;
    this->input >> this->linea >> this->NoOfHardConstraints;
    this->input >> this->linea >> this->NoOfSoftConstraints;

    /********************* ENTITIES ****************************/
    this->input >> this->linea;
    this->EID = new int[this->NoOfEntities];
    this->GID = new int[this->NoOfEntities];
    this->ESPACE = new double[this->NoOfEntities];
    for (int i = 0; i < this->NoOfEntities; i++)
    {
        this->input >> this->EID[i] >> this->GID[i] >> this->ESPACE[i];
    }

    /********************* ROOMS ****************************/
    this->input >> this->linea;
    this->RID = new int[this->NoOfRooms];
    this->FID = new int[this->NoOfRooms];
    this->RSPACE = new double[this->NoOfRooms];
    this->ADJ_LIST_SIZE = new int[this->NoOfRooms];
    this->ADJ_LIST = (int **)malloc(this->NoOfRooms * sizeof(int *));
    for (int i = 0; i < this->NoOfRooms; i++)
    {
        this->input >> this->RID[i] >> this->FID[i] >> this->RSPACE[i] >> this->ADJ_LIST_SIZE[i];
        this->ADJ_LIST[i] = (int *)malloc(this->ADJ_LIST_SIZE[i] * sizeof(int));
        for (int j = 0; j < this->ADJ_LIST_SIZE[i]; j++)
        {
            this->input >> this->ADJ_LIST[i][j];
        }
    }

    /********************* CONSTRAINTS ****************************/
    this->input >> this->linea;
    this->CID = new int[this->NoOfConstraints];
    this->CTYPE = new int[this->NoOfConstraints];
    this->SorH = new int[this->NoOfConstraints];
    this->C1 = new int[this->NoOfConstraints];
    this->C2 = new int[this->NoOfConstraints];
    for (int i = 0; i < this->NoOfConstraints; i++)
    {
        input >> this->CID[i] >> this->CTYPE[i] >> this->SorH[i] >> this->C1[i] >> this->C2[i];
    }
    /**********************************************************************/
    this->SetConstraintPenalties();

    double mean = 0;
    for (int i = 0; i < this->NoOfEntities; i++)
    {
        mean += this->ESPACE[i];
    }
    mean /= this->NoOfEntities;

    double sd = 0;
    for( int n = 0; n < this->NoOfEntities ; n++)
    {
        sd += ( this->ESPACE[n] - mean) * (this->ESPACE[n] - mean);
    }
    sd = sd / this->NoOfEntities;
    sd = sqrt(sd);

    for( int n = 0; n < this->NoOfEntities ; n++)
    {
        if(ESPACE[n] > (mean+2*sd))
            cout << n << endl;
    }
}