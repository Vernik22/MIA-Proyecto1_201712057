#include "mkdisk.h"
#include "estructuras.h"
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>

#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

mkdisk::mkdisk()
{
}

void mkdisk::ejecutarComandoMkdisk(mkdisk *disco)
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
    //cout << newpath << endl;
    dirExist(newpath, pathconc); //verifica si la path existe, si no existe la crea

    MBR mbrDisco;
    FILE *arch;
    arch = fopen(disco->path.c_str(), "wb");
    if (arch == NULL)
        exit(1);
    //se sibula un KB
    char buffer[1024];
    if (disco->unit == "k")
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
    else if (disco->unit == "m" || disco->unit.empty() == 1)
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

void mkdisk::ejecutarComando(string comandos)
{
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

void mkdisk::dirExist(string path, string pathconc)
{
    struct stat st;
    if (stat(path.c_str(), &st) == 0)
    {
        printf("Existe el directorio\n");
    }

    else
    {
        printf("No Existe el directorio: %s\n", path.c_str());
        string comando = "mkdir " + pathconc;
        system(comando.c_str());
        printf("Se creo el dir en: %s \n", path.c_str());
    }
}

bool mkdisk::ejecMkdisk(string nombreComando, Propiedad propiedades[], int cont)
{
    printf("---MKDISK---\n");
    mkdisk *df = new mkdisk();
    try
    {
        bool paramValid = true;
        bool flagFit = true; //bandera para este parametro que es opcional
        bool flagUnit = true; //bandera para este parametro que es opcional
        for (int i = 0; i < 10; i++)
        {
            Propiedad propiedadTemp = propiedades[i];
            string nombrePropiedad = propiedadTemp.Name;
            std::for_each(nombrePropiedad.begin(), nombrePropiedad.end(), [](char &c)
                          { c = ::tolower(c); });

            if (nombrePropiedad == "-size")
            {
                /* code */
            }
            else if (nombrePropiedad == "-f")
            {
                flagFit= false;
            }
            else if (nombrePropiedad == "-u")
            {
                flagUnit = false;
            }
            else if (nombrePropiedad == "-path")
            {
                /* code */
            }
        }

        if (flagFit) // si el parametro no venia entra aqui y se pone el parametro por defecto
        {
            df->ajuste = "ff";
        }else if (flagUnit) // si el parametro no venia entra aqui y se pone el parametro por defecto
        {
            df->unit = "m";
        }
        
        

        return paramValid;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
