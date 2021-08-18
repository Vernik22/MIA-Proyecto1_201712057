#ifndef MKDISK_H
#define MKDISK_H

#include "string"
using namespace std;

class mkdisk{

public:
    mkdisk();
    int size;
    string fechaCreachion;
    string unit;
    string path;
    string ajuste;
    void ejecutarComandoMkdisk(mkdisk *disco);
    void ejecutarComando(string comando);
    void mostrarDatos(mkdisk *disco);
};

#endif // IMKDISK_H
