#ifndef MOUNT_H
#define MOUNT_H
#include "estructuras.h"
#include <string.h>
#include <vector>

using namespace std;

class mount
{

public:
    mount();
    string path;
    string name;
    bool ejecMount(string nombreComando, Propiedad propiedades[], vector<DISCO> &listaDiscos);
    void dirExist(mount *disco, vector<DISCO> &listaDiscos);
    void ejecutarReporteMount(vector<DISCO> &listaDiscos);
    bool ejecutarComandoMount(mount *disco, vector<DISCO> &listaDiscos);
    void particionMontar(vector<DISCO> &listaDiscos, string nombreParticion,string nombreDisco, string path);
    bool existeDisco(vector<DISCO> &listaDiscos, string nombreDisco);
    vector<string> split(string str, char pattern);
};

#endif //MOUNT_H