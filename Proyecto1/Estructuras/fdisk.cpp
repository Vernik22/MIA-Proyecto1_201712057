#include "fdisk.h"
#include <algorithm>
#include <sys/stat.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>

#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
using namespace std;

fdisk::fdisk()
{
}

void fdisk::ejecutarComandoFdisk(fdisk *disco)
{
    //se ejecuta el comando para formatear el disco

    //abrir el archivo
    FILE *arch;
    int tamanoParticion;
    int startPart;
    arch = fopen(disco->path.c_str(), "rb+"); //se abre el archivo del disco en modo lectura

    if (arch == NULL)
    {
        std::cout << "Error! El disco no existe\n"; //si da null es porque no se encontro el archivo
        return;
    }
    fseek(arch, 0, SEEK_SET);              //se posiciona en el bite 0 del archivo
    MBR mbrTemp;                           //mbr temporal del disco
    fread(&mbrTemp, sizeof(MBR), 1, arch); //mbrTemp se guardara, sizeof es el tamaño en bites del mbr que se lee, 1 es la cantidad de elementos y arch el archivo abierto

    if (disco->unit == "b" || disco->unit == "B") // si viene en bytes
    {
        tamanoParticion = disco->size; //seteamos el tamaño en segun la unidad que se solicita
    }
    else if (disco->unit == "k" || disco->unit == "K")
    {
        tamanoParticion = disco->size * 1024;
    }
    else if (disco->unit == "m" || disco->unit == "M")
    {
        tamanoParticion = disco->size * 1024 * 1024;
    }

    string ajuste = disco->ajuste;
    std::for_each(ajuste.begin(), ajuste.end(), [](char &c)
                  { c = ::tolower(c); });

    if (ajuste == "b" || ajuste == "bf") //mejor ajuste
    {
        /*
        string i ="bf"; //por si se quiere tener bf en vez de b
        strcpy(mbrTemp.particiones[0].TipoAjuste , i.c_str()); */
        mbrTemp.particiones[0].TipoAjuste = 'b';
    }
    else if (ajuste == "f" || ajuste == "ff") //primer ajuste
    {
        mbrTemp.particiones[0].TipoAjuste = 'f';
    }
    else if (ajuste == "w" || ajuste == "wf") //peor ajuste este es el "por defecto"
    {
        mbrTemp.particiones[0].TipoAjuste = 'w';
    }

    //asignar nombre a la particion
    strcpy(mbrTemp.particiones[0].NombreParticion, disco->name.c_str());
    //asignar tamaño de la particion
    mbrTemp.particiones[0].TamanioTotal = tamanoParticion;
    //ajuste que se calculo
    mbrTemp.particiones[0].Inicio_particion = sizeof(MBR);
    //status de la particion 1 porque ya esta ocupada
    mbrTemp.particiones[0].Status_particion = '1';

    if (disco->type == "P" || disco->type == "p")
    {
        mbrTemp.particiones[0].TipoParticion = 'P';
        //aqui va el codigo 
    }
    else if (disco->type == "E" || disco->type == "e")
    {
        mbrTemp.particiones[0].TipoParticion = 'E';
        //agregar el EBR
        EBR auxiliar;
        auxiliar.TipoAjuste = '-';
        auxiliar.NombreParticion[0] = '\0';
        auxiliar.part_next = -1;
        auxiliar.TamanioTotal = -1;
        auxiliar.Inicio_particion = -1;
        auxiliar.Status_particion = '0';

        fseek(arch, mbrTemp.particiones[0].Inicio_particion, SEEK_SET); //se posiciona al inicio de la particion para meter el EBR
        fwrite(&auxiliar, sizeof(EBR), 1, arch);
    }else if (disco->type == "L" || disco->type == "l"){


    }else{
        //default
        printf("Ocurrio un Error\n");
    }

    fseek(arch, 0, SEEK_SET); //modificar en donde esta el mbr original
    fwrite(&mbrTemp, sizeof(MBR), 1, arch);
    fclose(arch);

    if (hayEspacio(tamanoParticion, mbrTemp.mbr_tamano))
    {
        //si hay espacio en el disco para la particion
    }
}

