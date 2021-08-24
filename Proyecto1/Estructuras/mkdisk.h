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
    void dirExist(string path, string pathconc);
};

#endif // IMKDISK_H
