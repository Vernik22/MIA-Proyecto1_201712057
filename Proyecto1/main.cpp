#include "iostream"
#include "Estructuras/manejador.h"
#include "Estructuras/Lista/list.h"
#include "Estructuras/Lista/list.cpp"
using namespace std;
void llenarListaDiscos(List<int>);

int main()
{
    List<int> listaDiscos;
    llenarListaDiscos(listaDiscos);
    string p="------------------------------Ingrese un comando------------------------------\n";

    string line;

    while(line!="salir"){    //esto me sirve para seguir leyendo siempre los comandos sin salirme
        cout << p;
        getline(cin,line);
       

    }


}

void llenarListaDiscos(List<int> listaDiscos){
cout<< "Pasa por llenardiscos"<< endl;

}

