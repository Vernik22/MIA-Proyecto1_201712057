#ifndef REP_H
#define REP_H
#include "estructuras.h"
#include <string.h>
#include <vector>

using namespace std;

class rep
{
public:
    rep();
    string name;
    string path;
    string id;
    string ruta;
    string root;
    bool ejecRep(string nombreComando, Propiedad propiedades[], vector<DISCO> &listaDiscos);
    vector<string> split(string str, char pattern);
    void dirExist(rep *disco);
    bool graficarMbr_Ebr(rep *disco, vector<DISCO> &listaDiscos);
    bool graficarDisco(rep *disco, vector<DISCO> &listaDiscos);
    string recorrerListaDisco(string id, vector<DISCO> &listaDiscos);
    void crearArchivo(string path,string data);
};

#endif //REP_H