#include "Solution.h"
#include "AlgorithmHC.h"

double select_move(int movement, Solution &osapSol)
{
    double calidad = numeric_limits<double>::max();
    switch (movement)
    {
    case 0:
        //SWAP
        break;
    case 1: // reallocate
        for (int ENTITY = 0; ENTITY < osapSol.NoOfEntities; ENTITY++)
        {
            int auxroom = osapSol.solution[ENTITY];
            int room = osapSol.SelectBestRoom(osapSol.setKRooms(ENTITY, osapSol.NoOfRooms / 3), ENTITY);
            osapSol.CurrentroomCapacity[room] -= osapSol.ESPACE[ENTITY];
            osapSol.CurrentroomCapacity[auxroom] += osapSol.ESPACE[ENTITY];
            osapSol.solution[ENTITY] = room;
            double currentcalidad = osapSol.MalUso() + (double)osapSol.Penalty();
            if ((calidad > currentcalidad) && ( currentcalidad != -1 ))
            {
                calidad = currentcalidad;
                //cout << ENTITY << endl;
            }
            else
            {
                osapSol.solution[ENTITY] = auxroom;
                osapSol.CurrentroomCapacity[room] += osapSol.ESPACE[ENTITY];
                osapSol.CurrentroomCapacity[auxroom] -= osapSol.ESPACE[ENTITY];
            }
        }
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

    osapSol.CrearSolucionInicial();
    double global = numeric_limits<double>::max();
    double calidadMin; 
    int local;
    double c;


    int n = 0;
    while (n < 100)
    {
        local = 1;
        calidadMin = osapSol.MalUso() + (double)osapSol.Penalty();
        while (local)
        {
            c = select_move(1, osapSol);
            if (c < calidadMin)
            {
                calidadMin = c;
            }
            else
            {
                local = 0;
            }
            //cout << c << endl;
        }
        if( c < global){
            global = c;
        }
        n++;
    }

    cout << global << endl;

    osapSol.FreeData();

    return 0;
}