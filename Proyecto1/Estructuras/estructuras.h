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

//structura para las particiones a montar
typedef struct
{
    string NombreParticion;
    string id;
    char estado;
    char EstadoMKS;
} MOUNT;

//Estructura de un disco
typedef struct
{
    string NombreDisco;
    string Path;
    char id;
    char estado;
    MOUNT Particiones[100];
} DISCO;

//journaling para la bitacora
/*
typedef struct
{
    char Journal_Tipo_Operacion[19]; //mkfs,mkfile,...etc
    char Journal_tipo;               //Carpeta(0), Archivo(1).
    char Journal_nombre[200];        //..../home....../home/chepe........../home/chepe/datos.txt
    char Journal_contenido[200];     //....este es el contenido de mi archivo de texto.....-
    char Journal_fecha[16];          // 18/06/2019 10:30
    char Journal_propietario[10];    //Quien creó el archivo o carpeta
    int Journal_permisos;            //777...664
    int Size;
} JOURNAL;
*/
typedef struct 
{
char Log_tipo_operacion [19];
char Log_tipo;
char Log_nombre [35];
char Log_Contenido [25];
char Log_fecha [19];
int Size;
}JOURNAL;


//superbloque
typedef struct
{
    char Sb_nombre_hd[15];
    int Sb_arbol_virtual_count;
    int Sb_detalle_directorio_count;
    int Sb_inodos_count;
    int Sb_bloques_count;
    int Sb_arbol_virtual_free;
    int Sb_detalle_directorio_free;
    int Sb_inodos_free;
    int Sb_bloques_free;
    char Sb_date_creacion[19];
    char Sb_date_ultimo_montaje[19];
    int Sb_montajes_count;
    int Sb_ap_bitmap_arbol_directorio;
    int Sb_ap_arbol_directorio;
    int Sb_ap_bitmap_detalle_directorio;
    int Sb_ap_detalle_directorio;
    int Sb_ap_bitmap_tabla_inodo;
    int Sb_ap_tabla_inodo;
    int Sb_ap_bitmap_bloques;
    int Sb_ap_bloques;
    int Sb_ap_log;
    int Sb_size_struct_arbol_directorio;
    int Sb_size_struct_Detalle_directorio;
    int Sb_size_struct_inodo;
    int Sb_size_struct_bloque;
    int Sb_first_free_bit_arbol_directorio;
    int Sb_first_free_bit_detalle_directoriio;
    int Sb_dirst_free_bit_tabla_inodo;
    int Sb_first_free_bit_bloques;
    int Sb_magic_num;
    int InicioCopiaSB;
    int ConteoAVD;
    int ConteoDD;
    int ConteoInodo;
    int ConteoBloque;
} SB;

//arbol virtual de directorio

typedef struct
{
    char Avd_fecha_creacion[19];
    char Avd_nomre_directotrio[15];
    int Avd_ap_array_subdirectoios[6];
    int Avd_ap_detalle_directorio;
    int Avd_ap_arbol_virtual_directorio;
    char Avd_proper[10];
} AVD;

//Detalle de Directorio
typedef struct
{
    char Dd_file_nombre[15];
    int Dd_file_ap_inodo;
    char Dd_file_date_creacion[19];
    char Dd_file_date_modificacion[19];
} ArregloDD;

//DD
typedef struct
{
    ArregloDD Dd_array_files[5];
    int Dd_ap_detalle_directorio;
    int Ocupado;
} DD;

//cantidad de Inodos
typedef struct
{
    int I_count_inodo;
    int I_size_archivo;
    int I_count_bloques_asignados;
    int I_array_bloques[4];
    int I_ao_indirecto;
    int I_id_proper;
} Inodo;

//Bloque de archivo
typedef struct
{
    char Db_data[64];
} Bloque;

class estructuras
{
public:
    estructuras();
    //   ~estructuras();
};

#endif //ESTRUCTURAS_H