#include "Solution.h"
#include "AlgorithmHC.h"
#include "Instancia.h"
#include "Salida.h"
#include <string>
using namespace std;

double INTERCHANGE(Solution &osapSol, Solution &nueva, Solution &aux, Instancia &inst)
{
    double calidad = osapSol.Calidad(inst);
    double castigo, currentcalidad;
    int cambio = -1;

    // Las habitaciones son tomadas de manera aleatoria
    random_shuffle(&inst.RID[0], &inst.RID[0] + inst.NoOfRooms - 1);
    for (int ROOM = 0; ROOM < (inst.NoOfRooms - 1) / 3; ROOM++)
    {
        int room1 = inst.RID[ROOM];
        int room2 = inst.RID[ROOM + 1];
        aux.copySolution(inst, osapSol.solution, osapSol.CurrentroomCapacity);
        // INTERCHANGE
        for (int ENTITY = 0; ENTITY < inst.NoOfEntities; ENTITY++)
        {
            if (aux.solution[ENTITY] == room1)
            {
                aux.CurrentroomCapacity[room1] += inst.ESPACE[ENTITY];
                aux.CurrentroomCapacity[room2] -= inst.ESPACE[ENTITY];
                aux.solution[ENTITY] = room2;
            }
            else if (aux.solution[ENTITY] == room2)
            {
                aux.CurrentroomCapacity[room2] += inst.ESPACE[ENTITY];
                aux.CurrentroomCapacity[room1] -= inst.ESPACE[ENTITY];
                aux.solution[ENTITY] = room1;
            }
        }
        castigo = aux.Penalty(inst);
        currentcalidad = aux.MalUso(inst) + castigo;
        // si y solo si la solución es factible y mejor que la actual se guarda como nueva (Mejor Mejora)
        if ((calidad > currentcalidad) && (castigo != -1))
        {
            calidad = currentcalidad;
            cambio = 1;
            nueva.copySolution(inst, aux.solution, aux.CurrentroomCapacity);
        }
    }
    return cambio;
}

double SWAP(Solution &osapSol, Solution &nueva, Instancia &inst)
{
    int cambio = -1;
    double castigo, currentcalidad;
    double calidad = osapSol.Calidad(inst);
    /* Las entidades son desordenadas */
    random_shuffle(&inst.EID[0], &inst.EID[0] + inst.NoOfEntities - 1);
    for (int ENTITY = 0; ENTITY < (inst.NoOfEntities - 1) / 3; ENTITY++)
    {
        int ent1 = inst.EID[ENTITY];
        int ent2 = inst.EID[ENTITY + 1];
        int auxroom1 = osapSol.solution[ent1];
        int auxroom2 = osapSol.solution[ent2];
        /* Se produce el swap de las entidades */
        osapSol.CurrentroomCapacity[auxroom1] += inst.ESPACE[ent1];
        osapSol.CurrentroomCapacity[auxroom2] += inst.ESPACE[ent2];
        osapSol.solution[ent1] = auxroom2;
        osapSol.solution[ent2] = auxroom1;
        osapSol.CurrentroomCapacity[auxroom2] -= inst.ESPACE[ent1];
        osapSol.CurrentroomCapacity[auxroom1] -= inst.ESPACE[ent2];
        /* Se evalúa la solucion vecina */
        castigo = osapSol.Penalty(inst);
        currentcalidad = osapSol.MalUso(inst) + castigo;
        /* Si la solución vecina es mejor y factible (Mejor Mejora) */
        if ((calidad > currentcalidad) && (castigo != -1))
        {
            calidad = currentcalidad;
            nueva.copySolution(inst, osapSol.solution, osapSol.CurrentroomCapacity);
            cambio = 1;
        }
        /* Se deshace el swap de las entidades */
        osapSol.CurrentroomCapacity[auxroom1] -= inst.ESPACE[ent1];
        osapSol.CurrentroomCapacity[auxroom2] -= inst.ESPACE[ent2];
        osapSol.solution[ent1] = auxroom1;
        osapSol.solution[ent2] = auxroom2;
        osapSol.CurrentroomCapacity[auxroom2] += inst.ESPACE[ent1];
        osapSol.CurrentroomCapacity[auxroom1] += inst.ESPACE[ent2];
    }
    /* Sólo si se ha encontrado una solución vecina factible de mayor calidad se actualiza la actual*/
    return cambio;
}

