#include "Salida.h"

Salida::Salida(Instancia &instancia, Solution &solution, string Filename)
{
    string file = "Salidas/"+Filename+".out";
    cout << file << endl;
    this->myfile.open( file );
    Restricciones_blandas(instancia, solution);
    Espacios(instancia, solution);
    Habitacion(instancia, solution);
    this->myfile.close();
}

void Salida::Restricciones_blandas(Instancia &instancia, Solution &solution)
{
    int restriccionesNoCumplidas = 0;
    for (int i = 0; i < instancia.NoOfConstraints; i++)
    {
        if (instancia.SorH[i] == 0)
        {
            int VALOR = solution.Constraints(instancia, instancia.CTYPE[i], instancia.C1[i], instancia.C2[i]);
            if (VALOR == 1)
                restriccionesNoCumplidas += VALOR;
        }
    }
    this->myfile << left << setw(4) << restriccionesNoCumplidas;
    for (int i = 0; i < instancia.NoOfConstraints; i++)
    {
        if (instancia.SorH[i] == 0)
        {
            if (solution.Constraints(instancia, instancia.CTYPE[i], instancia.C1[i], instancia.C2[i]) == 1)
                this->myfile << setw(4) << i;
        }
    }
    this->myfile << endl;
}

void Salida::Espacios(Instancia &instancia, Solution &solution)
{
    double sumaDesperdiciado = 0;
    double sumaSobreutilizado = 0;
    double espacio;
    for (int ROOM = 0; ROOM < instancia.NoOfRooms; ROOM++)
    {
        espacio = solution.CurrentroomCapacity[ROOM];
        sumaDesperdiciado += max(0.0, espacio);
        sumaSobreutilizado += max(0.0, -espacio);
    }
    this->myfile << left << setw(8) << sumaDesperdiciado + sumaSobreutilizado;
    this->myfile << setw(8) << sumaDesperdiciado;
    this->myfile << setw(8) << sumaSobreutilizado;
    this->myfile << endl;
}

void Salida::Habitacion(Instancia &instancia, Solution &solution)
{
    double espacio;
    for (int ROOM = 0; ROOM < instancia.NoOfRooms; ROOM++)
    {
        int suma = 0;
        espacio = solution.CurrentroomCapacity[ROOM];
        this->myfile << left << setw(8) << ROOM;
        this->myfile << setw(8) << max(0.0, espacio);
        this->myfile << setw(8) << max(0.0, -espacio);
        for ( int ENTITY = 0; ENTITY < instancia.NoOfEntities; ENTITY++)
        {
            if( solution.solution[ENTITY] == ROOM )
                suma += 1;
        }
        this->myfile << setw(5) << suma;
        for ( int ENTITY = 0; ENTITY < instancia.NoOfEntities; ENTITY++)
        {
            if( solution.solution[ENTITY] == ROOM )
                this->myfile << setw(5) << ENTITY;
        }
        this->myfile << endl;
    }    
}