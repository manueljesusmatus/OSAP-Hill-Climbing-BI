#include "Solution.h"

Solution::Solution(string FileToRead)
{
    /* LECTURA DE ARCHIVO */
    this->input.open(FileToRead);
    if (!this->input.is_open())
    {
        cout << "Error al Abrir archivo" << endl;
        exit(10);
    }
    /* LECTURA DE LAS PRIMERAS 3 VARIABLES */
    this->input >> this->linea >> this->NoOfEntities;
    this->input >> this->linea >> this->NoOfRooms;
    this->input >> this->linea >> this->NoOfConstraints;

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
    /* Se inicia el arreglo q representa la solución */
    this->SetConstraintPenalties();
    this->solution = new int[this->NoOfEntities];
    for (int z = 0; z < this->NoOfEntities; z++)
    {
        this->solution[z] = -1;
    }
    this->CurrentroomCapacity = new int[this->NoOfRooms];
    this->Krooms = new int[this->NoOfRooms / 3];
    for (int k = 0; k < this->NoOfRooms; k++)
    {
        this->CurrentroomCapacity[k] = this->RSPACE[k];
    }
}

int Solution::SelectBestRoom(int NEIGHBORHOOD_SIZE, int ENTITY)
{
    int BestRoom = 0;
    int Hsuma = 7000;
    int suma = 0;
    for (int g = 0; g < NEIGHBORHOOD_SIZE; g++)
    {
        int room = this->Krooms[g];
        this->solution[ENTITY] = room;
        this->CurrentroomCapacity[this->solution[ENTITY]] -= this->ESPACE[ENTITY];
        suma = this->SC_penalizacion();
        if (Hsuma > suma)
        {
            Hsuma = suma;
            BestRoom = room;
        }
        else
        {
            this->CurrentroomCapacity[this->solution[ENTITY]] += this->ESPACE[ENTITY];
        }
    }
    return BestRoom;
}

int Solution::setNeighbors(int ENTITY, int K)
{
    random_shuffle(&this->RID[0], &this->RID[this->NoOfRooms - 1]);
    int CONT = 0;
    for (int h = 0; h < this->NoOfRooms; h++)
    {
        int room = this->RID[h];
        if ((this->RSPACE[room] * 1.5 >= this->ESPACE[ENTITY]) &&
            (this->RSPACE[room] * 0.5 <= this->ESPACE[ENTITY]))
        {
            this->Krooms[CONT] = room;
            CONT++;
            if (CONT >= K)
            {
                return CONT;
            }
        }
    }
    return CONT;
}

void Solution::CrearSolucionInicial()
{
    // Se setea K = N / 3
    int K = this->NoOfRooms / 3;

    random_shuffle(&this->EID[0], &this->EID[this->NoOfEntities - 1]);
    for (int c = 0; c < this->NoOfEntities; c++)
    {
        int ENTITY = this->EID[c];
        int NEIGHBORHOOD_SIZE = this->setNeighbors(ENTITY, K);
        int BestRoom = SelectBestRoom(NEIGHBORHOOD_SIZE, ENTITY);
        this->CurrentroomCapacity[BestRoom] -= ESPACE[ENTITY];
        this->solution[ENTITY] = BestRoom;
    }
}

void Solution::SetConstraintPenalties()
{
    this->TypeConstraints = new int[10];
    this->TypeConstraints[0] = 20;
    this->TypeConstraints[1] = 10;
    this->TypeConstraints[2] = 10;
    this->TypeConstraints[3] = 10;
    this->TypeConstraints[4] = 10;
    this->TypeConstraints[5] = 10;
    this->TypeConstraints[6] = 50;
    this->TypeConstraints[7] = 10;
    this->TypeConstraints[8] = 10;
    this->TypeConstraints[9] = 10;
}

/* Número random entre 0 y max */
int Solution::getRandomNumber(int max)
{
    return rand() % max;
}

/* Cambio de habitación entre las entidades A y B*/
void Solution::swap(int EntityA, int EntityB)
{
    int aux = this->solution[EntityA];
    this->solution[EntityA] = this->solution[EntityB];
    this->solution[EntityB] = aux;
    return;
}

/* Intercambia todas las entidades en RoomA a RoomB y viceversa */
void Solution::interchange(int RoomA, int RoomB)
{
    for (int k = 0; k < this->NoOfEntities; k++)
    {
        if (this->solution[k] == RoomA)
            this->solution[k] = RoomB;
        else if (this->solution[k] == RoomB)
            this->solution[k] = RoomA;
    }
    return;
}

/* Asigna la habitación room a la entidad Entity*/
void Solution::reallocate(int Entity, int room)
{
    this->solution[Entity] = room;
    return;
}

void Solution::ShowSolution()
{
    for (int i = 0; i < this->NoOfEntities; i++)
    {
        cout << this->solution[i] << " - ";
    }
    cout << endl;
}

/*************************  CONSTRAINTS *************************/
int Solution::C_ALLOCATION_CONSTRAINT(int Entity, int room)
{
    int roomEntity = this->solution[Entity];
    if (roomEntity != -1)
    {
        if (roomEntity == room)
        {
            return 0;
        }
        return 1;
    }
    return 0;
}

