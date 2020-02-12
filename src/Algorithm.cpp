#include "Solution.h"
#include "Algorithm.h"

int translate(int n){
    if(n == 0)
        return 1;
    else
        return 0;
}

int penalty(int tipoderestrccion, int param1, int param2, Solution& osap)
{
    int n;
    switch (tipoderestrccion)
    {
    case 0:
        n = osap.C_ALLOCATION_CONSTRAINT(param1,param2);
        break;
    case 1:
        n = osap.C_NONALLOCATION_CONSTRAINT(param1,param2);
        break;
    case 2:
        cout << "no debería haber" << endl;
        break;
    case 3:
        n = osap.C_CAPACITY_CONSTRAINT(param1);
        break;
    case 4:
        n = osap.C_SAMEROOM_CONSTRAINT(param1,param2);
        break;
    case 5:
        n = osap.C_NOTSAMEROOM_CONSTRAINT(param1,param2);
        break;
    case 6:
        n = osap.C_NOTSHARING_CONSTRAINT(param1);
        break;
    case 7:
        n = osap.C_ADJACENCY_CONSTRAINT(param1,param2);
        break;
    case 8:
        n = osap.C_NEARBY_CONSTRAINT(param1,param2);
        break;
    case 9:
        n = osap.C_AWAYFROM_CONSTRAINT(param1,param2);
        break;
    default:
        cout << "ALGO SALIO MAL" << endl;
        break;
    }
    return translate(n);
}

int SC_penalizacion(Solution& osap)
{
    int suma = 0;
    for (int h = 0; h < osap.NoOfConstraints; h++)
    {
        if (osap.SorH[h] == 0)
        {
            int y = penalty(osap.CTYPE[h], osap.C1[h], osap.C2[h], osap);
            int w = osap.TypeConstraints[ osap.CTYPE[h] ];
            suma += w*y;
            cout << osap.CID[h] << "-" << osap.C1[h] << "-" << osap.C2[h] << "-" << y << "-" << suma <<endl;
        }
    }
    return suma;
}

int HillClimbing(string filename)
{
    Solution osapSol(filename);

    /* wea mala */
    osapSol.CrearSolucionInicial();
    /* print de la weaita */
    osapSol.ShowSolution();

    SC_penalizacion(osapSol);

    osapSol.FreeData();
    return 0;
}