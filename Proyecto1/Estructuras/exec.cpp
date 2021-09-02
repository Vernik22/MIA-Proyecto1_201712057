#include "exec.h"
#include <string.h>
#include <algorithm>
#include <sys/stat.h>
#include <vector>
#include "manejador.h"
//#include "manejador.cpp"
#include <fstream>
#include <stdlib.h>
#include <iostream>

using namespace std;

exec::exec()
{
}

bool exec::ejecExec(string nombreComando, Propiedad propiedades[], vector<DISCO> &listaDiscos)
{
    try
    {
        exec *d = new exec();
        bool paramValid = true;
        bool flagPath = false;

        for (int i = 0; i < 10; i++)
        {
            Propiedad propiedadTemp = propiedades[i];
            string nombrePropiedad = propiedadTemp.Name;
            std::for_each(nombrePropiedad.begin(), nombrePropiedad.end(), [](char &c)
                          { c = ::tolower(c); });

            if (nombrePropiedad == "-path")
            {
                flagPath = true;
                d->path = propiedadTemp.val;
            }
            else if (nombrePropiedad != "")
            {
                paramValid = false;
            }
        }

        if (paramValid && flagPath)
        {
            ejecutarComandoExec(d, listaDiscos);
            return paramValid;
        }
        else
        {
            return paramValid;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return false;
}

void exec::ejecutarComandoExec(exec *disco, vector<DISCO> &listaDiscos)
{
    printf("------------------------------Ejecutar comando EXEC------------------------------\n");
    manejador *leer = new manejador();
    ifstream f;
    string data;

    f.open(disco->path, ios::in); //abrimos el archivo en modo lectura

    if (f.fail())
    {
        cout << "no se pudo abrir el archivo" << endl;
    }
    else
    {
        while (!f.eof()) //mientras no sea el final del archivo
        {
            getline(f, data);
            cout << data << endl;
        }
        f.close(); //cerramos el archivo
        leer->leerTexto(data, listaDiscos);
    }
}