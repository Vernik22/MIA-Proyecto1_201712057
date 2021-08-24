#include <iostream>

#include "Estructuras/manejador.h"
#include "Estructuras/manejador.cpp"
//#include "Estructuras/Lista/list.h"
//#include "Estructuras/Lista/list.cpp"
#include <vector>
#include "Estructuras/estructuras.h"
#include "Estructuras/mkdisk.h"
#include "Estructuras/mkdisk.cpp"

using namespace std;
//void llenarListaDiscos(List<int> &lista);

vector<int> llenarListaDiscos(vector<int> &listaDiscos)
{
    vector<int> results;
    cout << "Pasa por llenardiscos" << endl;

    return results;
}

int main()
{

    vector<int> listaDiscos;
    listaDiscos = llenarListaDiscos(listaDiscos);
    string p = "------------------------------Ingrese un comando------------------------------\n";
    string s = ">> ";
    string comando = "";
    mkdisk *dico = new mkdisk();
    manejador *leer = new manejador();

    while (comando != "salir")
    { //esto me sirve para seguir leyendo siempre los comandos sin salirme
        cout << p;
        cout << s;
        comando = "";

        getline(cin, comando); // espera el comando y lo asigna a la variable comando

        if (comando != "" && comando != "salir")
        { 
            leer->leerTexto(comando,listaDiscos);

            //estas son pruebas
            cout << "leyendo comando" << endl;
            dico->path="/home/vernik/Escritorio/Discos/intento.dk";
            //dico->path = "/home/vernik/Escritorio/mis discos/Disco3.dk";
            dico->unit="m";
            dico->size=100;
            dico->ejecutarComandoMkdisk(dico);
            printf("Disco creado en %s con tamaño %i\n\n  ",dico->path.c_str(),dico->size);
            
        }
    }

    return 0;
}


