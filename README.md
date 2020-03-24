
# OSAP-Hill-Climbing MM + R

  * Manuel Jesús Matus Huaiquiñir
  * 201673533-K

## Build

make

## Run

 > ./main
 > Se ejecutarán los archivos escritos en el vector *archivos* en main.cpp 

>./main nott1e 
> Se ejecutará solo la instancia escrita (el archivo debe ser escrito sin la extensión *.txt*)

### Configuración
* Para seleccionar los archivos a ejecutar estos deben añadidos al vector archivos, los archivos deben ser escritos sin la extensión *.txt*
 * Para seleccionar el movimiento a realizar, la variable move debe ser modificada
 * * 0 -> Relocate
 * * 1 -> Swap
 * * 2 -> Interchange
 * * 3 -> Todas
 
## Notas
* Los archivos .h se encuentran comentados
* El tiempo promedio para las instancias PNe150 es entre 5 a 6 minutos por instacia
* El tiempo para las instancias nott es alrededor de 0.5 a  2 min, excepto por nott1 el cual alcanza 12 minutos

## Sobre los archivos .cpp
> AlgorithmHC lleva a cabo el algoritmo HCMM+R con las respectivas funciones que crean el vecindario en cada iteración.

> Solution, Contiene la estructura correspondiente a la representación de la solución además de un arreglo CurrentRoomCapacity el cual es usado para facilitar el calculo del espacio.

> Instancia, Contiene la data extraida de la Instancia bajo la cual se está ejecutando *HCalgorithm*

> Salida, archivo que construye el formato de salida para cada solución, los archivos creados se guardan en la carpeta *Salidas*

> main, archivo principal que ejecuta el programa
