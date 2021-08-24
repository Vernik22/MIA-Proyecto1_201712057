#include <iostream>

#include "Estructuras/manejador.h"
#include "Estructuras/Lista/list.h"
#include "Estructuras/Lista/list.cpp"
#include "Estructuras/estructuras.h"
#include "Estructuras/mkdisk.h"
#include "Estructuras/mkdisk.cpp"

using namespace std;
void llenarListaDiscos(List<int> &lista);

int main()
{

    List<int> listaDiscos;
    llenarListaDiscos(listaDiscos);
    string p = "------------------------------Ingrese un comando------------------------------\n";
    string s = ">> ";
    string comando = "";
    mkdisk *dico = new mkdisk();

    while (comando != "salir")
    { //esto me sirve para seguir leyendo siempre los comandos sin salirme
        cout << p;
        cout << s;
        comando = "";

        getline(cin, comando);

        if (comando != "" && comando != "salir")
        {
            cout << "leyendo comando" << endl;
            dico->path="/home/vernik/Escritorio/Discos/intento.dk";
            dico->unit="m";
            dico->size=100;
            dico->ejecutarComandoMkdisk(dico);
            printf("Disco creado en %s con tamaño %i\n\n  ",dico->path.c_str(),dico->size);
        }
    }

    return 0;
}

void llenarListaDiscos(List<int> &listaDiscos)
{
    cout << "Pasa por llenardiscos" << endl;
}
