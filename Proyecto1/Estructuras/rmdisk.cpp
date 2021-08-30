#include "rmdisk.h"
#include <string.h>
#include <sys/stat.h>
#include <vector>
#include <algorithm>

rmdisk::rmdisk()
{
}

void rmdisk::dirExist(rmdisk *disco)
{
    printf("------------------------------Ejecutar comando RMDISK------------------------------\n");

    //verificar que el directorio existe
    vector<string> resultados;
    resultados = split(disco->path, '/');
    string pathconc = "";
    for (int i = 1; i < resultados.size(); i++)
    {                                             //llenar el string con el path sin el disk.dk
        pathconc += "/\"" + resultados[i] + "\""; //le agrego comillas a los nombres por si vienen con espacios por ejemplo "mis discos"
    }
    //verificar 3 veces si el archivo existe, si el path existe, y enviar el path con comillas

    struct stat st;
    if (stat(disco->path.c_str(), &st) == 0)
    {
        printf("Existe el archivo en: %s\n", disco->path.c_str());
        string comando = "rm " + pathconc;
        system(comando.c_str());
        printf("Se borro el archivo en: %s \n", disco->path.c_str());
    }
    else
    {
        printf("No Existe el archivo: %s\n", disco->path.c_str());
    }
}

vector<string> rmdisk::split(string str, char pattern)
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

bool rmdisk::ejecRmdisk(string nombreComando, Propiedad propiedades[], int cont)
{

    try
    {
        rmdisk *disco = new rmdisk();
        bool paramValid = true;
        for (int i = 0; i < 10; i++) // recorrer los parametros y armando la clase disco
        {
            Propiedad propiedadTemp = propiedades[i];
            string nombrePropiedad = propiedadTemp.Name;
            std::for_each(nombrePropiedad.begin(), nombrePropiedad.end(), [](char &c)
                          { c = ::tolower(c); });

            //switch con los parametros

            if (nombrePropiedad == "-path")
            {
                disco->path=propiedadTemp.val;
            }
        }

        dirExist(disco);
        return paramValid;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return false;
}