double REALLOCATE(Solution &osapSol, Solution &nueva, Instancia &inst)
{
    int cambio = -1;
    double castigo, currentcalidad;
    double calidad = osapSol.Calidad(inst);
    for (int ENTITY = 0; ENTITY < inst.NoOfEntities; ENTITY++)
    {
        int auxroom = osapSol.solution[ENTITY];
        // se toma una habitación aleatoria
        int room = rand() % inst.NoOfRooms;
        osapSol.CurrentroomCapacity[room] -= inst.ESPACE[ENTITY];
        osapSol.CurrentroomCapacity[auxroom] += inst.ESPACE[ENTITY];
        osapSol.solution[ENTITY] = room;
        castigo = osapSol.Penalty(inst);
        // si la habitación es factible y de mayor calidad (Mejor Mejora)
        if( castigo != -1 )
        {
            currentcalidad = osapSol.MalUso(inst) + castigo;
            if ( calidad > currentcalidad )
            {
                calidad = currentcalidad;
                cambio = 1;
                nueva.copySolution(inst, osapSol.solution, osapSol.CurrentroomCapacity);
            }
        }
        osapSol.CurrentroomCapacity[room] += inst.ESPACE[ENTITY];
        osapSol.CurrentroomCapacity[auxroom] -= inst.ESPACE[ENTITY];
        osapSol.solution[ENTITY] = auxroom;
    }
    return cambio;
}

/* Función que deriva al movimiento determinado y convierte a nueva en current si se ecnontró una mejor solución en el vecindario */
double Vecindario(int movement, Solution &osapSol, Solution &nueva, Solution &aux, Instancia &inst)
{
    int cambio;
    if (movement == 0)
    {
        cambio = REALLOCATE(osapSol, nueva, inst);
    }
    else if (movement == 1)
    {
        cambio = SWAP(osapSol, nueva, inst);
    }
    else if (movement == 2)
    {
        cambio = INTERCHANGE(osapSol, nueva, aux, inst);
    }
    // si la solución cambio (es decir nueva.calidad > osap.calidad), nueva pasa a ser current
    if(cambio == 1)
        osapSol.copySolution( inst, nueva.solution, nueva.CurrentroomCapacity );
    return osapSol.Calidad( inst );
}


/*
* loops determina cuantos restarts se llevarán a cabo
* move determina cual movimiento se realizará 
*               0 -> Reallocate
*               1 -> Swap
*               2 -> Interchange
*               3 -> cada iteracion uno de los 3 es seleccionado aleatoriamente
*/
int HillClimbing(string filename, int loops, int move)
{
    Instancia instancia(filename);
    Solution SolCurrent(instancia);
    Solution SolBest(instancia);
    Solution SolNueva(instancia);
    Solution aux(instancia);


    int local;
    double calidadMin, calidadNuevaSolucion, suma = 0;
    double *calidades = new double[loops];
    double *iniciales = new double[loops];
    double global = numeric_limits<double>::max();
    int n = 0;

    while (n < loops)
    {
        local = 1;
        SolCurrent.CrearSolucionInicial( instancia );
        calidadMin = SolCurrent.Calidad( instancia );
        iniciales[n] = calidadMin;
        while (local)
        {
            int movimiento = move;
            if (move == 3)
                movimiento = rand() % 3;
            calidadNuevaSolucion = Vecindario(movimiento, SolCurrent, SolNueva, aux, instancia);

            if (calidadNuevaSolucion < calidadMin)
                calidadMin = calidadNuevaSolucion;
            else
                local = 0;
        }
        suma += calidadMin;
        calidades[n] = calidadMin;
        if (calidadMin < global)
        {
            SolBest.copySolution(instancia, SolCurrent.solution, SolCurrent.CurrentroomCapacity);
            global = calidadMin;
        }
        n++;
    }

    cout << "Calidad Promedio : " << suma / loops << endl;
    cout << "Mejor solución : " << global << endl;
    Salida outputFile(instancia, SolBest, filename);


    instancia.FreeData();
    return 0;
}