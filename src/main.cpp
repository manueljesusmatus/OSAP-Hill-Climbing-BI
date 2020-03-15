#include <iostream>
#include <string>
#include <time.h>
#include <stdio.h>
#include "AlgorithmHC.h"
using namespace std;

int main(int argc, char const *argv[])
{
    /* SEMILLA ALEATORIA PARA LOS RANDOMS QUE SE HAN DE EJECUTAR */
    clock_t t_ini, t_prev, t_fin;
    double secs;
    t_ini = clock();
    srand(time(NULL));

    int largo = 1;//5+6+6+6+6+6+6;
    string archivos[largo] = {/*
                        "nott1e","nott1d","nott1c","nott1b","nott1",
                        "p000_n000","p000_n005","p000_n010","p000_n015","p000_n020","p000_n025",
                        "p005_n000","p005_n005","p005_n010","p005_n015","p005_n020","p005_n025",
                        "p010_n000",*/"p010_n005"}/*,"p010_n010","p010_n015","p010_n020","p010_n025",
                        "p015_n000","p015_n005","p015_n010","p015_n015","p015_n020","p015_n025",
                        "p020_n000","p020_n005","p020_n010","p020_n015","p020_n020","p020_n025",
                        "p025_n000","p025_n005","p025_n010","p025_n015","p025_n020","p025_n025"
                        }*/;
    int loops = 100;


    cout << "loops : " << loops <<endl << endl;
    for(int move = 3; move < 4; move++){
        cout << "------ MOVE : " << move << " ------"<< endl;
        for( int c=0; c < largo; c++)
        {
            t_prev = clock();
            cout << "Abriendo archivo Instancias_OSAP/" + archivos[c] << endl;
            HillClimbing(archivos[c], loops, move);
            secs = (double)(clock() - t_prev) / CLOCKS_PER_SEC;
            printf("%.2g minutos\n", secs / 60 );
            cout << endl;
        }
    }

    //HillClimbing("Instancias_OSAP/" + filename);


    t_fin = clock();
    secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
    printf("%.2g minutos\n", secs / 60 );
    cout << endl;
    return 0;
}