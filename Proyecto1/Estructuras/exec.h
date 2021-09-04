#ifndef EXEC_H
#define EXEC_H
#include "estructuras.h"
#include <string.h>
#include <vector>

using namespace std;

class exec
{

public:
    exec();
    string path;
    bool ejecExec(Propiedad propiedades[],vector<DISCO> &listaDiscos);
    void ejecutarComandoExec(exec *disco,vector<DISCO> &listaDiscos);
};

#endif //EXCEC_H