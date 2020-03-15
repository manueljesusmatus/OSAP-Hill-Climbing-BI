#include "Solution.h"
#include "AlgorithmHC.h"

double SWAP(Solution &osapSol, double calidad)
{
    int entity = -1;
    int room1,room2;
    for (int ENTITY = 0; ENTITY < osapSol.NoOfEntities-1; ENTITY++)
    {
        int auxroom1 = osapSol.solution[ENTITY];
        int auxroom2 = osapSol.solution[ENTITY + 1];
        osapSol.CurrentroomCapacity[auxroom1] += osapSol.ESPACE[ENTITY];
        osapSol.CurrentroomCapacity[auxroom2] += osapSol.ESPACE[ENTITY + 1];
        // SWAP
        osapSol.solution[ENTITY] = auxroom2;
        osapSol.solution[ENTITY + 1] = auxroom1;
        osapSol.CurrentroomCapacity[auxroom2] -= osapSol.ESPACE[ENTITY];
        osapSol.CurrentroomCapacity[auxroom1] -= osapSol.ESPACE[ENTITY + 1];
        double currentcalidad = osapSol.MalUso() + (double)osapSol.Penalty();
        if ((calidad > currentcalidad) && (currentcalidad != -1))
        {
            calidad = currentcalidad;
            entity = ENTITY;
            room1 = auxroom1;
            room2 = auxroom2;
        }
        osapSol.solution[ENTITY] = auxroom1;
        osapSol.solution[ENTITY + 1] = auxroom2;
        osapSol.CurrentroomCapacity[auxroom2] += osapSol.ESPACE[ENTITY];
        osapSol.CurrentroomCapacity[auxroom1] += osapSol.ESPACE[ENTITY + 1];
        osapSol.CurrentroomCapacity[auxroom1] -= osapSol.ESPACE[ENTITY];
        osapSol.CurrentroomCapacity[auxroom2] -= osapSol.ESPACE[ENTITY + 1];
    }
    if(entity != -1){
        osapSol.solution[entity] = room2;
        osapSol.solution[entity + 1] = room1;
        osapSol.CurrentroomCapacity[room2] -= osapSol.ESPACE[entity];
        osapSol.CurrentroomCapacity[room1] -= osapSol.ESPACE[entity + 1];
    }
    return calidad;
}

double REALLOCATE(Solution &osapSol, double calidad)
{
    int finalroom = -1;
    int finalentity;
    for (int ENTITY = 0; ENTITY < osapSol.NoOfEntities; ENTITY++)
    {
        int auxroom = osapSol.solution[ENTITY];
        //int room = rand() % osapSol.NoOfRooms;  
        int room = osapSol.SelectBestRoom(osapSol.NoOfRooms, ENTITY);
        //int room = osapSol.SelectBestRoom(osapSol.NoOfRooms/3, ENTITY);
        osapSol.CurrentroomCapacity[room] -= osapSol.ESPACE[ENTITY];
        osapSol.CurrentroomCapacity[auxroom] += osapSol.ESPACE[ENTITY];
        osapSol.solution[ENTITY] = room;
        double castigo = (double)osapSol.Penalty();
        double currentcalidad = osapSol.MalUso() + castigo;
        if ((calidad > currentcalidad) && (castigo != -1))
        {
            calidad = currentcalidad;
            finalentity = ENTITY;
            finalroom = room;
        }
        osapSol.solution[ENTITY] = auxroom;
        osapSol.CurrentroomCapacity[room] += osapSol.ESPACE[ENTITY];
        osapSol.CurrentroomCapacity[auxroom] -= osapSol.ESPACE[ENTITY];
    }
    if( finalroom != -1){
        osapSol.CurrentroomCapacity[finalroom] -= osapSol.ESPACE[finalentity];
        osapSol.CurrentroomCapacity[osapSol.solution[finalentity]] += osapSol.ESPACE[finalentity];
        osapSol.solution[finalentity] = finalroom;
    }
    return calidad;
}

double Vecindario(int movement, Solution &osapSol)
{
    double calidad = numeric_limits<double>::max();
    switch (movement)
    {
    case 0:
        calidad = SWAP(osapSol, calidad);
        break;
    case 1: // reallocate
        calidad = REALLOCATE(osapSol, calidad);
        break;
    case 2:
        //INTERCHANGE
        break;
    }
    return calidad;
}

int HillClimbing(string filename)
{
    Solution osapSol(filename);
    double global = numeric_limits<double>::max();
    int loops = 10;
    double calidadMin;
    int local;
    double calidadNuevaSolucion;
    double suma = 0;
    double castigo;
    int n = 0;
    while (n < loops)
    {
        osapSol.CrearSolucionInicial();
        local = 1;
        castigo = (double)osapSol.Penalty();
        calidadMin = osapSol.MalUso() + castigo;
        cout << "--> " << calidadMin << "   ---> "<< castigo << endl;
        if ( castigo == -1)
            osapSol.ShowSolution();
        while (local)
        {
            calidadNuevaSolucion = Vecindario( rand() % 2 , osapSol);
            if (calidadNuevaSolucion < calidadMin)
            {
                calidadMin = calidadNuevaSolucion;
            }
            else
            {
                local = 0;
            }
        }
        suma += calidadNuevaSolucion;
        if (calidadNuevaSolucion < global)
        {
            global = calidadNuevaSolucion;
        }
        if( n%100 == 0){
            cout << n << endl;
        }
        n++;
    }
    cout << suma / loops << endl;
    cout << global << endl;


    osapSol.FreeData();

    return 0;
}