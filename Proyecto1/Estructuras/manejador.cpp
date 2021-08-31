#include "manejador.h"
#include "estructuras.h"
#include "mkdisk.h"
#include "mkdisk.cpp"
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
#include "rmdisk.h"
#include "rmdisk.cpp"
#include "fdisk.h"
#include "fdisk.cpp"
#include "mount.h"
#include "mount.cpp"

using namespace std;
/* ---------------------------------------------------------------------- */
manejador::manejador()
{
}
//lee y reconoce los comandos
//vector<int> &listaDiscos lista discos como parametro
void manejador::leerTexto(string data )
{
    printf("------------------------------Leer Comando------------------------------\n");
    vector<Comando> listaComandos;
    vector<string> lineasComando = split(data, '\n');
    Comando c;
    int sizeVec1 = lineasComando.size();
    for (int i = 0; i < lineasComando.size(); i++)
    {
        string esComentario = lineasComando[i];
        if (strstr(esComentario.c_str(), "#") == NULL)
        {
            string comando = lineasComando[i];
            //verificar los comentarios

            vector<string> propiedades = split(comando, ' ');
            //nombre del comando
            string nombreComando = propiedades[0];
            //struct para el comando
            std::for_each(nombreComando.begin(), nombreComando.end(), [](char &c)
                          { c = ::tolower(c); }); //nombre comando a lower case

            c.Name = nombreComando;
            Propiedad propiedadesTem[10];
            int sizeVec = propiedades.size();
            for (int j = 1; j < sizeVec; j++)
            {
                if (propiedades[j] == "")
                {
                    continue;
                }
                else if (propiedades[j] == "-p") //no se para que es
                {
                    propiedadesTem[j - 1].Name = "-p";
                    propiedadesTem[j - 1].val = "-p";
                }
                else
                {
                    if (strstr(propiedades[j].c_str(), "=") != NULL)
                    {
                        vector<string> valor_propiedad_Comando = split(propiedades[j], '=');
                        propiedadesTem[j - 1].Name = valor_propiedad_Comando[0];
                        propiedadesTem[j - 1].val = valor_propiedad_Comando[1];
                    }
                    else
                    {
                        propiedadesTem[j - 1].Name = "-sigue";
                        propiedadesTem[j - 1].val = propiedades[i];
                    }
                }
            }
            for (int f = 0; f < 10; f++)
            {
                c.propiedades[f] = propiedadesTem[f];
            }
            //agregando el comando a la lista de comandos
            listaComandos.push_back(c);
        }
    }
    listaComandosValidos(listaComandos);
}

void manejador::listaComandosValidos(vector<Comando> &listaComandos)
{
  //  bool ParametroValido = true;
    int cont = 1;
    int sizeVec = listaComandos.size();
    for (int i = 0; i < sizeVec; i++)
    {
        Comando comandoTemp;
        comandoTemp = listaComandos[i];
        string nombreComando = comandoTemp.Name;

        std::for_each(nombreComando.begin(), nombreComando.end(), [](char &c)
                      { c = ::tolower(c); });

        //cout << nombreComando << endl;

        //switch improvisado que lee todos los comandos
        if (nombreComando == "mkdisk")
        {

            mkdisk *jdisco = new mkdisk();
            bool parametrosValidos = jdisco->ejecMkdisk(nombreComando, comandoTemp.propiedades, cont);
            if (parametrosValidos == false)
            {
                printf("--- Parametros Invalidos ---\n");
            }
        }
        else if (nombreComando == "rmdisk")
        {
            rmdisk *discoaRem = new rmdisk();
            discoaRem->ejecRmdisk(nombreComando, comandoTemp.propiedades, cont);
        }
        else if (nombreComando == "fdisk")
        {
            fdisk *discoF = new fdisk();
            discoF->ejecFdisk(nombreComando,comandoTemp.propiedades,cont);
            
        }else if (nombreComando == "mount")
        {
           // mount *discoM = new mount();
            //discoM->ejecMount();
            //discoM->ejecutarReporteMount();
        }else if (nombreComando == "umount")
        {
            //umount *discoU = new umount();
            //discoU->ejecUmount();
        }
        
        
    }
}
vector<string> manejador::split(string str, char pattern)
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