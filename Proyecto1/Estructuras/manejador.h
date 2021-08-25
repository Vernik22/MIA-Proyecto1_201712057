#ifndef MANEJADOR_H
#define MANEJADOR_H
#include <iostream>
#include <vector>
#include "estructuras.h"


using namespace std;

class manejador
{
private:
    /* data */
public:
    manejador(/* args */);

    void leerTexto(string data, vector<int> &listaDiscos);
    void listaComandosValidos(vector<Comando> listaComandos);
    vector<string> split(string str, char pattern);
};


#endif //MANEJADOR_H
