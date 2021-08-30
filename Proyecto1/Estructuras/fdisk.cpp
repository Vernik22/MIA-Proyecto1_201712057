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
    int tamanoParticion;
    int startPart = sizeof(MBR);
    Particion particion;

    MBR mbrTemp; //mbr temporal del disco

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
        particion.TipoAjuste = 'b';
    }
    else if (ajuste == "f" || ajuste == "ff") //primer ajuste
    {
        particion.TipoAjuste = 'f';
    }
    else if (ajuste == "w" || ajuste == "wf") //peor ajuste este es el "por defecto"
    {
        particion.TipoAjuste = 'w';
    }

    //verificar si viene la propiedad eliminar

    if (disco->type == "P" || disco->type == "p")
    {
        FILE *arch;
        arch = fopen(disco->path.c_str(), "rb+"); //se abre el archivo del disco en modo lectura

        if (arch == NULL)
        {
            std::cout << "Error! El disco no existe\n"; //si da null es porque no se encontro el archivo
            return;
        }
        fseek(arch, 0, SEEK_SET);              //se posiciona en el bite 0 del archivo
        fread(&mbrTemp, sizeof(MBR), 1, arch); //mbrTemp se guardara, sizeof es el tamaño en bites del mbr que se lee, 1 es la cantidad de elementos y arch el archivo abierto

        //aqui va el codigo
        Particion parts[4];
        //llenar particones
        for (int i = 0; i < 4; i++)
        {
            parts[i] = mbrTemp.particiones[i];
        }

        if (hayEspacio(tamanoParticion, mbrTemp.mbr_tamano))
        {
            //si hay espacio en el disco para la particion
            //verifica si hay particiones disponibles
            if (parts[0].Status_particion == '1')
            {
                printf("Ya existe una particion\n");
                for (int i = 0; i < 4; i++)
                {
                    //posicion en bytes del partstart de la n particion
                    startPart += parts[i].TamanioTotal;
                    if (parts[i].Status_particion == '0')
                    {
                        cout << startPart << endl;
                        break;
                    }
                }
            }
            if (hayEspacio(startPart + tamanoParticion, mbrTemp.mbr_tamano))
            {
                particion.Status_particion = '1';
                particion.TipoParticion = 'P';
                particion.Inicio_particion = startPart;
                particion.TamanioTotal = tamanoParticion;
                strcpy(particion.NombreParticion, disco->name.c_str());

                //copiando particion creada
                for (int i = 0; i < 4; i++)
                {
                    if (parts[i].Status_particion == '0')
                    {
                        parts[i] = particion;
                        break;
                    }
                }
                fseek(arch, 0, SEEK_SET); //modificar en donde esta el mbr original
                for (int i = 0; i < 4; i++)
                {
                    mbrTemp.particiones[i] = parts[i];
                }
                fwrite(&mbrTemp, sizeof(MBR), 1, arch);
                fclose(arch);
                imprimirDatosDisco(disco->path);
            }
        }
    }
    else if (disco->type == "E" || disco->type == "e")
    {
        FILE *arch;
        arch = fopen(disco->path.c_str(), "rb+"); //se abre el archivo del disco en modo lectura

        if (arch == NULL)
        {
            std::cout << "Error! El disco no existe\n"; //si da null es porque no se encontro el archivo
            return;
        }
        fseek(arch, 0, SEEK_SET);              //se posiciona en el bite 0 del archivo
        fread(&mbrTemp, sizeof(MBR), 1, arch); //mbrTemp se guardara, sizeof es el tamaño en bites del mbr que se lee, 1 es la cantidad de elementos y arch el archivo abierto
        //Particiones Extendidas
        Particion parts[4];
        bool flagExt = true;
        for (int i = 0; i < 4; i++)
        {
            parts[i] = mbrTemp.particiones[i];
            if (parts[i].TipoParticion == 'E')
            {
                flagExt = false;
            }
        }
        if (hayEspacio(tamanoParticion, mbrTemp.mbr_tamano))
        {
            //verifica si ya hay particiones
            if (parts[0].Status_particion == '1')
            {
                printf("Ya existe una particion\n");
                for (int i = 0; i < 4; i++)
                {
                    //posicion del partstart de la n particion
                    startPart += parts[i].TamanioTotal;
                    if (parts[i].Status_particion == '0')
                    {
                        cout << startPart << endl;
                        break;
                    }
                }
            }
            if (hayEspacio(startPart + tamanoParticion, mbrTemp.mbr_tamano))
            {
                //dando valores a la particion
                particion.TipoParticion = 'E';
                particion.Status_particion = '1';
                particion.Inicio_particion = startPart;
                particion.TamanioTotal = tamanoParticion;
                strcpy(particion.NombreParticion, disco->name.c_str());
                //particion creada
                for (int i = 0; i < 4; i++)
                {
                    if (parts[i].Status_particion == '0' && flagExt)
                    {
                        parts[i] = particion;
                        break;
                    }
                }
                if (flagExt)
                {
                    fseek(arch, 0, SEEK_SET); //modificar en donde esta el mbr original
                    for (int i = 0; i < 4; i++)
                    {
                        mbrTemp.particiones[i] = parts[i];
                    }
                    fwrite(&mbrTemp, sizeof(MBR), 1, arch);
                    //agregar el EBR
                    EBR auxiliar;
                    auxiliar.TipoAjuste = particion.TipoAjuste;
                    strcpy(auxiliar.NombreParticion, disco->name.c_str());
                    auxiliar.part_next = -1;
                    auxiliar.TamanioTotal = tamanoParticion;
                    auxiliar.Inicio_particion = startPart;
                    auxiliar.Status_particion = '1';

                    fseek(arch, auxiliar.Inicio_particion, SEEK_SET); //se posiciona al inicio de la particion para meter el EBR
                    fwrite(&auxiliar, sizeof(EBR), 1, arch);
                    fclose(arch);
                    imprimirDatosDisco(disco->path);
                }
            }
        }
    }
    else if (disco->type == "L" || disco->type == "l")
    {
        printf("Particion Logica\n");
        //extensiones logicas
        Particion parts[4];
        bool flagExt = false;
        for (int i = 0; i < 4; i++)
        {
            parts[i] = mbrTemp.particiones[i];
            if (parts[i].TipoParticion == 'E')
            {
                flagExt = true;
            }
        }
        if (!flagExt)
        {
            printf("No existe una particion Extendida\n");
        }
        else
        {
            EBR auxiliar;
            auxiliar.TipoAjuste = particion.TipoAjuste;
            strcpy(auxiliar.NombreParticion, disco->name.c_str());
            auxiliar.part_next = 0;
            auxiliar.TamanioTotal = tamanoParticion;
            auxiliar.Inicio_particion = startPart;
            auxiliar.Status_particion = '1';
            inicioParticionLogica(disco->path, auxiliar);
        }
    }
    else
    {
        //default
        printf("Ocurrio un Error\n");
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
    return false;
}

