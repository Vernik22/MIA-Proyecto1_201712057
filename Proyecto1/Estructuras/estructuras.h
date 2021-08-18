#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H
#include "time.h"
#include <iostream>
#include <string>
using namespace std;

//estructura para cada comando y sus propiedades
typedef struct 
{
    string Name;
    string val;
}Propiedad;

typedef struct 
{
    string Name;
    Propiedad propiedades[];
}Comando;

//Estructuras para particiones y discos
typedef struct 
{
    char Status_particion;
    char TipoParticion;
    char TipoAjuste[2];
    int Inicio_particion;
    int TamanioTotal;
    char NombreParticion[15];
}Particion;

//estructura para el mbr
typedef struct{
    int mbr_tamano;
    char mbr_fecha_creacion[16];
    int No_identificacion;
    char disk_ajusto[2];
    Particion particiones[4];
}MBR;

//estructura para las particiones logicas
typedef struct{

}EBR;

class estructuras
{
public:
    estructuras();
 //   ~estructuras();
};


#endif //ESTRUCTURAS_H