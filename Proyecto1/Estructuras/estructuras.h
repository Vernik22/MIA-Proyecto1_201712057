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
    char disk_ajuste[2];
    Particion particiones[4];
} MBR;

//estructura para las particiones logicas
typedef struct
{
    char Status_particion;
    char TipoAjuste;
    int Inicio_particion;
    int TamanioTotal;
    int part_next;
    char NombreParticion[15];
} EBR;

typedef struct
{
    string NombreParticion;
    string id;
    char estado;
    char EstadoMKS;
} MOUNT;

typedef struct
{
    string NombreDisco;
    string Path;
    char id;
    char estado;
    MOUNT Particiones[100];
} DISCO;

typedef struct
{
    char Journal_Tipo_Operacion[10]; //mkfs,mkfile,...etc
    char Journal_tipo;               //Carpeta(0), Archivo(1).
    char Journal_nombre[200];        //..../home....../home/chepe........../home/chepe/datos.txt
    char Journal_contenido[200];     //....este es el contenido de mi archivo de texto.....-
    char Journal_fecha[16];          // 18/06/2019 10:30
    char Journal_propietario[10];    //Quien creó el archivo o carpeta
    int Journal_permisos;            //777...664
} JOURNAL;

class estructuras
{
public:
    estructuras();
    //   ~estructuras();
};

#endif //ESTRUCTURAS_H