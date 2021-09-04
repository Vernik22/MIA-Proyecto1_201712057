#include "mkdisk.h"
#include "estructuras.h"
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

mkdisk::mkdisk()
{
}

void mkdisk::ejecutarComandoMkdisk(mkdisk *disco)
{

    MBR mbrDisco;
    FILE *arch;
    arch = fopen(disco->path.c_str(), "wb");
    if (arch == NULL)
        exit(1);
    //se sibula un KB
    char buffer[1024];
    if (disco->unit == "k" || disco->unit == "K")
    {
        //se asigna el tamaño a mbr
        mbrDisco.mbr_tamano = disco->size * 1024;
        //lleno la variable buffer de ceros para que no este en null;
        for (int i = 0; i < 1024; i++)
        {
            buffer[i] = '\0';
        }

        for (int i = 0; i < (disco->size); i++)
        {
            fwrite(&buffer, 1024, 1, arch);
        }
        fclose(arch);
    }
    //si son megas
    else if (disco->unit == "m" || disco->unit == "M")
    {
        mbrDisco.mbr_tamano = disco->size * 1024 * 1024; //se multiplica 2 veces por que esta en bytes
        for (int i = 0; i < 1024; i++)
        {
            buffer[i] = '\0';
        }
        for (int i = 0; i < (disco->size * 1024); i++)
        {
            fwrite(&buffer, 1024, 1, arch);
        }
        fclose(arch);
    }

    //etiqueta unica al disco

    //hora actual
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    //printf("Current local time and date: %s", asctime(timeinfo));
    string fecha = asctime(timeinfo);

    disco->fechaCreachion = fecha;

    //dar datos al mbr
    strcpy(mbrDisco.mbr_fecha_creacion, fecha.c_str());
    mbrDisco.No_identificacion = (rand() % 100); //numero random para darle etiqueta al disco

    //si el fit viene bacio se pone el primer ajuste
    if (disco->ajuste.empty() == true)
    {
        strcpy(mbrDisco.disk_ajuste, "F"); // se convierte a char * para guardarlo en el MBR
    }
    else
    {
        strcpy(mbrDisco.disk_ajuste, disco->ajuste.c_str()); // se convierte a char * para guardarlo en el MBR
    }

    printf("\nFecha de Creacion : %s\n", mbrDisco.mbr_fecha_creacion);

    Particion vacia;
    vacia.Status_particion = '0'; //status inactivo
    vacia.TipoParticion = '-';
    vacia.TipoAjuste = '-';
    vacia.Inicio_particion = -1;
    vacia.TamanioTotal = -1;
    vacia.NombreParticion[0] = '\0';
    for (int i = 0; i < 4; i++)
    {
        mbrDisco.particiones[i] = vacia;
    }
    //agregar el mbr al disco
    arch = fopen(disco->path.c_str(), "rb+"); //modo de escritura mixto permite actualizar un fichero sin borrar elcontenido anterior
    if (arch != NULL)
    {
        fseek(arch, 0, SEEK_SET);
        fwrite(&mbrDisco, sizeof(MBR), 1, arch);
        fclose(arch);
        printf("Disco creado correctamente \n");
    }
    else
    {
        printf("Error, No se puede acceder al disco, MBR no creado");
    }
}


vector<string> mkdisk::split(string str, char pattern)
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

void mkdisk::dirExist(mkdisk *disco)
{
     printf("------------------------------Ejecutar comando MKDISK------------------------------\n");

    //verificar que el directorio existe
    vector<string> resultados;
    resultados = split(disco->path, '/');
    string newpath = "";
    string pathconc = "";
    for (int i = 1; i < resultados.size() - 1; i++)
    {                                             //llenar el string con el path sin el disk.dk
        pathconc += "/\"" + resultados[i] + "\""; //le agrego comillas a los nombres por si vienen con espacios por ejemplo "mis discos"
        newpath += "/" + resultados[i];           //este es sin comillas para buscar el directorio no importa si vienen con espacios
    }
    //verificar 3 veces si el archivo existe, si el path existe, y enviar el path con comillas

    struct stat st;
    if (stat(disco->path.c_str(), &st) == 0)
    {
        printf("Existe el archivo en: %s\n", disco->path.c_str());
    }else if (stat(newpath.c_str(), &st) == 0)
    {
        printf("Existe el directorio: %s\n",newpath.c_str());
        ejecutarComandoMkdisk(disco);
    }
    else
    {
        printf("No Existe el directorio: %s\n", disco->path.c_str());
        string comando = "mkdir -p " + newpath;
        system(comando.c_str());
        printf("Se creo el dir en: %s \n", disco->path.c_str());
        ejecutarComandoMkdisk(disco);
    }
}

bool mkdisk::ejecMkdisk(string nombreComando, Propiedad propiedades[], int cont)
{

    try
    {
        mkdisk *d = new mkdisk();
        //printf("---MKDISK---\n");
        bool paramValid = true;
        bool flagFit = true;  //bandera para este parametro que es opcional
        bool flagUnit = true; //bandera para este parametro que es opcional
        for (int i = 0; i < 10; i++)
        {
            Propiedad propiedadTemp = propiedades[i];
            string nombrePropiedad = propiedadTemp.Name;
            std::for_each(nombrePropiedad.begin(), nombrePropiedad.end(), [](char &c)
                          { c = ::tolower(c); });

            if (nombrePropiedad == "-size")
            {
                //printf("---entra en size---%s\n", propiedadTemp.val.c_str());
                int s = stoi(propiedadTemp.val);
                d->size=s;
            }
            else if (nombrePropiedad == "-f")
            {
                flagFit = false;
                d->ajuste=propiedadTemp.val;
            }
            else if (nombrePropiedad == "-u")
            {
                flagUnit = false;
                d->unit=propiedadTemp.val;
            }
            else if (nombrePropiedad == "-path")
            {
                d->path=propiedadTemp.val;
            }else if(nombrePropiedad!=""){
                paramValid=false;
            }
        }

        if (flagFit) // si el parametro no venia entra aqui y se pone el parametro por defecto
        {
            d->ajuste = "ff";
        }
        if (flagUnit) // si el parametro no venia entra aqui y se pone el parametro por defecto
        {
            d->unit = "m";
        }

        
        if (paramValid)
        {
            dirExist(d);
        return paramValid;
        }else{
            return paramValid;
        }
        
        
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return false ; 
}
