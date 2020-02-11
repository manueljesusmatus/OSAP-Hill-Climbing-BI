#include <iostream>
#include <fstream>
#include <string>
#include "DataOSAP.h"
using namespace std;

int main(int argc, char const *argv[])
{
    string filename = "nott1.txt";
    DataOSAP d(filename);
    cout << d.NoOfConstraints << endl;
    d.FreeData();
    return 0;
}