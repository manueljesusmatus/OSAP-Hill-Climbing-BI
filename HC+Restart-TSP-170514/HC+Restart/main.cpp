#include "includes.h"
#include "globales.h"

void SWaP(solucion * solucion_actual, solucion * candidata_solucion, int pos1, int pos2){
  int aux;
  //copiar la informacion de solucion actual en candidata a solucion
  pos2=pos2%Tinstancia  ;
  *candidata_solucion=*solucion_actual;
  if(debug) cout<<"\t"<<pos1<<"->"<<pos2;
  //Intercambiar la informacion 
  aux=(candidata_solucion)->tour[pos1];
  (candidata_solucion)->tour[pos1]=(candidata_solucion)->tour[pos2];
  (candidata_solucion)->tour[pos2]=aux;
  //Validar permutacion y marcar como usado
  validar_permutacion(candidata_solucion);
  calcular_aptitud(candidata_solucion);
  return;
}

void escribir_en_archivo_resultados (void){
  ofstream res;
  res.open(archivo_resultados);
  res<<mejor_solucion<<endl;
  res.close();
  cout<<mejor_solucion<<endl;
  return;
}

void salir(void){
  escribir_en_archivo_resultados();
  //salida sin error
  exit(0);
  return;
}

void calcular_aptitud(solucion * temp){
  int apt=0;
  //calcular el costo del tour
  for(int i=0; i<(Tinstancia-1); i++)
    apt+=(int)distancia[temp->tour[i]][temp->tour[i+1]];
  
  apt+=(int)distancia[temp->tour[Tinstancia-1]][temp->tour[0]];
  
  temp->aptitud=apt;

  if(iteracion>0)
    if(apt==optimo_global.aptitud){
      mejor_solucion=*temp;
      cout<<"Optimo encontrado!"<<endl;
      salir();
    }

  return;
}

void validar_permutacion(solucion * temp){
  int set[Tinstancia];
  for(int i=0;i<Tinstancia;i++)
      set[i]= 1; 
  for(int i=0;i<Tinstancia;i++){
    if(set[temp->tour[i]]==0){
      cout<<"ERROR: Solucion invalida"<<endl; 
      salir();
    }
    set[temp->tour[i]]=0;
  }
  return;
}

float conv_rad(float coord){
  int int_coord=coord;
  double dif=coord-int_coord;
  return (PI*(int_coord+5.0*dif/3.0)/180.0);
}

void leer_archivo_configuracion(void){

  char instancia[1000];
  //lectura de archivo de configuracion
  FILE *configuracion;
  char buf[100];
  int i,j;

  configuracion=fopen(archivo_configuracion, "r");
  int rt=fscanf(configuracion,"%s",buf);
  while(strcmp("NODE_COORD_SECTION",buf)!=0){
      if(strcmp("EDGE_WEIGHT_TYPE",buf)==0){
	rt=fscanf(configuracion,"%s",buf); //los dos puntos que estan separados
        rt=fscanf(configuracion,"%s",buf);
	if(strcmp("GEO",buf)==0) 
          tipodistancia=GEO;
	else if(strcmp("EUC_2D",buf)==0) 
	  tipodistancia=EUC;
	buf[0]=0;
      }
      else if(strcmp("EDGE_WEIGHT_TYPE:",buf)==0){
	int rt=fscanf(configuracion,"%s",buf);
	if (strcmp("GEO",buf)==0) 
	  tipodistancia=GEO;
	else if(strcmp("EUC_2D",buf)==0) 
	  tipodistancia=EUC;
	buf[0]=0;
      }
      else if(strcmp("DIMENSION",buf)==0){
        rt=fscanf(configuracion,"%s",buf);//los dos puntos que estan separados
	rt=fscanf(configuracion,"%d",&Tinstancia);
	buf[0]=0;
      }
      else if(strcmp("DIMENSION:",buf)==0) {
	rt=fscanf(configuracion, "%d", &Tinstancia);
	buf[0]=0;
      }
      else if(strcmp("NAME",buf)==0)
        buf[0]=0;
      else if(strcmp("NAME:",buf)==0) 
	buf[0]=0;
      rt=fscanf(configuracion,"%s", buf);
    }

  if(strcmp("NODE_COORD_SECTION",buf)==0){
    if(Tinstancia==0){
      cout<<"ERROR: Problemas en la lectura del tamano de la instancia"<<endl;
      salir();
    }

    //Reserva de espacio para el arreglo de coordenadas y la matriz de distancia
    coordenadas=new coordenada[Tinstancia];
    distancia=new int*[Tinstancia];
    for( int i = 0 ; i < Tinstancia ; i++ )
      distancia[i] = new int [Tinstancia];
    //Lectura de las coordenadas de cada punto
    int id;
    float x,y;
    int contador=0;
    while(contador<Tinstancia){
      int rt=fscanf(configuracion,"%d %f %f\n",&id,&x,&y);
      coordenadas[id-1].x=x;
      coordenadas[id-1].y=y;
      contador++;
    }
  }

  //Calculo de la matriz de distancias
  for(int i=0;i<(Tinstancia-1);i++){
    for(int j=i+1;j<Tinstancia;j++){
      if(tipodistancia==EUC)
        distancia[i][j]=round(sqrt(pow((coordenadas[i].x-coordenadas[j].x),2)+pow((coordenadas[i].y-coordenadas[j].y),2)));
      else if(tipodistancia==GEO){
        float RRR=6378.388;
        float latitud1=conv_rad(coordenadas[i].x);
        float longitud1=conv_rad(coordenadas[i].y);
        float latitud2=conv_rad(coordenadas[j].x);
        float longitud2=conv_rad(coordenadas[j].y);
        float q1=cos(longitud1-longitud2);
        float q2=cos(latitud1-latitud2);
        float q3=cos(latitud1+latitud2);
        distancia[i][j]=(int)(RRR*acos(0.5*((1.0+q1)*q2-(1.0-q1)*q3))+1.0);
      }
      distancia[j][i]=distancia[i][j];    
    }	
  }
  if (debug){
    for(int i=0;i<Tinstancia;i++)
      for(int j=0;j<Tinstancia;j++)
        cout<<distancia[i][j]<<" ";
      cout<<endl;  
  }
  strcpy(instancia,archivo_configuracion);
  int largo=strlen(instancia);
  instancia[largo-4]='\0';
  strcat(instancia,".opt.tour");

  FILE *lectura_optimo;
  lectura_optimo=fopen(instancia, "r");
  if(lectura_optimo==NULL){
    cout<<"ERROR: Problemas en la lectura del archivo de tour optimo"<<endl;
    salir();
  }

  do{
    int rt=fscanf(lectura_optimo,"%s",buf); 
  }while(strcmp("TOUR_SECTION", buf)!=0);

  int tmp;
  for(int i=0; i<Tinstancia;i++){
    int rt=fscanf(lectura_optimo, "%d", &tmp);
    optimo_global.tour.push_back(tmp-1);
  }
  calcular_aptitud(&optimo_global);
  
  fclose(lectura_optimo);
  fclose(configuracion);
  return;
}

