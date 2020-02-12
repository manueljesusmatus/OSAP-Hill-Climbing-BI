#include "DataOSAP.h"


DataOSAP::DataOSAP(string FileToRead)
{
    this->input.open(FileToRead);
    if (!this->input.is_open())
    {
        cout << "Error al Abrir archivo" << endl;
        exit(10);
    }
    else
    {
        /* LECTURA DE LAS PRIMERAS 3 VARIABLES */
        this->input >> this->linea >> this->NoOfEntities;
        this->input >> this->linea >> this->NoOfRooms;
        this->input >> this->linea >> this->NoOfConstraints;

        /********************* ENTITIES ****************************/
        this->input >> this->linea;
        /* ENTITIES VARS */
        this->EID = new int[this->NoOfEntities];
        this->GID = new int[this->NoOfEntities];
        this->ESPACE = new double[this->NoOfEntities];
        /* LECTURA DE TABLA ENTITIES */
        for (int i = 0; i < this->NoOfEntities; i++)
        {
            this->input >> this->EID[i] >> this->GID[i] >> this->ESPACE[i];
        }

        /********************* ROOMS ****************************/
        this->input >> this->linea;
        /* ROOMS VARS */
        this->RID = new int[this->NoOfRooms];
        this->FID = new int[this->NoOfRooms];
        this->RSPACE = new double[this->NoOfRooms];
        this->ADJ_LIST = (int **)malloc(this->NoOfRooms * sizeof(int *));
        /* LECTURA DE TABLA ROOMS */
        for (int i = 0; i < this->NoOfRooms; i++)
        {
            this->input >> this->RID[i] >> this->FID[i] >> this->RSPACE[i] >> this->ADJ_LIST_SIZE;
            this->ADJ_LIST[i] = (int *)malloc(this->ADJ_LIST_SIZE * sizeof(int));
            for (int j = 0; j < this->ADJ_LIST_SIZE; j++)
            {
                this->input >> this->ADJ_LIST[i][j];
            }
        }

        /********************* CONSTRAINTS ****************************/
        this->input >> this->linea;
        /* ENTITIES VARS */
        this->CID = new int[this->NoOfConstraints];
        this->CTYPE = new int[this->NoOfConstraints];
        this->SorH = new int[this->NoOfConstraints];
        this->C1 = new int[this->NoOfConstraints];
        this->C2 = new int[this->NoOfConstraints];
        /* LECTURA DE TABLA ENTITIES */
        for (int i = 0; i < this->NoOfConstraints; i++)
        {
            input >> this->CID[i] >> this->CTYPE[i] >> this->SorH[i] >> this->C1[i] >> this->C2[i];
        }
    }
}


void DataOSAP::initSolution(){
    /* Se inicia el arreglo q representa la solución */
    solution = new int[this->NoOfEntities];
}


/* Número random entre 0 y max */
int DataOSAP::getRandomNumber( int max ){
    return rand() %max;
}


/* Cambio de habitación entre las entidades A y B*/
void DataOSAP::swap(int EntityA, int EntityB){
    int aux = this->solution[EntityA];
    this->solution[EntityA] = this->solution[EntityB];
    this->solution[EntityB] = aux;
    return;
}


/* Intercambia todas las entidades en RoomA a RoomB y viceversa */
void DataOSAP::interchange(int RoomA, int RoomB){
    for(int k = 0; k < this->NoOfEntities; k++){
        if( this->solution[k] == RoomA)
            this->solution[k] = RoomB;
        else if( this->solution[k] == RoomB)
            this->solution[k] = RoomA;
    }
    return;
}


/* Asigna la habitación room a la entidad Entity*/
void DataOSAP::allocate(int Entity, int room){
    this->solution[Entity] = room;
    return;
}


void DataOSAP::initialSolution(){
    int k = this->NoOfRooms/3;
    this->AuxRoomsN = new int[ k ];

    for( int ENTITY = 0; ENTITY < this->NoOfEntities; ENTITY++){
        // Cada entidad es enviada a una pieza totalmente random (hasta el moment)
        this->solution[ENTITY] = getRandomNumber( k );
    }
}


void DataOSAP::ShowSolution(){
    for(int i = 0; i < this->NoOfEntities; i++ ){
        cout << this->solution[i] << " - ";
    }
    cout << endl;
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
    delete []this->FID;
    delete []this->RID;
    delete []this->EID;
    delete []this->GID;
    delete []this->ESPACE;
    delete []this->RSPACE;
    delete []this->CID;
    delete []this->CTYPE;
    delete []this->SorH;
    delete []this->C1;
    delete []this->C2;
    delete []this->solution;
    delete []this->AuxRoomsN;
}