void fdisk::dirExist(fdisk *disco)
{
    printf("------------------------------Ejecutar comando FDISK------------------------------\n");

    //verificar que el directorio existe
    struct stat st;
    if (stat(disco->path.c_str(), &st) == 0)
    {
        printf("Existe el archivo en: %s\n", disco->path.c_str());
        if (disco->borrar != "")
        {
            eliminarParticion(disco->path, disco->name, disco->borrar);
        }
        else
        {
            ejecutarComandoFdisk(disco);
        }
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

void fdisk::eliminarParticion(string path, string nombre, string tipoDelete)
{
    char name2[15];
    char name1[15];
    int result;
    bool Entrada = false;
    strcpy(name2, nombre.c_str());
    FILE *arch;
    arch = fopen(path.c_str(), "rb+"); //se abre el archivo del disco en modo lectura

    if (arch == NULL)
    {
        std::cout << "Error! El disco no existe\n"; //si da null es porque no se encontro el archivo
        return;
    }
    MBR mbrTemp;
    fseek(arch, 0, SEEK_SET);
    fread(&mbrTemp, sizeof(MBR), 1, arch);
    Particion parts[4];
    for (int i = 0; i < 4; i++)
    {
        parts[i] = mbrTemp.particiones[i];
    }
    for (int i = 0; i < 4; i++)
    {
        strcpy(name1, parts[i].NombreParticion);
        result = strcmp(name1, name2);
        if (parts[i].TipoParticion == 'E' && result == 0)
        {
            printf("Es una Extendida \n");
            Entrada = true;
        }
        else if (parts[i].TipoParticion == 'P' && result == 0)
        {
            Particion partTemp;
            partTemp.Status_particion = '0';
            partTemp.TipoParticion = '\0';
            partTemp.TipoAjuste = '\0';
            partTemp.Inicio_particion = 0;
            partTemp.TamanioTotal = 0;
            partTemp.NombreParticion[0] = ' ';
            parts[i] = partTemp;
            for (int j = 0; j < 4; j++)
            {
                mbrTemp.particiones[j] = parts[j];
            }
            fseek(arch, 0, SEEK_SET);
            fwrite(&mbrTemp, sizeof(EBR), 1, arch);
            printf("Particion Primaria Eliminada \n");
            imprimirDatosDisco(path);

            Entrada = true;
        }
    }
    if (Entrada == false)
    {
        for (int i = 0; i < 4; i++)
        {
            if (parts[i].TipoParticion == 'E')
            {
                int inicioExtendida = parts[i].Inicio_particion;
                fseek(arch, inicioExtendida, SEEK_SET);
                EBR ebrAnterior;
                EBR ebr;
                ebrAnterior = ebr;
                fread(&ebr, sizeof(EBR), 1, arch);
                if (ebr.part_next == -1)
                {
                    printf("No hay particiones Logicas \n");
                }
                else
                {
                    fseek(arch, inicioExtendida, SEEK_SET);
                    fread(&ebr, sizeof(EBR), 1, arch);
                    while (true)
                    {
                        strcpy(name1, ebr.NombreParticion);
                        result = strcmp(name1, name2);
                        if (result == 0)
                        {
                            printf("Particion Logica Encontrada \n");
                            string del = tipoDelete;
                            std::for_each(del.begin(), del.end(), [](char &c)
                                          { c = ::tolower(c); });
                            if (del == "fast")
                            {
                                ebrAnterior.part_next = ebr.part_next;
                                fseek(arch, ebrAnterior.Inicio_particion, SEEK_SET);
                                fwrite(&ebrAnterior, sizeof(EBR), 1, arch);
                            }
                            else if (del == "full")
                            {
                                ebrAnterior.part_next = ebr.part_next;
                                fseek(arch, ebrAnterior.Inicio_particion, SEEK_SET);
                                fwrite(&ebrAnterior, sizeof(EBR), 1, arch);
                            }
                            Entrada = true;
                        }
                        if (ebr.part_next == -1)
                        {
                            break;
                        }
                        else
                        {
                            fseek(arch, ebr.part_next, SEEK_SET);
                            ebrAnterior = ebr;
                            fread(&ebr, sizeof(EBR), 1, arch);
                        }
                    }
                }
            }
        }
    }
    if(Entrada== false){
        printf("Error no se encontro la particion \n");
    }
    fclose(arch);
}

bool fdisk::inicioParticionLogica(string path, EBR ebrTemp)
{
    //abre el archivo
    MBR mbrTemp;
    FILE *arch;
    arch = fopen(path.c_str(), "rb+"); //se abre el archivo del disco en modo lectura

    if (arch == NULL)
    {
        std::cout << "Error! El disco no existe\n"; //si da null es porque no se encontro el archivo
        return;
    }
    fseek(arch, 0, SEEK_SET);              //se posiciona en el bite 0 del archivo
    fread(&mbrTemp, sizeof(MBR), 1, arch); //mbrTemp se guardara, sizeof es el tamaño en bites del mbr que se lee, 1 es la cantidad de elementos y arch el archivo abierto

    Particion parts[4];
    for (int i = 0; i < 4; i++)
    {
        parts[i] = mbrTemp.particiones[i];
    }
    for (int i = 0; i < 4; i++)
    {
        if (parts[i].TipoParticion == 'E')
        {
            int inicioExtendida = parts[i].Inicio_particion;
            fseek(arch, inicioExtendida, SEEK_SET);
            EBR ebr2;
            fread(&ebr2, sizeof(EBR), 1, arch);
            if (ebr2.part_next == -1)
            {
                ebr2.part_next = ebr2.Inicio_particion + sizeof(EBR) + ebrTemp.TamanioTotal;
                fseek(arch, inicioExtendida, SEEK_SET);
                fwrite(&ebr2, sizeof(EBR), 1, arch);
                ebrTemp.Inicio_particion = ebr2.part_next;
                ebrTemp.part_next = -1;
                fseek(arch, ebrTemp.Inicio_particion, SEEK_SET);
                fwrite(&ebrTemp, sizeof(EBR), 1, arch);

                fseek(arch, inicioExtendida, SEEK_SET);
                fread(&ebr2, sizeof(EBR), 1, arch);
                cout << ebr2.Inicio_particion << endl;
                cout << ebr2.part_next << endl;
                fclose(arch);
                return false;
            }
            else
            {
                printf("Inicio Particion n \n");
                fseek(arch, inicioExtendida, SEEK_SET);
                fwrite(&ebr2, sizeof(EBR), 1, arch);
                while (true)
                {
                    if (ebr2.part_next == -1)
                    {
                        printf("Es la ultima logica \n");
                        ebr2.part_next = ebr2.Inicio_particion + sizeof(EBR) + ebr2.TamanioTotal;
                        fseek(arch, ebr2.Inicio_particion, SEEK_SET);
                        fwrite(&ebr2, sizeof(EBR), 1, arch);
                        ebrTemp.Inicio_particion = ebr2.part_next;
                        ebrTemp.part_next = -1;
                        fseek(arch, ebrTemp.Inicio_particion, SEEK_SET);
                        fwrite(&ebrTemp, sizeof(EBR), 1, arch);
                        cout << "NombreLogica: ";
                        cout << ebrTemp.NombreParticion << endl;
                        break;
                    }
                    else
                    {
                        fseek(arch, ebr2.part_next, SEEK_SET);
                        fwrite(&ebr2, sizeof(EBR), 1, arch);
                        cout << "NombreLogica: ";
                        cout << ebr2.NombreParticion << endl;
                    }
                }

                fclose(arch);
                return false;
            }
        }
    }
    fclose(arch);
    return false;
}