bool fdisk::ejecFdisk(string nombreComando, Propiedad propiedades[], int cont)
{

    try
    {
        fdisk *d = new fdisk();
        //printf("---MKDISK---\n");
        bool paramValid = true;
        bool flagFit = true;  //bandera para este parametro que es opcional
        bool flagUnit = true; //bandera para este parametro que es opcional
        bool flagAdd = true;
        bool flagDelete = true;
        bool flagType = true;
        for (int i = 0; i < 10; i++)
        {
            Propiedad propiedadTemp = propiedades[i];
            string nombrePropiedad = propiedadTemp.Name;
            std::for_each(nombrePropiedad.begin(), nombrePropiedad.end(), [](char &c)
                          { c = ::tolower(c); });

            if (nombrePropiedad == "-size")
            {
                int s = stoi(propiedadTemp.val);
                d->size = s;
            }
            else if (nombrePropiedad == "-f")
            {
                flagFit = false;
                d->ajuste = propiedadTemp.val;
            }
            else if (nombrePropiedad == "-u")
            {
                flagUnit = false;
                d->unit = propiedadTemp.val;
            }
            else if (nombrePropiedad == "-path")
            {

                d->path = propiedadTemp.val;
            }
            else if (nombrePropiedad == "-type")
            {
                flagType = false;
                d->type = propiedadTemp.val;
            }
            else if (nombrePropiedad == "-delete")
            {
                flagDelete = false;
                d->borrar = propiedadTemp.val;
            }
            else if (nombrePropiedad == "-name")
            {
                d->name = propiedadTemp.val;
            }
            else if (nombrePropiedad == "-add")
            {
                flagAdd = false;
                int s = stoi(propiedadTemp.val);
                d->add = s;
            }
            else if (nombrePropiedad != "")
            {
                paramValid = false;
            }
        }

        if (flagFit) // si el parametro no venia entra aqui y se pone el parametro por defecto
        {
            d->ajuste = "wf";
        }
        else if (flagUnit) // si el parametro no venia entra aqui y se pone el parametro por defecto
        {
            d->unit = "k";
        }
        else if (flagType) // si el parametro no venia entra aqui y se pone el parametro por defecto
        {
            d->type = "p";
        }
        else if (flagDelete) // si el parametro no venia entra aqui y se pone el parametro por defecto
        {
            d->borrar = ""; //el fast es por defecto
        }
        else if (flagAdd) // si el parametro no venia entra aqui y se pone el parametro por defecto
        {
            d->add = 0;
        }

        if (paramValid)
        {
            dirExist(d);
            return paramValid;
        }
        else
        {
            return paramValid;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void fdisk::dirExist(fdisk *disco)
{
    printf("------------------------------Ejecutar comando FDISK------------------------------\n");

    //verificar que el directorio existe
    struct stat st;
    if (stat(disco->path.c_str(), &st) == 0)
    {
        printf("Existe el archivo en: %s\n", disco->path.c_str());
        ejecutarComandoFdisk(disco);
    }
    else
    {
        printf("No Existe el archivo: %s\n", path.c_str());
    }
}

vector<string> fdisk::split(string str, char pattern)
{

    int posInit = 0;
    int posFound = 0;
    string splitted;
    vector<string> results;

    while (posFound >= 0)
    {
        posFound = str.find(pattern, posInit);
        splitted = str.substr(posInit, posFound - posInit);
        posInit = posFound + 1;
        results.push_back(splitted);
    }

    return results;
}

void fdisk::imprimirDatosDisco(string path)
{
    //  std::cout << "SI LLAME A IMPRIMIR DATOS DISCO\n";
    FILE *arch;
    arch = fopen(path.c_str(), "rb+");
    if (arch == NULL)
    {
        //return 0;
    }
    MBR mbr;
    fseek(arch, 0, SEEK_SET);
    fread(&mbr, sizeof(MBR), 1, arch);
    fclose(arch);
    std::cout << "\n----------DATOS DEL DISCO-----\n";
    std::cout << "\nMBR NAME: " << mbr.No_identificacion;
    std::cout << "\nMBR SIZE: " << mbr.mbr_tamano;

    for (int i = 0; i < 4; i++)
    {
        //  if(MBR.mbr_partitions[i]!=NULL){
        if (mbr.particiones[i].Status_particion == '1')
        {
            std::cout << "\n\nPARTICION : " << i;
            std::cout << "\nPARTICION STATUS : " << mbr.particiones[i].Status_particion;
            std::cout << "\nPARTICION TYPE : " << mbr.particiones[i].TipoParticion;
            std::cout << "\nPARTICION FIT : " << mbr.particiones[i].TipoAjuste;
            std::cout << "\nPARTICION START : " << mbr.particiones[i].Inicio_particion;
            std::cout << "\nPARTICION SIZE : " << mbr.particiones[i].TamanioTotal;
            std::cout << "\nPARTICION NAME : " << mbr.particiones[i].NombreParticion;
            // }
        }
    }
}

bool fdisk::hayEspacio(int tamanoParticion, int tamanoDisco)
{
    if (tamanoParticion > tamanoDisco || tamanoParticion < 0)
    {
        printf("ERROR -----> El tamaño de la particion es mayor al tamaño del disco o el tamaño es incorrecto\n");
        return false;
    }

    return true;
}