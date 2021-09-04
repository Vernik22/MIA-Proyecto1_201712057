#include "manejador.h"
#include "estructuras.h"
#include "mkdisk.h"
#include "mkdisk.cpp"
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstring>
#include "rmdisk.h"
#include "rmdisk.cpp"
#include "fdisk.h"
#include "fdisk.cpp"
#include "mount.h"
#include "mount.cpp"
#include "umount.h"
#include "umount.cpp"
#include "mkfs.h"
#include "mkfs.cpp"
//#include "rep.h"
//#include "rep.cpp"
#include "exec.h"
#include "exec.cpp"

using namespace std;
/* ---------------------------------------------------------------------- */
manejador::manejador()
{
}
//lee y reconoce los comandos
//vector<int> &listaDiscos lista discos como parametro
void manejador::leerTexto(string data, vector<DISCO> &listaDiscos)
{
    printf(" \n");
    cout << endl
         << data << endl;
    vector<Comando> listaComandos;
    vector<string> lineasComando = split(data, '\n');
    Comando c;
    int sizeVec1 = lineasComando.size();
    for (int i = 0; i < sizeVec1; i++)
    {
        bool flagCOM = false;
        vector<string> com;
        string esComentario = lineasComando[i];
        if (strstr(esComentario.c_str(), "#") != NULL)
        {
            com = split(data, '#');
            flagCOM = true;
        }

        if (esComentario[0] != '#')
        {
            string comando = lineasComando[i];
            //verificar los comentarios
            if (flagCOM)
            {
                comando = com[0];
            }
            

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
        else
        {
            cout << "Es comentario" << endl;
        }
    }
    listaComandosValidos(listaComandos, listaDiscos);
}

void manejador::listaComandosValidos(vector<Comando> &listaComandos, vector<DISCO> &listaDiscos)
{
    int lis = listaComandos.size();
    if (lis != 0)
    {
        //printf("------------------------------Leer Comando------------------------------\n");
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
                discoF->ejecFdisk(nombreComando, comandoTemp.propiedades, cont);
            }
            else if (nombreComando == "mount")
            {
                mount *discoM = new mount();
                discoM->ejecMount(nombreComando, comandoTemp.propiedades, listaDiscos);
                discoM->ejecutarReporteMount(listaDiscos);
            }
            else if (nombreComando == "umount")
            {
                umount *discoU = new umount();
                discoU->ejecUmount(nombreComando, comandoTemp.propiedades, listaDiscos);
                mount *discoM = new mount();
                discoM->ejecutarReporteMount(listaDiscos);
            }
            else if (nombreComando == "mkfs")
            {
                mkfs *discoM = new mkfs();
                discoM->ejecMkfs(nombreComando, comandoTemp.propiedades, listaDiscos);
            }
            else if (nombreComando == "exec")
            {
                exec *discoE = new exec();
                discoE->ejecExec(comandoTemp.propiedades, listaDiscos);
            }
            else if (nombreComando == "rep")
            {
                //rep *discoR = new rep();
                //discoR->ejecRep(nombreComando, comandoTemp.propiedades, listaDiscos);
            }
            else if (nombreComando == "pause")
            {

                printf("Pause Presione una tecla para continuar... \n");
                cin.get();
            }
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

