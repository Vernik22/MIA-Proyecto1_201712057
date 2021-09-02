#ifndef MKFS_H
#define MKFS_H
#include "estructuras.h"
#include "string"
#include <vector>

using namespace std;
class mkfs
{

public:
    mkfs();
    string id;
    string type;
    string fs;
    bool ejecMkfs(string nombreComando, Propiedad propiedades[],vector<DISCO> &listaDiscos);
    void ejecutarComandoMkfs(mkfs *disco,vector<DISCO> &listaDiscos);
    vector<MBR> returnMbr(string path,string nombreParticion);
    void crearRaiz(string path, int InicioParticion);
    int cantidadBloqueUsar(string data);
    SB devolverSuperBloque(string path,string nombreParticion);
    bool idValido(string id, vector<DISCO> &listaDiscos);
};

#endif // MKFS_H