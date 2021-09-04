#include "umount.h"
#include "estructuras.h"
#include <string.h>
#include <vector>
#include <algorithm>
#include <sys/stat.h>

using namespace std;

umount::umount()
{
}

bool umount::ejecUmount(string noombreComando, Propiedad propiedades[], vector<DISCO> &listaDiscos)
{

    try
    {
        umount *disco = new umount();
        bool paramValid = true;
        bool flagId = true;

        for (int i = 0; i < 10; i++)
        {
            Propiedad propiedadTemp = propiedades[i];
            string nombrePropiedad = propiedadTemp.Name;
            std::for_each(nombrePropiedad.begin(), nombrePropiedad.end(), [](char &c)
                          { c = ::tolower(c); });

            //switch para los parametros
            if (nombrePropiedad == "-id")
            {
                flagId = false;
                disco->id = propiedadTemp.val;
                break;
            }
        }
        if (flagId)
        {
            printf("Parametros invalidos, no hay id \n");
            paramValid = false;
            return paramValid;
        }
        else
        {
            ejecutarComandoUmount(disco, listaDiscos);
            return paramValid;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return false;
}

void umount::ejecutarComandoUmount(umount *disco, vector<DISCO> &listaDiscos)
{
    printf("------------------------------Ejecutar comando UMOUNT------------------------------\n");
    string Id = disco->id;
    bool eliminada = false;
    char noParticion = Id[3];
    char idDisco = Id[2];
    int pos = listaDiscos.size();
    for (int i = 0; i < pos; i++)
    {
        DISCO dis;
        dis = listaDiscos[i];
        cout << dis.id;
        cout << idDisco << endl;
        
        if (dis.id == idDisco)
        {
            for (int j = 0; j < 100; j++)
            {
                MOUNT mountTemp = dis.Particiones[j];
                if (mountTemp.id == disco->id)
                {
                    eliminada = true;
                    printf("Encontrada: %s \n", noParticion);
                    mountTemp.id = noParticion;
                    mountTemp.NombreParticion = "";
                    mountTemp.estado = '0';
                    mountTemp.EstadoMKS = '0';
                    dis.Particiones[j] = mountTemp;
                    printf("Particion Desmontada ");
                    cout << disco->id;
                    break;
                }
            }
        }
        listaDiscos[i] = dis;
    }
    if (eliminada == false)
    {
        printf("No se pudo desmontar la particion con id: %s \n", disco->id.c_str());
    }
}