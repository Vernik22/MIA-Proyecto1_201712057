#include "mount.h"
#include <algorithm>
#include <string.h>
#include <vector>
#include <sys/stat.h>
#include "estructuras.h"

using namespace std;

mount::mount()
{
}

bool mount::ejecMount(string nombreComando, Propiedad propiedades[],vector<DISCO> &listaDiscos)
{
    try
    {
        mount *disco = new mount();
        bool paramValid = true;
        bool flagPath = false;
        bool flagName = false;
        for (int i = 0; i < 10; i++)
        {
            Propiedad propiedadTemp = propiedades[i];
            string nombrePropiedad = propiedadTemp.Name;
            std::for_each(nombrePropiedad.begin(), nombrePropiedad.end(), [](char &c)
                          { c = ::tolower(c); });

            //switch para los parametros
            if (nombrePropiedad == "-path")
            {
                disco->path = propiedadTemp.val;
                flagPath = true;
            }
            else if (nombrePropiedad == "-name")
            {
                disco->name = propiedadTemp.val;
                flagName = true;
            }
        }

        if (flagPath && flagName)
        {
            dirExist(disco, listaDiscos);
            return paramValid;
        }
        else
        {
            paramValid = false;
            return paramValid;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return false;
}

void mount::dirExist(mount *disco, vector<DISCO> &listaDiscos)
{
    printf("------------------------------Ejecutar comando MOUNT------------------------------\n");
    //verificar que el directorio existe
    vector<string> resultados;
    resultados = split(disco->path, '/');
    string pathconc = "";
    for (int i = 1; i < resultados.size(); i++)
    {                                             //llenar el string con el path sin el disk.dk
        pathconc += "/\"" + resultados[i] + "\""; //le agrego comillas a los nombres por si vienen con espacios por ejemplo "mis discos"
    }
    struct stat st;
    if (stat(disco->path.c_str(), &st) == 0)
    {
        printf("Existe el archivo en: %s\n", disco->path.c_str());
        //ejecutar comando mount
        ejecutarComandoMount(disco, listaDiscos);
    }
    else
    {
        printf("No Existe el archivo: %s\n", disco->path.c_str());
    }
}

vector<string> mount::split(string str, char pattern)
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

bool mount::ejecutarComandoMount(mount *disco, vector<DISCO> &listaDiscos)
{
    bool encontrada = false;
    vector<string> lineaComando = split(disco->path, '/');
    int pos = lineaComando.size();
    string nombreDisco = lineaComando[pos - 1];

    FILE *arch;
    arch = fopen(disco->path.c_str(), "wb");
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
        string nombre = parts[i].NombreParticion;
        if (nombre == disco->name)
        {
            encontrada = true;
            if (parts[i].TipoParticion == 'E')
            {
                printf("Error no se puede Montar una particion Extendida \n");
            }
            else
            {
                particionMontar(listaDiscos, disco->name, nombreDisco, disco->path);
            }
        }
        if (parts[i].TipoParticion == 'E')
        {
            EBR ebrTemp;
            fseek(arch, parts[i].Inicio_particion, SEEK_SET);
            fread(&ebrTemp, sizeof(EBR), 1, arch);
            while (true)
            {
                if (ebrTemp.part_next == -1)
                {
                    break;
                }
                else
                {
                    fseek(arch, ebrTemp.part_next, SEEK_SET);
                    fread(&ebrTemp, sizeof(EBR), 1, arch);
                }
                string nombreP = ebrTemp.NombreParticion;
                if (nombreP == disco->name)
                {
                    encontrada = true;
                    particionMontar(listaDiscos, disco->name, nombreDisco, disco->path);
                }
            }
        }
    }
    if (encontrada == false)
    {
        printf("Error no se encontro la particion \n");
    }

    fclose(arch);

    return true;
}

void mount::particionMontar(vector<DISCO> &listaDiscos, string nombreParticion, string nombreDisco, string path)
{
    int pos = listaDiscos.size();
    for (int i = 0; i < pos; i++)
    {
        DISCO dis;
        dis = listaDiscos[i];
        char es = dis.estado;
        if (dis.estado == '0' && !existeDisco(listaDiscos, nombreDisco))
        {
            dis.NombreDisco = nombreDisco;
            dis.Path = path;
            dis.estado = '1';
            for (int j = 0; j < 100; j++)
            {
                MOUNT mountTemp = dis.Particiones[j];
                if (mountTemp.estado == '0')
                {
                    mountTemp.id = "57" + dis.id + mountTemp.id;
                    mountTemp.NombreParticion = nombreParticion;
                    mountTemp.estado = '1';
                    mountTemp.EstadoMKS = '0';
                    dis.Particiones[j] = mountTemp;
                    break;
                }
                else if (mountTemp.estado == '1' && mountTemp.NombreParticion == nombreParticion)
                {
                    printf("La particion ya esta montada \n");
                    break;
                }
            }
            listaDiscos[i] = dis;
            break;
        }
        else if (dis.estado == '1' && existeDisco(listaDiscos, nombreDisco) && nombreDisco == dis.NombreDisco)
        {
            printf("Otra particion montada en el disco ");
            cout << dis.id << endl;
            for (int j = 0; j < 100; j++)
            {
                MOUNT mountTemp = dis.Particiones[j];
                if (mountTemp.estado == '0')
                {
                    mountTemp.id = "57" + dis.id + mountTemp.id;
                    mountTemp.NombreParticion = nombreParticion;
                    mountTemp.estado = '1';
                    mountTemp.EstadoMKS = '0';
                    dis.Particiones[j] = mountTemp;
                    break;
                }
                else if (mountTemp.estado == '1' && mountTemp.NombreParticion == nombreParticion)
                {
                    printf("La particion ya esta montada \n");
                    break;
                }
            }
            listaDiscos[i] = dis;
            break;
        }
    }
}

bool mount::existeDisco(vector<DISCO> &listaDiscos, string nombreDisco)
{
    bool existe = false;
    int pos = listaDiscos.size();
    for (int i = 0; i < pos; i++)
    {
        DISCO dis;
        dis = listaDiscos[i];
        if (dis.NombreDisco == nombreDisco)
        {
            return true;
        }
        else
        {
            existe = false;
        }
    }
    return existe;
}

void mount::ejecutarReporteMount(vector<DISCO> &listaDiscos)
{
    printf("------------------------------MOUNT------------------------------\n");
    int pos = listaDiscos.size();
    for (int i = 0; i < pos; i++)
    {
        DISCO dis;
        dis = listaDiscos[i];
        if (dis.NombreDisco != "")
        {
            for (int j = 0; j < 100; j++)
            {
                MOUNT mountTemp = dis.Particiones[j];
                if (mountTemp.NombreParticion != "")
                {
                    cout << dis.Path + "|";
                    cout << mountTemp.NombreParticion + "|";
                    cout << mountTemp.id << endl;
                }
            }
        }
    }
}