#include "Solution.h"

Solution::Solution(Instancia &i)
{
    this->solution = new int[i.NoOfEntities];
    this->CurrentroomCapacity = new double[i.NoOfRooms];
}

/* Se reescriben los valores de la solución actual */
void Solution::copySolution(Instancia &i, int EntityAux[], double RoomAux[])
{
    for (int ENTITY = 0; ENTITY < i.NoOfEntities; ENTITY++)
    {
        this->solution[ENTITY] = EntityAux[ENTITY];
    }
    /* Se inicializan las habitaciones con su capacidad máxima */
    for (int ROOM = 0; ROOM < i.NoOfRooms; ROOM++)
    {
        this->CurrentroomCapacity[ROOM] = RoomAux[ROOM];
    }
}

/* Función que desordena POR TRAMOS el orden en las entidades a asignarse según la precedencia (GRANDES,DURAS,RESTO) */
void Solution::setOrden(Instancia &inst)
{
    int h = inst.EntidadesGrandes;
    int cotai = 0;
    random_shuffle(&inst.entidades[cotai], &inst.entidades[cotai] + h);
    cotai += h;
    h = inst.EntidadesDuras;
    random_shuffle(&inst.entidades[cotai], &inst.entidades[cotai] + h - 1);
    cotai += h;
    h = inst.EntidadesAllocated;
    random_shuffle(&inst.entidades[cotai], &inst.entidades[cotai] + h - 1);
    cotai += h;
    h = inst.NoOfEntities;
    random_shuffle(&inst.entidades[cotai], &inst.entidades[cotai] + h - 1);
}

/* se busca la mejor habitación para una entidad */
int Solution::SelectBestRoom(Instancia &inst, int ENTITY)
{
    int BestRoom = -1;
    double Hsuma = numeric_limits<double>::max();
    double castigo, espacios;
    for (int room = 0; room < inst.NoOfRooms; room++)
    {
        // Se asigna habitación
        this->solution[ENTITY] = room;
        // se toma el espacio
        this->CurrentroomCapacity[room] -= inst.ESPACE[ENTITY];
        castigo = Penalty(inst);
        espacios = MalUso(inst);
        if (
            (Hsuma > castigo + espacios) &&
            (castigo != -1))
        {
            Hsuma = castigo + espacios;
            BestRoom = room;
        }
        // libera el espacio
        this->CurrentroomCapacity[room] += inst.ESPACE[ENTITY];
    }
    return BestRoom;
}

/* Función q inicializa la primera solución */
void Solution::CrearSolucionInicial(Instancia &inst)
{
    double calidad = -1;
    while (calidad == -1)
    {
        // se establece el orden en que las entidades seran seteadas según la heuristica escogida
        setOrden(inst);
        // Se inicializa la solución como un arreglo de entidades sin asignar
        for (int ENTITY = 0; ENTITY < inst.NoOfEntities; ENTITY++)
        {
            this->solution[ENTITY] = -1;
        }
        // Se inicializan las habitaciones con su capacidad máxima
        for (int ROOM = 0; ROOM < inst.NoOfRooms; ROOM++)
        {
            this->CurrentroomCapacity[ROOM] = inst.RSPACE[ROOM];
        }
        // Se asignan las entidades
        for (int c = 0; c < inst.lenEntidades; c++)
        {
            int ENTITY = inst.entidades[c];
            if (this->solution[ENTITY] == -1)
            {
                int BestRoom = this->SelectBestRoom(inst, ENTITY);
                if(BestRoom == -1)
                    break;
                this->CurrentroomCapacity[BestRoom] -= inst.ESPACE[ENTITY];
                this->solution[ENTITY] = BestRoom;
            }
        }
        calidad = Penalty(inst);
    }
}

void Solution::ShowSolution(Instancia &inst)
{
    for (int i = 0; i < inst.NoOfEntities; i++)
    {
        cout << this->solution[i] << "-";
        //cout << "ENTITY : " << i << "   ROOM : " << this->solution[i] << endl;
    }
    cout << endl;
}

void Solution::ShowSpace(Instancia &inst)
{
    for (int i = 0; i < inst.NoOfRooms; i++)
    {
        cout << this->CurrentroomCapacity[i] << " - ";
        //cout << "ENTITY : " << i << "   ROOM : " << this->solution[i] << endl;
    }
    cout << endl;
}

/************************* Revisar Restricciones *************************/
int Solution::C_ALLOCATION_CONSTRAINT(Instancia &inst, int Entity, int room)
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

int Solution::C_NONALLOCATION_CONSTRAINT(Instancia &inst, int Entity, int room)
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

int Solution::C_CAPACITY_CONSTRAINT(Instancia &inst, int Room)
{
    if (this->CurrentroomCapacity[Room] < 0)
    {
        return 1;
    }
    return 0;
}

int Solution::C_SAMEROOM_CONSTRAINT(Instancia &inst, int EntityA, int EntityB)
{
    if ((this->solution[EntityA] == -1) || (this->solution[EntityB] == -1))
        return 0;
    else if (this->solution[EntityA] == this->solution[EntityB])
        return 0;
    else
        return 1;
}

