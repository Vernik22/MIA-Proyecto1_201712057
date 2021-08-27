#ifndef RMDISK_H
#define RMDISK_H
#include "estructuras.h"
#include "string"
#include <vector>
using namespace std;

class rmdisk{

public:
    rmdisk();
    int size;
    string fechaCreachion;
    string path;
    void dirExist(rmdisk *disco);
    vector<string> split(string str, char pattern);
    bool ejecRmdisk(string nombreComando, Propiedad propiedades[], int cont);
};

#endif // IMKDISK_H
