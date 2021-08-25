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
} Propiedad;

typedef struct
{
    string Name;
    Propiedad propiedades[10];
} Comando;

//Estructuras para particiones y discos
typedef struct
{
    char Status_particion;
    char TipoParticion;
    char TipoAjuste;
    int Inicio_particion;
    int TamanioTotal;
    char NombreParticion[15];
} Particion;

//estructura para el mbr
typedef struct
{
    int mbr_tamano;
    char mbr_fecha_creacion[16];
    int No_identificacion;
    char disk_ajuste[1];
    Particion particiones[4];
} MBR;

//estructura para las particiones logicas
typedef struct
{

} EBR;

typedef struct
{
    string NombreParticion;
    string Id;
    char Estado[1];
    char EstadoMKS[1];
} MOUNT;

typedef struct
{
    string NombreDisco;
    string Path;
    char Id[1];
    char Estado[1];
    MOUNT Particines[100];
} DISCO;

class estructuras
{
public:
    estructuras();
    //   ~estructuras();
};

#endif //ESTRUCTURAS_H