int Solution::C_NOTSAMEROOM_CONSTRAINT(Instancia &inst, int EntityA, int EntityB)
{
    if ((this->solution[EntityA] == -1) || (this->solution[EntityB] == -1))
        return 0;
    else if (this->solution[EntityA] != this->solution[EntityB])
        return 0;
    else
        return 1;
}

int Solution::C_NOTSHARING_CONSTRAINT(Instancia &inst, int Entity)
{
    if (this->solution[Entity] == -1)
        return 0;
    int room = this->solution[Entity];
    for (int y = 0; y < inst.NoOfEntities; y++)
    {
        if ((this->solution[y] == room) && (y != Entity))
            return 1;
    }
    return 0;
}

int Solution::C_ADJACENCY_CONSTRAINT(Instancia &inst, int EntityA, int EntityB)
{
    if ((this->solution[EntityA] == -1) || (this->solution[EntityB] == -1))
        return 0;
    int roomA = this->solution[EntityA];
    int roomB = this->solution[EntityB];
    int size = inst.ADJ_LIST_SIZE[roomA];
    for (int x = 0; x < size; x++)
    {
        if (inst.ADJ_LIST[roomA][x] == roomB)
            return 0;
    }
    return 1;
}

int Solution::C_NEARBY_CONSTRAINT(Instancia &inst, int EntityA, int EntityB)
{
    if ((this->solution[EntityA] == -1) || (this->solution[EntityB] == -1))
        return 0;
    int roomA = this->solution[EntityA];
    int roomB = this->solution[EntityB];
    if (inst.FID[roomA] == inst.FID[roomB])
        return 0;
    else
        return 1;
}

int Solution::C_AWAYFROM_CONSTRAINT(Instancia &inst, int EntityA, int EntityB)
{
    if ((this->solution[EntityA] == -1) || (this->solution[EntityB] == -1))
        return 0;
    int roomA = this->solution[EntityA];
    int roomB = this->solution[EntityB];
    if (inst.FID[roomA] != inst.FID[roomB])
        return 0;
    else
        return 1;
}

/******************** Calcular función de evaluación ********************************/

int Solution::Constraints(Instancia &inst, int tipoderestrccion, int param1, int param2)
{
    int n = 0;
    switch (tipoderestrccion)
    {
    case 0:
        n = this->C_ALLOCATION_CONSTRAINT(inst, param1, param2);
        break;
    case 1:
        n = this->C_NONALLOCATION_CONSTRAINT(inst, param1, param2);
        break;
    case 2:
        cout << "no debería haber" << endl;
        break;
    case 3:
        n = this->C_CAPACITY_CONSTRAINT(inst, param1);
        break;
    case 4:
        n = this->C_SAMEROOM_CONSTRAINT(inst, param1, param2);
        break;
    case 5:
        n = this->C_NOTSAMEROOM_CONSTRAINT(inst, param1, param2);
        break;
    case 6:
        n = this->C_NOTSHARING_CONSTRAINT(inst, param1);
        break;
    case 7:
        n = this->C_ADJACENCY_CONSTRAINT(inst, param1, param2);
        break;
    case 8:
        n = this->C_NEARBY_CONSTRAINT(inst, param1, param2);
        break;
    case 9:
        n = this->C_AWAYFROM_CONSTRAINT(inst, param1, param2);
        break;
    }
    return n;
}

/* Calcula el castigo por restricción blanda no cumplida, en caso de insatisfacer 1 restriccion dura retorna -1*/
double Solution::Penalty(Instancia &inst)
{
    int suma = 0;
    for (int h = 0; h < inst.NoOfConstraints; h++)
    {
        /* y es '1' si la restricción h es violada, en otro caso 0 */
        int y = Constraints(inst, inst.CTYPE[h], inst.C1[h], inst.C2[h]);
        /* La solución actual no respeta una restricción dura */
        if ((y > 0) && (inst.SorH[h] == 1))
        {
            return -1.0;
        }
        /* w es la penalización por violar la restricción h */
        int w = inst.TypeConstraints[inst.CTYPE[h]];
        suma += w * y;
    }
    return (double)suma;
}

/* Retorna el resultado dado por la función de evaluación */
double Solution::Calidad(Instancia &inst)
{
    return Penalty(inst) + MalUso(inst);
}

/* Calcula el espacio mal usado*/
double Solution::MalUso(Instancia &inst)
{
    double sumaDesperdiciado = 0;
    double sumaSobreutilizado = 0;
    double suma = 0, suma1 = 0, suma2 = 0;
    for (int ROOM = 0; ROOM < inst.NoOfRooms; ROOM++)
    {
        sumaDesperdiciado = this->CurrentroomCapacity[ROOM];
        sumaSobreutilizado = (-2 * this->CurrentroomCapacity[ROOM]);
        suma1 = max(0.0, sumaDesperdiciado);
        suma2 = max(0.0, sumaSobreutilizado);
        suma += (suma1 + suma2);
    }
    return suma;
}

Solution::~Solution()
{
    delete[] this->solution;
    delete[] this->CurrentroomCapacity;
};