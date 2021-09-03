#include "rep.h"
#include "estructuras.h"
#include <string.h>
#include <algorithm>
#include <sys/stat.h>
#include <vector>
#include <fstream>
#include <stdlib.h>

using namespace std;

rep::rep()
{
}
bool rep::ejecRep(string nombreComando, Propiedad propiedades[], vector<DISCO> &listaDiscos)
{
    printf("------------------------------Ejecutar comando REP------------------------------\n");
    try
    {
        rep *d = new rep();
        bool paramValid = true;
        bool flagName = false;
        bool flagPath = false;
        bool flagId = false;
        for (int i = 0; i < 10; i++)
        {
            Propiedad propiedadTemp = propiedades[i];
            string nombrePropiedad = propiedadTemp.Name;
            std::for_each(nombrePropiedad.begin(), nombrePropiedad.end(), [](char &c)
                          { c = ::tolower(c); });

            if (nombrePropiedad == "-name")
            {
                flagName = true;
                d->name = propiedadTemp.val;
            }
            else if (nombrePropiedad == "-path")
            {
                flagPath = true;
                d->path = propiedadTemp.val;
            }
            else if (nombrePropiedad == "-id")
            {
                flagId = true;
                d->id = propiedadTemp.val;
            }
            else if (nombrePropiedad == "-ruta")
            {
                d->ruta = propiedadTemp.val;
            }
            else if (nombrePropiedad == "-root")
            {
                d->root = propiedadTemp.val;
            }

            else if (nombrePropiedad != "")
            {
                paramValid = false;
            }
        }

        if (flagName)
        {
            if (flagPath)
            {
                if (flagId)
                {
                    //switch para el tipo de reporte segun el nombre
                    string nombreRep = d->name;
                    std::for_each(nombreRep.begin(), nombreRep.end(), [](char &c)
                                  { c = ::tolower(c); });

                    // Ejecutar comando mkdir para crear la carpeta si no existe
                    dirExist(d);

                    if (nombreRep == "mbr")
                    {
                        graficarMbr_Ebr(d, listaDiscos);
                    }
                    else if (nombreRep == "disk")
                    {
                        graficarDisco(d, listaDiscos);
                    }
                    else if (nombreRep == "inode")
                    {
                        //graficarInode();
                    }
                    else if (nombreRep == "journaling")
                    {
                        //graficarBitacora();
                    }
                    else if (nombreRep == "block")
                    {
                        //graficarBlock();
                    }
                    else if (nombreRep == "bm_inode")
                    {
                        //reporteBm_inode();
                    }
                    else if (nombreRep == "bm_block")
                    {
                        //reporteBm_Block();
                    }
                    else if (nombreRep == "tree")
                    {
                        //reporte_Tree();
                    }
                    else if (nombreRep == "sb")
                    {
                        //graficarSb();
                    }
                    else if (nombreRep == "file")
                    {
                        /* code */
                    }
                    else if (nombreRep == "ls")
                    {
                        /* code */
                    }
                    else
                    {
                        cout << "Nombre reporte no existe, Error" << endl;
                        return false;
                    }

                    //  ejecutarComandoRep(d, listaDiscos);
                }
                else
                {
                    cout << "Parametro -id no econtrado" << endl;
                    return false;
                }
            }
            else
            {
                cout << "Parametro -path no econtrado" << endl;
                return false;
            }
        }
        else
        {
            cout << "Parametro -name no econtrado" << endl;
            return false;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return false;
}

vector<string> rep::split(string str, char pattern)
{

    int posInit = 0;
    int posFound = 0;
    string splitted;
    vector<string> results;

    while (posFound >= 0)
    {
        posFound = str.find(pattern, posInit);
        splitted = str.substr(posInit, posFound - posInit);
        posInit = posFound + 1;
        results.push_back(splitted);
    }

    return results;
}

void rep::dirExist(rep *disco)
{

    //verificar que el directorio existe
    vector<string> resultados;
    resultados = split(disco->path, '/');
    string newpath = "";
    string pathconc = "";
    for (int i = 1; i < resultados.size() - 1; i++)
    {                                             //llenar el string con el path sin el disk.dk
        pathconc += "/\"" + resultados[i] + "\""; //le agrego comillas a los nombres por si vienen con espacios por ejemplo "mis discos"
        newpath += "/" + resultados[i];           //este es sin comillas para buscar el directorio no importa si vienen con espacios
    }
    //verificar 3 veces si el archivo existe, si el path existe, y enviar el path con comillas

    struct stat st;
    if (stat(disco->path.c_str(), &st) == 0)
    {
        printf("Existe el archivo en: %s\n", disco->path.c_str());
    }
    else if (stat(newpath.c_str(), &st) == 0)
    {
        printf("Existe el directorio: %s\n", newpath.c_str());
    }
    else
    {
        printf("No Existe el directorio: %s\n", disco->path.c_str());
        string comando = "mkdir " + pathconc;
        system(comando.c_str());
        printf("Se creo el dir en: %s \n", disco->path.c_str());
    }
}
bool rep::graficarMbr_Ebr(rep *disco, vector<DISCO> &listaDiscos)
{
    char nombreParticion[15];
    string buffer;
    vector<string> graph;

    buffer = "digraph G{\nsubgraph cluster{\nlabel=\"MBR\"\ntbl[shape=box,label=<\n<table border='0' cellborder='1' cellspacing='0' width='300'  height='200' >\n<tr>  <td width='150'> <b>Nombre</b> </td> <td width='150'> <b>Valor</b> </td>  </tr>\n";
    graph.push_back(buffer);
    string pathDisco = recorrerListaDisco(disco->id, listaDiscos);
    FILE *arch;
    arch = fopen(pathDisco.c_str(), "wb");
    if (arch == NULL)
    {
        cout << "No existe la ruta " << pathDisco << endl;
        fclose(arch);
        return false;
    }
    else
    {
        MBR mbr1;
        fseek(arch, 0, SEEK_SET);
        fread(&mbr1, sizeof(MBR), 1, arch);
        string s = to_string(mbr1.mbr_tamano);
        buffer = "<tr>  <td><b>mbr_tamaño</b></td><td>" + s + "</td>  </tr>\n";
        graph.push_back(buffer);
        s = mbr1.mbr_fecha_creacion;
        buffer = "<tr>  <td><b>mbr_Fecha_Creacion</b></td><td>" + s + "</td>  </tr>\n";
        graph.push_back(buffer);
        s = to_string(mbr1.No_identificacion);
        buffer = "<tr>  <td><b>mbr_Disk_Signature</b></td><td>" + s + "</td>  </tr>\n";
        graph.push_back(buffer);
        s = "F";
        buffer = "<tr>  <td><b>mbr_Disk_Fit</b></td><td>" + s + "</td>  </tr>";
        graph.push_back(buffer);

        Particion parts[4];
        for (int i = 0; i < 4; i++)
        {
            parts[i] = mbr1.particiones[i];
        }

        for (int i = 0; i < 4; i++)
        {

            if (parts[i].NombreParticion != nombreParticion)
            {
                s = to_string(i + 1);
                string a = "" + parts[i].Status_particion;
                buffer = "<tr>  <td><b>part_Status_" + s + "</b></td><td>" + a + "</td>  </tr>\n";
                graph.push_back(buffer);
                a = "" + parts[i].TipoParticion;
                buffer = "<tr>  <td><b>Tipo_Particion_" + s + "</b></td><td>" + a + "</td>  </tr>\n";
                graph.push_back(buffer);
                a = "" + parts[i].TipoAjuste;
                buffer = "<tr>  <td><b>Tipo_Ajuste_" + s + "</b></td><td>" + a + "</td>  </tr>\n";
                graph.push_back(buffer);
                a = to_string(parts[i].Inicio_particion);
                buffer = "<tr>  <td><b>Inicio_Particion_" + s + "</b></td><td>" + a + "</td>  </tr>\n";
                graph.push_back(buffer);
                a = to_string(parts[i].TamanioTotal);
                buffer = "<tr>  <td><b>TamanioTotal_" + s + "</b></td><td>" + a + "</td>  </tr>\n";
                graph.push_back(buffer);
                a = parts[i].NombreParticion;
                buffer = "<tr>  <td><b>NombreParticion_" + s + "</b></td><td>" + a + "</td>  </tr>\n";
                graph.push_back(buffer);
            }
        }

        buffer = "</table>\n>];\n}";
        graph.push_back(buffer);
        //verificar si hay EBR

        for (int i = 0; i < 4; i++)
        {
            if (parts[i].TipoParticion == 'E')
            {
                buffer = "subgraph cluster_1{\n label=\"EBR_Inicial\"\ntbl_1[shape=box, label=<\n<table border='0' cellborder='1' cellspacing='0'  width='300' height='160' >\n<tr>  <td width='150'><b>Nombre</b></td> <td width='150'><b>Valor</b></td>  </tr>\n";
                graph.push_back(buffer);
                int inicioExtendida = parts[i].Inicio_particion;
                fseek(arch, inicioExtendida, SEEK_SET);
                EBR ebr1;
                fread(&ebr1, sizeof(EBR), 1, arch);
                s = to_string(i + 1);
                string a = "" + ebr1.TipoAjuste;
                buffer = "<tr>  <td><b>Tipo_Ajuste_" + s + "</b></td><td>" + a + "</td>  </tr>\n";
                graph.push_back(buffer);
                a = to_string(ebr1.Inicio_particion);
                buffer = "<tr>  <td><b>Inicio_Particion_" + s + "</b></td><td>" + a + "</td>  </tr>\n";
                graph.push_back(buffer);
                a = to_string(ebr1.part_next);
                buffer = "<tr>  <td><b>Particion_Siguiente_" + s + "</b></td><td>" + a + "</td>  </tr>\n";
                graph.push_back(buffer);
                a = to_string(ebr1.TamanioTotal);
                buffer = "<tr>  <td><b>TamanioTotal_" + s + "</b></td><td>" + a + "</td>  </tr>\n";
                graph.push_back(buffer);
                a = ebr1.NombreParticion;
                buffer = "<tr>  <td><b>NombreParticion_" + s + "</b></td><td>" + a + "</td>  </tr>\n";
                graph.push_back(buffer);
                buffer = "</table>\n>];\n}";
                graph.push_back(buffer);
                if (ebr1.part_next == -1)
                {
                    printf("No hay particiones Logicas \n");
                }
                else
                {
                    int cont = 0;
                    fseek(arch, inicioExtendida, SEEK_SET);
                    fread(&ebr1, sizeof(EBR), 1, arch);
                    while (true)
                    {
                        if (ebr1.part_next == -1)
                        {
                            break;
                        }
                        else
                        {
                            fseek(arch, ebr1.part_next, SEEK_SET);
                            fread(&ebr1, sizeof(EBR), 1, arch);
                            cont++;
                        }
                        s = to_string(cont + 1);
                        string c = to_string(cont);
                        buffer = "subgraph cluster_" + s + "{\n label=\"EBR_" + c + "\"\ntbl_" + s + "[shape=box, label=<\n<table border='0' cellborder='1' cellspacing='0'  width='300' height='160' >\n<tr>  <td width='150'><b>Nombre</b></td> <td width='150'><b>Valor</b></td>  </tr>\n";
                        graph.push_back(buffer);
                        a = ebr1.TipoAjuste;
                        buffer = "<tr>  <td><b>Tipo_Ajuste_" + c + "</b></td><td>" + a + "</td>  </tr>\n";
                        graph.push_back(buffer);
                        a = to_string(ebr1.Inicio_particion);
                        buffer = "<tr>  <td><b>Inicio_Particion_" + c + "</b></td><td>" + a + "</td>  </tr>\n";
                        graph.push_back(buffer);
                        a = to_string(ebr1.part_next);
                        buffer = "<tr>  <td><b>Particion_Siguiente_" + c + "</b></td><td>" + a + "</td>  </tr>\n";
                        graph.push_back(buffer);
                        a = to_string(ebr1.TamanioTotal);
                        buffer = "<tr>  <td><b>TamanioTotal_" + c + "</b></td><td>" + a + "</td>  </tr>\n";
                        graph.push_back(buffer);
                        a = ebr1.NombreParticion;
                        buffer = "<tr>  <td><b>NombreParticion_" + c + "</b></td><td>" + a + "</td>  </tr>\n";
                        graph.push_back(buffer);
                        buffer = "</table>\n>];\n}";
                        graph.push_back(buffer);
                    }
                }
            }
        }
        buffer = "}\n}";
        graph.push_back(buffer);
        fclose(arch);
        string datos;
        int jj = graph.size();

        for (int i = 0; i < jj; i++)
        {
            datos = datos + graph[i];
        }
        crearArchivo(disco->path, datos);
        return false;
    }

    fclose(arch);

    return false;
}

//Graficar disco y calcular porcentajes
bool rep::graficarDisco(rep *disco, vector<DISCO> &listaDiscos)
{
    char nombreParticion[15];
    string buffer;
    vector<string> graph;

    buffer = "digraph G{\ntbl [\nshape=box\nlabel=<\n<table border='0' cellborder='2' width='100' height=\"30\" color='lightblue4'>\n<tr>";
    graph.push_back(buffer);
    string pathDisco = recorrerListaDisco(disco->id, listaDiscos);
    FILE *arch;
    arch = fopen(pathDisco.c_str(), "wb");
    if (arch == NULL)
    {
        cout << "No existe la ruta " << pathDisco << endl;
        fclose(arch);
        return false;
    }
    else
    {
        float porcentajeUtilizado = 0.0;
        int espacioUtilizado = 0;
        MBR mbr1;
        fseek(arch, 0, SEEK_SET);
        fread(&mbr1, sizeof(MBR), 1, arch);
        int TamanioDisco = mbr1.mbr_tamano;
        Particion parts[4];
        for (int i = 0; i < 4; i++)
        {
            parts[i] = mbr1.particiones[i];
        }
        buffer = "<td height='30' width='75'> MBR </td>";
        graph.push_back(buffer);
        for (int i = 0; i < 4; i++)
        {
            if (parts[i].NombreParticion != nombreParticion && parts[i].TipoParticion == 'P')
            {
                porcentajeUtilizado = (parts[i].TamanioTotal / TamanioDisco) * 100;
                string s = parts[i].NombreParticion;
                string a = to_string(porcentajeUtilizado);
                buffer = "<td height='30' width='75.0'>PRIMARIA <br/>" + s + " <br/> Ocupado: " + a + "%</td>";
                graph.push_back(buffer);
                espacioUtilizado += parts[i].TamanioTotal;
            }
            else if (parts[i].Status_particion == '0')
            {
                buffer = "<td height='30' width='75.0'>Libre</td>";
                graph.push_back(buffer);
            }
            if (parts[i].TipoParticion == 'E')
            {
                espacioUtilizado += parts[i].TamanioTotal;
                porcentajeUtilizado = (parts[i].TamanioTotal / TamanioDisco) * 100;
                buffer = "<td  height='30' width='15.0'>\n";
                graph.push_back(buffer);
                buffer = "<table border='5'  height='30' WIDTH='15.0' cellborder='1'>\n";
                graph.push_back(buffer);
                string s = parts[i].NombreParticion;
                string a = to_string(porcentajeUtilizado);
                buffer = " <tr>  <td height='60' colspan='100%'>EXTENDIDA <br/>" + s + " <br/> Ocupado:" + a + "%</td>  </tr>\n<tr>";
                graph.push_back(buffer);
                int inicioExtendida = parts[i].Inicio_particion;
                fseek(arch, inicioExtendida, SEEK_SET);
                EBR ebr1;
                fread(&ebr1, sizeof(EBR), 1, arch);
                if (ebr1.part_next == -1)
                {
                    printf("No hay particiones logicas \n");
                }
                else
                {
                    int EspacioUtilizado = 0;
                    int cont = 0;
                    fseek(arch, inicioExtendida, SEEK_SET);
                    fread(&ebr1, sizeof(EBR), 1, arch);
                    while (true)
                    {
                        if (ebr1.part_next == -1)
                        {
                            break;
                        }
                        else
                        {
                            fseek(arch, ebr1.part_next, SEEK_SET);
                            fread(&ebr1, sizeof(EBR), 1, arch);
                            EspacioUtilizado += ebr1.TamanioTotal;
                            porcentajeUtilizado = (ebr1.TamanioTotal / parts[i].TamanioTotal) * 100;
                            s = ebr1.NombreParticion;
                            a = to_string(porcentajeUtilizado);
                            buffer = "<td height='30'>EBR</td><td height='30'> Logica:  " + a + " " + a + "%</td>";
                            graph.push_back(buffer);
                            cont++;
                        }
                    }
                    if ((parts[i].TamanioTotal - EspacioUtilizado) > 0)
                    {
                        porcentajeUtilizado = ((TamanioDisco - EspacioUtilizado) / TamanioDisco) * 100;
                        a = to_string(porcentajeUtilizado);
                        buffer = "<td height='30' width='100%'>Libre: " + a + "%</td>";
                        graph.push_back(buffer);
                    }
                }
                buffer = "</tr>\n";
                graph.push_back(buffer);
                buffer = "</table>\n</td>";
                graph.push_back(buffer);
            }
        }
        if ((TamanioDisco - espacioUtilizado) > 0)
        {
            porcentajeUtilizado = ((TamanioDisco - espacioUtilizado) / TamanioDisco) * 100;
            string a = to_string(porcentajeUtilizado);
            buffer = "<td height='30' width='75.0'>Libre: " + a + "%</td>";
            graph.push_back(buffer);
        }
        buffer = "     </tr>\n</table>\n>];\n}";
        graph.push_back(buffer);
        fclose(arch);
        string datos;
        int jj = graph.size();

        for (int i = 0; i < jj; i++)
        {
            datos = datos + graph[i];
        }
        crearArchivo(disco->path, datos);
        return false;
    }

    fclose(arch);
    return false;
}

string rep::recorrerListaDisco(string id, vector<DISCO> &listaDiscos)
{
    char idDisco = id[2];
    string pathDisco = "";
    string nombreParticion = "";
    string nombreDisco = "";
    int pos = listaDiscos.size();
    for (int i = 0; i < pos; i++)
    {
        DISCO disco;
        disco = listaDiscos[i];
        if (idDisco == disco.id)
        {
            for (int j = 0; j < 100; j++)
            {
                MOUNT mountTemp = disco.Particiones[j];
                if (mountTemp.id == id)
                {
                    mountTemp.EstadoMKS = '1';
                    disco.Particiones[j] = mountTemp;
                    listaDiscos[i] = disco;
                    pathDisco = disco.Path;
                    return pathDisco;
                    break;
                }
            }
        }
        listaDiscos[i] = disco;
    }
    return "";
}

void rep::crearArchivo(string path, string data)
{
    vector<string> resultados;
    resultados = split(path, '/');
    int jj = resultados.size();
    string nombreArchivo = resultados[jj - 1];
    vector<string> rr;
    rr = split(nombreArchivo, '.');
    ofstream archivo;

    archivo.open(path, ios::out); // abriendo archivo
    if (archivo.fail())
    {
        archivo.close();
        cout << "No se pudo crear el archivo";
    }
    else
    {
        archivo << data;
        archivo.close(); // cerrar el archivo

        string comando = "dot -Tpdf " + path + " -o" + rr[0] + ".pdf";
        string comando1 = "xdg-open " + rr[0] + ".pdf";
        string comando2 = "xdg-open " + path;
        system(comando.c_str());
        system(comando1.c_str());
        system(comando2.c_str());
    }
}