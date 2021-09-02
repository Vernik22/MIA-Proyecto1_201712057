#include <iostream>

#include "Estructuras/manejador.h"
#include "Estructuras/manejador.cpp"
#include <vector>
#include "Estructuras/estructuras.h"

using namespace std;
//vector<DISCO>
void llenarListaDiscos(vector<DISCO> &listaDiscos)
{
    vector<char> idDis = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
                          'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
                          'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

    for (int i = 0; i < 26; i++)
    {
        DISCO dis;
        dis.estado = '0';
        dis.id = idDis[i];
        for (int j = 0; j < 100; j++)
        {
            MOUNT mountTemp;
            mountTemp.NombreParticion = "";
            mountTemp.id = to_string(j + 1);
            mountTemp.estado = '0';
            dis.Particiones[j] = mountTemp;
        }
        listaDiscos.push_back(dis);
    }
    /*
    cout << "Pasa por llenardiscos" << endl;
    cout << listaDiscos[0].estado << endl;
    cout << listaDiscos[0].id << endl;
    cout << listaDiscos[0].Particiones[0].id << endl;
    cout << listaDiscos[0].Particiones[0].estado << endl;
    */
}

int main()
{

    vector<DISCO> listaDiscos;
    llenarListaDiscos(listaDiscos);
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
            leer->leerTexto(comando, listaDiscos);
            //leer->leerTexto(comando);

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