void crear_solucion_aleatoria (solucion *i_temp){
  int rand;
  //creacion de la permutacion aleatoria
  int set[Tinstancia];
  for(int i=0;i<Tinstancia;i++)
    set[i]=i;

  for (int i=0;i<Tinstancia;i++){
    rand=int_rand(0,(Tinstancia-i)); // Un elemento del arreglo (entre 0 y Tinstancia-1 la primera vez y asi...)
    i_temp->tour[i]=set[rand]; 
    set[rand]=set[Tinstancia-i-1]; 
  }
  //calculo de la aptitud de la solucion
  calcular_aptitud(i_temp);
  return;
}

int int_rand (int a, int b){
  //generacion de numeros aleatorios entre a y b-1
  int retorno=0;
  if (a<b){ 
    retorno=(int)((b-a)*drand48());
    retorno=retorno+a;
  }
  else{
    retorno=(int)((a-b)*drand48());
    retorno=retorno+b;
  }
  return retorno;
}

int leer_entradas(int argc, char **argv){
  //archivo con la instancia del problema
  archivo_configuracion=(char *)(argv[1]);
  
  //archivo donde escribir los resultados de la ejecucion
  archivo_resultados=(char *)(argv[2]);
  
  //SEMILLA
  semilla=atoi(argv[3]);

  //BUDGET
  Mr=atoi(argv[4]);
  if(debug)
    printf("Mr: %d\n", Mr);

  debug=atoi(argv[5]);

  return 1;
}


int main (int argc, char *argv[]){
  
  //lectura de parametros
  if(!leer_entradas(argc,argv)){
    cout<<"Problemas en la lectura de los parametros";
    exit(1);
  }

  //lectura de instancia
  restart=-1;
  iteracion=-1;
  leer_archivo_configuracion();
  solucion solucion_actual, candidata_solucion, mejor_candidata;

  //semilla aleatoria
  srand48 (semilla);
  bool optimo_local;
  
  crear_solucion_aleatoria(&solucion_actual);
  mejor_solucion=solucion_actual;
  if(debug) cout<<"Restart | Iteracion | Solucion-Actual \t\t\t\t |Vecindario \t\t\t\t\t  |Mejor-solucion"<<endl;   
  if(debug) cout<<"-------------------------------------------------------------------------------------";
  if(debug) cout<<"-------------------------------------------------------------------------------------"<<endl;
  for(restart=0;restart<Mr;restart++){
    optimo_local=false;
    iteracion=0;
    do{
      if(debug) cout<<"   "<<restart+1 <<" \t     "<<iteracion+1<<"\t\t"<<solucion_actual<<endl;
      //time_t ini_it=time(NULL);
      mejor_candidata=solucion_actual;
      for(int current=0;current<Tinstancia;current++){
          if(debug) cout<<"\t\t\t\t  \t\t\t";
          SWaP(&solucion_actual, &candidata_solucion, current, current+1);
          if(debug) cout<<"\t"<< candidata_solucion<<endl;
          if(candidata_solucion.aptitud < mejor_candidata.aptitud)
            mejor_candidata=candidata_solucion;
      }
      iteracion++;
      if(debug) getchar();
      if(mejor_candidata.aptitud<solucion_actual.aptitud) //Minimizacion
          solucion_actual=mejor_candidata;
      else
          optimo_local=true;
    }while(!optimo_local);
    if(solucion_actual.aptitud < mejor_solucion.aptitud)
        mejor_solucion = solucion_actual;
    if(debug) cout<<"\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t"<<mejor_solucion<<endl;
    crear_solucion_aleatoria(&solucion_actual);
    if(debug) getchar();
  }
  if(debug) cout<<"-------------------------------------------------------------------------------------"<<endl;
  salir();
}
