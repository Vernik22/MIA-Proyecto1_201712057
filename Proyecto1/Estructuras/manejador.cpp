#include "manejador.h"
#include "estructuras.h"
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
#include "mkdisk.h"
#include "mkdisk.cpp"

using namespace std;
/* ---------------------------------------------------------------------- */
manejador::manejador()
{
}
//lee y reconoce los comandos

void manejador::leerTexto(string data, vector<int> &listaDiscos)
{
    printf("------------------------------Leer Comando------------------------------\n");
    vector<Comando> listaComandos;
    vector<string> lineasComando = split(data, '\n');
    Comando c;

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

            for (int i = 1; i < propiedades.size(); i++)
            {
                if (propiedades[i] == "")
                {
                    continue;
                }
                else if (propiedades[i] == "-p") //no se para que es
                {
                    propiedadesTem[i - 1].Name = "-p";
                    propiedadesTem[i - 1].val = "-p";
                }
                else
                {
                    if (strstr(propiedades[i].c_str(), "=") != NULL)
                    {
                        vector<string> valor_propiedad_Comando = split(propiedades[i], '=');
                        propiedadesTem[i - 1].Name = valor_propiedad_Comando[0];
                        propiedadesTem[i - 1].val = valor_propiedad_Comando[1];
                    }
                    else
                    {
                        propiedadesTem[i - 1].Name = "-sigue";
                        propiedadesTem[i - 1].val = propiedades[i];
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

void manejador::listaComandosValidos(vector<Comando> listaComandos)
{
    bool ParametroValido = true;
    int cont = 1;

    for (int i = 0; i < listaComandos.size(); i++)
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

            mkdisk *disco = new mkdisk();
            bool parametrosValidos = disco->ejecMkdisk(nombreComando, comandoTemp.propiedades, cont);
            if (parametrosValidos == false)
            {
                printf("--- Parametros Invalidos ---\n");
            }
        }
        else if (nombreComando == "rmdisk")
        {
            printf("---RMDISK---\n");
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