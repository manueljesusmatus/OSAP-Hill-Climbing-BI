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
    this->CurrentroomCapacity = new double[this->NoOfRooms];
    this->EntidadesDuras = this->EntitiesHardConstraint();

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

/* se busca la mejor habitación para una entidad */
int Solution::SelectBestRoom(int NEIGHBORHOOD_SIZE, int ENTITY)
{
    int BestRoom = 0;
    double Hsuma = numeric_limits<double>::max();
    double suma = 0;
    double castigo;
    random_shuffle(&this->RID[0], &this->RID[this->NoOfRooms - 1]);
    for (int g = 0; g < NEIGHBORHOOD_SIZE; g++)
    {
        int room = this->RID[g];

        // Se asigna habitación
        this->solution[ENTITY] = room;
        // se toma el espacio
        this->CurrentroomCapacity[room] -= this->ESPACE[ENTITY];
        castigo = (double)this->Penalty();
        suma = this->MalUso();
        if ((Hsuma > (suma + castigo)) && (castigo != -1))
        {
            Hsuma = suma + castigo;
            BestRoom = room;
        }
        // libera el espacio
        this->CurrentroomCapacity[room] += this->ESPACE[ENTITY];
    }
    return BestRoom;
}

/* Función q obtiene las entidades asociadas a restricciones duras */
int Solution::EntitiesHardConstraint()
{
    this->AuxArray = new int[this->NoOfEntities];
    int n = 0;
    for (int Constraint = 0; Constraint < this->NoOfConstraints; Constraint++)
    {
        if (this->SorH[Constraint] == 1)
        {
            switch (this->CTYPE[Constraint])
            {
            case 0:
                this->AuxArray[n++] = this->C1[Constraint];
                break;
            case 1:
                this->AuxArray[n++] = this->C1[Constraint];
                break;
            case 4:
                this->AuxArray[n++] = this->C1[Constraint];
                this->AuxArray[n++] = this->C2[Constraint];
                break;
            case 5:
                this->AuxArray[n++] = this->C1[Constraint];
                this->AuxArray[n++] = this->C2[Constraint];
                break;
            case 6:
                this->AuxArray[n++] = this->C1[Constraint];
                break;
            case 7:
                this->AuxArray[n++] = this->C1[Constraint];
                this->AuxArray[n++] = this->C2[Constraint];
                break;
            case 8:
                this->AuxArray[n++] = this->C1[Constraint];
                this->AuxArray[n++] = this->C2[Constraint];
                break;
            case 9:
                this->AuxArray[n++] = this->C1[Constraint];
                this->AuxArray[n++] = this->C2[Constraint];
                break;
            default:
                break;
            }
        }
    }
    return n;
}

/* Función q inicializa la primera solución */
void Solution::CrearSolucionInicial()
{
    // Se inicializa el arrgelo solución con -1 (Entidad sin habitación)
    for (int z = 0; z < this->NoOfEntities; z++)
        this->solution[z] = -1;
    // Se inicializa la capacidad de las habitaciones al máximo
    for (int k = 0; k < this->NoOfRooms; k++)
        this->CurrentroomCapacity[k] = this->RSPACE[k];
    // Se inicializa K
    int K = this->NoOfRooms; //3;
    // Heuristica, primero se asignan las habitaciones involucradas en retricciones duras
    for (int c = 0; c < this->EntidadesDuras; c++)
    {
        int ENTITY = this->AuxArray[c];
        if (this->solution[ENTITY] == -1)
        {
            int BestRoom = SelectBestRoom(K, ENTITY);
            this->CurrentroomCapacity[BestRoom] -= ESPACE[ENTITY];
            this->solution[ENTITY] = BestRoom;
        }
    }
    // Se asignan el resto de habitaciones aleatoriamente
    random_shuffle(&this->EID[0], &this->EID[this->NoOfEntities - 1]);
    for (int c = 0; c < this->NoOfEntities; c++)
    {
        int ENTITY = this->EID[c];
        if (this->solution[ENTITY] == -1)
        {
            int BestRoom = SelectBestRoom(K, ENTITY);
            this->CurrentroomCapacity[BestRoom] -= ESPACE[ENTITY];
            this->solution[ENTITY] = BestRoom;
        }
    }
}

/* Penalización de las restricciones */
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

void Solution::ShowSolution()
{
    for (int i = 0; i < this->NoOfEntities; i++)
    {
        cout <<"ENTITY : " << i << "   ROOM : " << this->solution[i] << endl;
    }
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
        n = 2 * this->C_SAMEROOM_CONSTRAINT(param1, param2);
        break;
    case 5:
        n = 2 * this->C_NOTSAMEROOM_CONSTRAINT(param1, param2);
        break;
    case 6:
        n = this->C_NOTSHARING_CONSTRAINT(param1);
        break;
    case 7:
        n = 2 * this->C_ADJACENCY_CONSTRAINT(param1, param2);
        break;
    case 8:
        n = 2 * this->C_NEARBY_CONSTRAINT(param1, param2);
        break;
    case 9:
        n = 2 * this->C_AWAYFROM_CONSTRAINT(param1, param2);
        break;
    }
    return n;
}

int Solution::Penalty()
{
    int suma = 0;
    for (int h = 0; h < this->NoOfConstraints; h++)
    {
        /* y es '1' si la restricción h es violada, en otro caso 0 */
        int y = Constraints(this->CTYPE[h], this->C1[h], this->C2[h]);
        /* La solución actual no respeta una restricción dura*/
        if ((y > 0) && (this->SorH[h] == 1))
        {
            return -1;
        }
        /* w es la penalización por violar la restricción h */
        int w = this->TypeConstraints[this->CTYPE[h]];
        suma += w * y;
    }
    return suma;
}

double Solution::MalUso()
{
    double sumaDesperdiciado = 0;
    double sumaSobreutilizado = 0;
    double suma = 0;
    for (int ROOM = 0; ROOM < this->NoOfRooms; ROOM++)
    {
        sumaDesperdiciado = this->CurrentroomCapacity[ROOM];
        sumaSobreutilizado = (-2 * this->CurrentroomCapacity[ROOM]);
        suma += max(sumaSobreutilizado, sumaDesperdiciado);
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
    delete[] this->AuxArray;
};