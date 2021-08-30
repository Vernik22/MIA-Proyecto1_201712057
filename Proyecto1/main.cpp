#include <iostream>

#include "Estructuras/manejador.h"
#include "Estructuras/manejador.cpp"
#include <vector>
#include "Estructuras/estructuras.h"

using namespace std;
/*
vector<int> llenarListaDiscos(vector<int> &listaDiscos)
{
    vector<int> results;
    cout << "Pasa por llenardiscos" << endl;

    return results;
}
*/

int main()
{

    vector<int> listaDiscos;
    //listaDiscos = llenarListaDiscos(listaDiscos);
    string p = "\n\n------------------------------Ingrese un comando------------------------------\n";
    string comando = "";

   // mkdisk *dico = new mkdisk();
    manejador *leer = new manejador();

    while (comando != "salir")
    { //esto me sirve para seguir leyendo siempre los comandos sin salirme
        cout << p;
        comando = "";

        getline(cin, comando); // espera el comando y lo asigna a la variable comando

        if (comando != "" && comando != "salir")
        { 
            leer->leerTexto(comando,listaDiscos);

            //estas son pruebas
           /*
            
            dico->path="/home/vernik/Escritorio/Discos/intento.dk";
            //dico->path = "/home/vernik/Escritorio/mis discos/Disco3.dk";
            dico->unit="m";
            dico->size=100;
            dico->ejecutarComandoMkdisk(dico);
            printf("Disco creado en %s con tamaño %i\n\n  ",dico->path.c_str(),dico->size);
            
            */
        }
    }

    return 0;
}


