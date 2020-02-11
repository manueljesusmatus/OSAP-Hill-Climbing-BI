#ifndef externo
int Tinstancia;
#endif

#ifdef externo
extern int Tinstancia;
#endif

struct coordenada{
  float x, y;
};
  
class solucion{
  friend ostream &operator<<(ostream &, const solucion &);

  public:
    int aptitud;
    bool usado;
    vector <int> tour;

  solucion(){   // Constructor
    aptitud=AMAX;
    usado=true;
    tour=vector<int>(Tinstancia, 1);
  }
  
  solucion(const solucion &a){
    aptitud=a.aptitud;
    tour=a.tour;
  }

  solucion& operator=(const solucion &a){
    this->aptitud=a.aptitud;
    this->usado=a.usado;
    this->tour=a.tour;
    return *this;
  }

  int operator==(const solucion &a) const{
    if( this->aptitud != a.aptitud) return 0;
    return 1;
  }

  int operator<(const solucion &a) const{
    if( this->aptitud < a.aptitud ) return 1;
    return 0;
  }
};

ostream &operator<<(ostream &output, const solucion &a){
  for(int i=0;i<a.tour.size();i++)
    output<<a.tour[i]<<" ";
  output<<"("<<a.aptitud<<")";  
  return output;
}

char* archivo_configuracion;
char* archivo_resultados;
int semilla;

int **distancia;
coordenada *coordenadas;
int tipodistancia;

solucion optimo_global,mejor_solucion;

int iteracion, restart, Mr;
int debug;
float aptitud_opt;
int iteracion_opt;

int leer_entradas(int,char **);
int int_rand (int,int);
void calcular_aptitud(solucion *);
void validar_permutacion(solucion *);
void escribir_en_archivo_resultados();
void leer_archivo_configuracion(void);
void salir(void);

