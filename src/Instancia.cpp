#include "Instancia.h"

Instancia::Instancia(string FileToRead)
{
    /* LECTURA DE ARCHIVO */
    this->input.open("Instancias_OSAP/" + FileToRead + ".txt");
    if (!this->input.is_open())
    {
        cout << "Error al Abrir archivo" << endl;
        exit(10);
    }
    /* LECTURA DE LAS PRIMERAS 5 VARIABLES */
    this->input >> this->linea >> this->NoOfEntities;
    this->input >> this->linea >> this->NoOfRooms;
    this->input >> this->linea >> this->NoOfFloors;
    this->input >> this->linea >> this->NoOfConstraints;
    this->input >> this->linea >> this->NoOfHardConstraints;
    this->input >> this->linea >> this->NoOfSoftConstraints;

    /********************* ENTITIES ****************************/
    this->input >> this->linea;
    this->EID = new int[this->NoOfEntities];
    this->GID = new int[this->NoOfEntities];
    this->ESPACE = new double[this->NoOfEntities];
    this->mean = 0;
    for (int i = 0; i < this->NoOfEntities; i++)
    {
        this->input >> this->EID[i] >> this->GID[i] >> this->ESPACE[i];
        this->mean += this->ESPACE[i];
    }
    this->mean /= this->NoOfEntities;

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
    this->sd = 0;
    for (int n = 0; n < this->NoOfEntities; n++)
    {
        this->sd += (this->ESPACE[n] - mean) * (this->ESPACE[n] - mean);
    }
    this->sd /= this->NoOfEntities;
    this->sd = sqrt(this->sd);

    this->SetConstraintPenalties();
    this->EntitiesHardConstraint();
    this->setEntities();
}

/* Penalización de las restricciones */
void Instancia::SetConstraintPenalties()
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

void Instancia::FreeData()
{
    /* Free memory */
    this->input.close();
    for (int i = 0; i < this->NoOfRooms; i++)
    {
        free(this->ADJ_LIST[i]);
    }
    free(this->ADJ_LIST);
}

/* Función q obtiene las entidades asociadas a restricciones duras */
void Instancia::EntitiesHardConstraint()
{
    this->HardEntities = new int[this->NoOfHardConstraints * 2];
    this->allocated = new int[this->NoOfHardConstraints * 2];
    int n = 0, m = 0;
    for (int Constraint = 0; Constraint < this->NoOfConstraints; Constraint++)
    {
        if (this->SorH[Constraint] == 1)
        {
            switch (this->CTYPE[Constraint])
            {
            case 0:
                this->allocated[m++] = this->C1[Constraint];
                break;
            case 1:
                this->HardEntities[n++] = this->C1[Constraint];
                break;
            case 4:
                this->HardEntities[n++] = this->C1[Constraint];
                this->HardEntities[n++] = this->C2[Constraint];
                break;
            case 5:
                this->HardEntities[n++] = this->C1[Constraint];
                this->HardEntities[n++] = this->C2[Constraint];
                break;
            case 6:
                this->allocated[m++] = this->C1[Constraint];
                break;
            case 7:
                this->HardEntities[n++] = this->C1[Constraint];
                this->HardEntities[n++] = this->C2[Constraint];
                break;
            case 8:
                this->HardEntities[n++] = this->C1[Constraint];
                this->HardEntities[n++] = this->C2[Constraint];
                break;
            case 9:
                this->HardEntities[n++] = this->C1[Constraint];
                this->HardEntities[n++] = this->C2[Constraint];
                break;
            default:
                break;
            }
        }
    }
    this->EntidadesDuras = n;
    this->EntidadesAllocated = m;
}

/* Función que realiza merge de las entidades GRandes, Duras, Resto a asignar */
void Instancia::setEntities()
{
    int h = 0;
    this->entidades = new int[this->NoOfConstraints * 2 + this->NoOfEntities];
    for (int ENTITY = 0; ENTITY < this->NoOfEntities; ENTITY++)
    {
        if (this->ESPACE[ENTITY] > (this->mean + this->sd * 2))
            this->entidades[h++] = ENTITY;
    }
    this->EntidadesGrandes = h;
    for (int z = 0; z < this->EntidadesDuras; z++)
    {
        this->entidades[h++] = this->HardEntities[z];
    }
    for (int z = 0; z < this->EntidadesAllocated; z++)
    {
        this->entidades[h++] = this->allocated[z];
    }
    for (int c = 0; c < this->NoOfEntities; c++)
    {
        this->entidades[h++] = c;
    }
    this->lenEntidades = this->EntidadesDuras + this->EntidadesGrandes + this->EntidadesAllocated + this->NoOfEntities;
}

Instancia::~Instancia()
{
    delete[] this->TypeConstraints;

    delete[] this->EID;
    delete[] this->GID;
    delete[] this->ESPACE;

    delete[] this->RID;
    delete[] this->FID;
    delete[] this->RSPACE;
    delete[] this->ADJ_LIST_SIZE;

    delete[] this->CID;
    delete[] this->CTYPE;
    delete[] this->SorH;
    delete[] this->C1;
    delete[] this->C2;

    delete[] this->allocated;
    delete[] this->HardEntities;
    delete[] this->entidades;
};