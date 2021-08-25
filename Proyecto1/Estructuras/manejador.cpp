#include "manejador.h"
#include "estructuras.h"
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>

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
            Propiedad propiedadesTem[propiedades.size()-1];

            for(int i = 1; i< propiedades.size()-1;i++){
                if (propiedades[i]=="")
                {
                    /* code */
                }
                

            }
            listaComandos.push_back(c);
        }
    }
    listaComandosValidos(listaComandos);
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

        cout << nombreComando << endl;

        //switch improvisado que lee todos los comandos
        if (nombreComando == "mkdisk")
        {
            printf("---MKDISK---\n");
        }else if (nombreComando== "rmdisk")
        {
            printf("---RMDISK---\n");
        }
        
    }
}