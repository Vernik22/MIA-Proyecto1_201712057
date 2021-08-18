#include "iostream"
#include "Estructuras/manejador.h"
#include "Estructuras/Lista/list.h"
#include "Estructuras/Lista/list.cpp"
using namespace std;
void llenarListaDiscos(List<int>& lista);

int main()
{
    List<int> listaDiscos;
    llenarListaDiscos(listaDiscos);
    string p="------------------------------Ingrese un comando------------------------------\n";
    string s=">> ";
    string comando="";

    while(comando!="salir"){    //esto me sirve para seguir leyendo siempre los comandos sin salirme
        cout << p;
        cout << s;
        comando="";
        getline(cin,comando);
       
        if(comando!="" && comando!="salir"){
            cout<<"leyendo comando"<<endl;
        }
    }


}

void llenarListaDiscos(List<int>& listaDiscos){
cout<< "Pasa por llenardiscos"<< endl;

}

