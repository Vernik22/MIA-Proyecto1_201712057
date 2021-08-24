#ifndef MKDISK_H
#define MKDISK_H
#include "estructuras.h"
#include "string"
#include <vector>
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
    vector<string> split(string str, char pattern);
};

#endif // IMKDISK_H
