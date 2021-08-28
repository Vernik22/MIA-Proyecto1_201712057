#ifndef FDISK_H
#define FDISK_H
#include <iostream>
#include <vector>
#include "estructuras.h"

using namespace std;

class fdisk
{

public:
    fdisk();

    int size;
    string type;
    string unit;
    string path;
    string ajuste;
    string borrar;
    string name;
    int add;
    void ejecutarComandoFdisk(fdisk *disco);
    void imprimirDatosDisco(string path);
    bool ejecFdisk(string nombreComando, Propiedad propiedades[], int cont);
    void dirExist(fdisk *disco);
    vector<string> split(string str, char pattern);
};

#endif //FDISK_H