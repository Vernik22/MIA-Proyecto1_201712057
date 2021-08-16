%{
#include "scanner.h"
#include <QString>
#include <string>
#include "qdebug.h"
#include <iostream>
#include "imkdisk.h"

using namespace std;
extern int yylineno;
extern int columna;
extern char *yytext;


int yyerror(const char* mens)
{
std::out<< mens<<" "<<yytext<< std::endl;
return 0;
}

%}

%defines "parser.h"
%output "parser.cpp"
%define parse.error verbose
//%error-verbose
%locations
%union{
char TEXT[256];
class imkdisk *mdisk;

}
//Terminales de tipo text, son strings
//mkdisk
%token<TEXT> pmkdisk;
%token<TEXT> psize;
%token<TEXT> ppath;
%token<TEXT> pf;
%token<TEXT> pu;
//rmdisk
%token<TEXT> prmdisk;
//fdisk
%token<TEXT> pfdisk;
%token<TEXT> ptype;
%token<TEXT> pp;
%token<TEXT> pe;
%token<TEXT> pl;
%token<TEXT> pdelete;
%token<TEXT> pname;
%token<TEXT> padd;
%token<TEXT> pbf;
%token<TEXT> pff;
%token<TEXT> pwf;
%token<TEXT> pb;
%token<TEXT> pk;
%token<TEXT> pm;
//Mount
%token<TEXT> pmount;
//Umount
%token<TEXT> pumount;
%token<TEXT> pid;
//Mkfs
%token<TEXT> pmkfs;
%token<TEXT> pfs;
%token<TEXT> pfast;
%token<TEXT> pfull;
%token<TEXT> p2fs;
%token<TEXT> p3fs;
//Login
%token<TEXT> plogin;
%token<TEXT> pusr;
%token<TEXT> ppwd;
%token<TEXT> plogout;
//Mkgrp
%token<TEXT> pmkgrp;
%token<TEXT> prmgrp;
//mkusr
%token<TEXT> pmkusr;
%token<TEXT> pgrp;
//rmusr
%token<TEXT> prmusr;
//chmod
%token<TEXT> pchmod;
%token<TEXT> pugo;
%token<TEXT> pr;
//touch
%token<TEXT> ptouch;
%token<TEXT> pcount;
%token<TEXT> pstdin;
//cat
%token<TEXT> pcat;
%token<TEXT> pfile;
//rm
%token<TEXT> prm;
%token<TEXT> pedit;
//ren
%token<TEXT> pren;
//mkdir
%token<TEXT> pmkdir;
//cp
%token<TEXT> pcp;
%token<TEXT> pdest;
//mv
%token<TEXT> pmv;
//find
%token<TEXT> pfind;
%token<TEXT> qmark;
//chown
%token<TEXT> pchown;
//chgrp
%token<TEXT> pchgrp;
//pause
%token<TEXT> pause;
//exec
%token<TEXT> exec;
//reportes
%token<TEXT> rep;
%token<TEXT> pmbr;
%token<TEXT> pdisk;
%token<TEXT> pinode;
%token<TEXT> pjournaling;
%token<TEXT> pblock;
%token<TEXT> pbminode;
%token<TEXT> pbmblock;
%token<TEXT> ptree;
%token<TEXT> psb;
%token<TEXT> pls;
%token<TEXT> pruta;
%token<TEXT> proot;

%token<TEXT> punto;
%token<TEXT> bracketabre;
%token<TEXT> bracketcierra;
%token<TEXT> corcheteabre;
%token<TEXT> corchetecierra;
%token<TEXT> puntocoma;
%token<TEXT> potencia;
%token<TEXT> coma;
%token<TEXT> parentesisabre;
%token<TEXT> parentesiscierra;

%token<TEXT> llaveabre;
%token<TEXT> llavecierra;
%token<TEXT> mas;
%token<TEXT> menos;
%token<TEXT> multiplicacion;
%token<TEXT> igual;
%token<TEXT> dolar;
%token<TEXT> dospuntos;

%token<TEXT> entero;
%token<TEXT> numnegativo;
%token<TEXT> cadena;
%token<TEXT> identificador;
%token<TEXT> caracter;
%token<TEXT> ruta;
%token<TEXT> rutacualquiera;

%type<mdisk> COMANDOMKDISK; // lista de instrucciones

%left suma menos
%left multi division
%left potencia
%start INICIO
%%

INICIO : LEXPA { }
;

LEXPA:  pmkdisk COMANDOMKDISK
{


$2->mostrarDatos($2);//ejecuto el metodo "mostrarDatos" del objeto retornado en COMANDOMKDISK
printf("estoy en lexpa primera produccion");
}


;

COMANDOMKDISK:
menos psize igual entero {int tam=atoi($4); imkdisk *disco=new imkdisk(); disco->size=tam;  $$=disco;}
;