int Solution::C_NONALLOCATION_CONSTRAINT(int Entity, int room)
{
    int roomEntity = this->solution[Entity];
    if (roomEntity != -1)
    {
        if (roomEntity != room)
        {
            return 0;
        }
        return 1;
    }
    return 0;
}

int Solution::C_CAPACITY_CONSTRAINT(int Room)
{
    if (this->CurrentroomCapacity[Room] < 0)
    {
        return 1;
    }
    return 0;
}

int Solution::C_SAMEROOM_CONSTRAINT(int EntityA, int EntityB)
{
    if ((this->solution[EntityA] == -1) || (this->solution[EntityB] == -1))
        return 0;
    else if (this->solution[EntityA] == this->solution[EntityB])
        return 0;
    else
        return 1;
}

int Solution::C_NOTSAMEROOM_CONSTRAINT(int EntityA, int EntityB)
{
    if ((this->solution[EntityA] == -1) || (this->solution[EntityB] == -1))
        return 0;
    else if (this->solution[EntityA] != this->solution[EntityB])
        return 0;
    else
        return 1;
}

int Solution::C_NOTSHARING_CONSTRAINT(int Entity)
{
    if (this->solution[Entity] == -1)
        return 0;
    int room = this->solution[Entity];
    for (int y = 0; y < this->NoOfEntities; y++)
    {
        if ((this->solution[y] == room) && (y != Entity))
            return 1;
    }
    return 0;
}

int Solution::C_ADJACENCY_CONSTRAINT(int EntityA, int EntityB)
{
    if ((this->solution[EntityA] == -1) || (this->solution[EntityB] == -1))
        return 0;
    int roomA = this->solution[EntityA];
    int roomB = this->solution[EntityB];
    int size = this->ADJ_LIST_SIZE[roomA];
    for (int x = 0; x < size; x++)
    {
        if (this->ADJ_LIST[roomA][x] == roomB)
            return 0;
    }
    return 1;
}

int Solution::C_NEARBY_CONSTRAINT(int EntityA, int EntityB)
{
    if ((this->solution[EntityA] == -1) || (this->solution[EntityB] == -1))
        return 0;
    int roomA = this->solution[EntityA];
    int roomB = this->solution[EntityB];
    if (this->FID[roomA] == this->FID[roomB])
        return 0;
    else
        return 1;
}

int Solution::C_AWAYFROM_CONSTRAINT(int EntityA, int EntityB)
{
    if ((this->solution[EntityA] == -1) || (this->solution[EntityB] == -1))
        return 0;
    int roomA = this->solution[EntityA];
    int roomB = this->solution[EntityB];
    if (this->FID[roomA] != this->FID[roomB])
        return 0;
    else
        return 1;
}

int Solution::Constraints(int tipoderestrccion, int param1, int param2)
{
    int n = 0;
    switch (tipoderestrccion)
    {
    case 0:
        n = this->C_ALLOCATION_CONSTRAINT(param1, param2);
        break;
    case 1:
        n = this->C_NONALLOCATION_CONSTRAINT(param1, param2);
        break;
    case 2:
        cout << "no debería haber" << endl;
        break;
    case 3:
        n = this->C_CAPACITY_CONSTRAINT(param1);
        break;
    case 4:
        n = this->C_SAMEROOM_CONSTRAINT(param1, param2);
        break;
    case 5:
        n = this->C_NOTSAMEROOM_CONSTRAINT(param1, param2);
        break;
    case 6:
        n = this->C_NOTSHARING_CONSTRAINT(param1);
        break;
    case 7:
        n = this->C_ADJACENCY_CONSTRAINT(param1, param2);
        break;
    case 8:
        n = this->C_NEARBY_CONSTRAINT(param1, param2);
        break;
    case 9:
        n = this->C_AWAYFROM_CONSTRAINT(param1, param2);
        break;
    }
    return n;
}

int Solution::SC_penalizacion()
{
    int suma = 0;
    for (int h = 0; h < this->NoOfConstraints; h++)
    {
        /* y es '1' si la restricción h es violada, en otro caso 0 */
        int y = Constraints(this->CTYPE[h], this->C1[h], this->C2[h]);
        /* w es la penalización por violar la restricción h */
        int w = this->TypeConstraints[this->CTYPE[h]];
        suma += w * y;
    }
    return suma;
}

void Solution::FreeData()
{
    /* Free memory */
    this->input.close();
    for (int i = 0; i < this->NoOfRooms; i++)
    {
        free(this->ADJ_LIST[i]);
    }
    free(this->ADJ_LIST);
}

Solution::~Solution()
{
    delete[] this->FID;
    delete[] this->RID;
    delete[] this->EID;
    delete[] this->GID;
    delete[] this->ESPACE;
    delete[] this->RSPACE;
    delete[] this->CID;
    delete[] this->CTYPE;
    delete[] this->SorH;
    delete[] this->C1;
    delete[] this->C2;
    delete[] this->solution;
    delete[] this->CurrentroomCapacity;
    delete[] this->Krooms;
    delete[] this->ADJ_LIST_SIZE;
    delete[] this->TypeConstraints;
};