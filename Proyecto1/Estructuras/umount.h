#ifndef UMOUNT_H
#define UMOUNT_H
#include "estructuras.h"
#include <string.h>
#include <vector>

using namespace std;

class umount
{

public:
    umount();
    string id;
    bool ejecUmount(string nombreComando, Propiedad propiedades[],vector<DISCO> &listaDiscos);
    void ejecutarComandoUmount(umount *disco, vector<DISCO> &listaDiscos);
};

#endif //UMOUNT_H