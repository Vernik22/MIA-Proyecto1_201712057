#include "mkfs.h"
#include "estructuras.h"
#include <vector>
#include <string.h>
#include <algorithm>
#include <sys/stat.h>
#include <time.h>

using namespace std;

mkfs::mkfs()
{
}

bool mkfs::ejecMkfs(string nombreComando, Propiedad propiedades[], vector<DISCO> &listaDiscos)
{
    try
    {
        mkfs *d = new mkfs();
        bool paramValid = true;
        bool flagId = false; // bandera para saber si el parametro obligatorio vino
        bool flagType = true;
        bool flagFs = true; //banderas para saber si vino o no el parametro
        for (int i = 0; i < 10; i++)
        {
            Propiedad propiedadTemp = propiedades[i];
            string nombrePropiedad = propiedadTemp.Name;
            std::for_each(nombrePropiedad.begin(), nombrePropiedad.end(), [](char &c)
                          { c = ::tolower(c); });

            if (nombrePropiedad == "-id")
            {
                flagId = true;
                d->id = propiedadTemp.val;
            }
            else if (nombrePropiedad == "-type")
            {
                flagType = false;
                d->type = propiedadTemp.val;
            }
            else if (nombrePropiedad == "-fs")
            {
                flagFs = false;
                d->fs = propiedadTemp.val;
            }
            else if (nombrePropiedad != "")
            {
                paramValid = false;
            }
        }

        if (flagType)
        {
            d->type = "full";
        }
        if (flagFs)
        {
            d->fs = "2fs";
        }

        if (flagId && paramValid)
        {
            ejecutarComandoMkfs(d, listaDiscos);
            return paramValid;
        }
        else
        {
            printf("Parametros invalidos, no hay id o parametros de mas \n");
            paramValid = false;
            return paramValid;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return false;
}

void mkfs::ejecutarComandoMkfs(mkfs *disco, vector<DISCO> &listaDiscos)
{
    //fecha y hora actual
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    //printf("Current local time and date: %s", asctime(timeinfo));
    string fecha = asctime(timeinfo);

    bool dValido = idValido(disco->id, listaDiscos);
    if (dValido)
    {
        char NoParticion = disco->id[2];
        char IdDisco = disco->id[3];
        string pathDisco = "";
        string nombreParticion = "";
        string nombreDisco = "";
        int pos = listaDiscos.size();
        for (int i = 0; i < pos; i++)
        {
            DISCO dis;
            dis = listaDiscos[i];

            if (dis.id == IdDisco)
            {
                for (int j = 0; j < 100; j++)
                {
                    MOUNT mountTemp = dis.Particiones[j];
                    if (mountTemp.id == disco->id)
                    {
                        mountTemp.EstadoMKS = '1';
                        nombreParticion = mountTemp.NombreParticion;
                        pathDisco = dis.Path;
                        nombreDisco = dis.NombreDisco;
                        dis.Particiones[j] = mountTemp;
                        break;
                    }
                }
            }
            listaDiscos[i] = dis;
        }
        vector<MBR> res = returnMbr(pathDisco, nombreParticion); //retorno vector con mbr pero el segundo es un truco porque retorno el size particion y el inicio de la particion
        MBR mbr1 = res[0];
        MBR otr = res[1];
        int sizeParticion = otr.mbr_tamano;
        int InicioParticion = otr.No_identificacion;
        SB superBloque;
        AVD avd;
        DD dd;
        Inodo inodo;
        Bloque bloque;
        JOURNAL bitacora;

        //n
        int noEstructuras = ((sizeParticion - 2 * sizeof(SB)) / (27 + sizeof(AVD) + sizeof(DD) + 5 * sizeof(Inodo) + 20 * sizeof(Bloque) + sizeof(JOURNAL)));

        //No Estructuras
        int cantidadAvd = noEstructuras;
        int cantidadDd = noEstructuras;
        int cantidadInodos = noEstructuras;
        int cantidadBloques = 4 * cantidadInodos;
        int Bitacoras = noEstructuras;

        //Bitmaps
        int InicioBitmapAvd = InicioParticion + sizeof(SB);
        int inicioAvd = InicioBitmapAvd + cantidadAvd;
        int InicioBitmapDD = inicioAvd + (sizeof(AVD) * cantidadAvd);
        int InicioDD = InicioBitmapDD + cantidadDd;
        int InicioBitmapInodo = InicioDD + (sizeof(DD) * cantidadDd);
        int InicioInodo = InicioBitmapInodo + cantidadInodos;
        int InicioBitmapBloque = InicioInodo + (sizeof(Inodo) * cantidadInodos);
        int InicioBloque = InicioBitmapBloque + cantidadBloques;
        int InicioBitacora = InicioBloque + (sizeof(Bloque) * cantidadBloques);
        int InicioCopiaSB = InicioBitacora + (sizeof(JOURNAL) * Bitacoras);

        //iniciando superbloque
        strcpy(superBloque.Sb_nombre_hd, nombreDisco.c_str());
        superBloque.Sb_arbol_virtual_count = cantidadAvd;
        superBloque.Sb_detalle_directorio_count = cantidadDd;
        superBloque.Sb_inodos_count = cantidadInodos;
        superBloque.Sb_bloques_count = cantidadBloques;

        superBloque.Sb_arbol_virtual_free = cantidadAvd;
        superBloque.Sb_detalle_directorio_free = cantidadDd;
        superBloque.Sb_inodos_free = cantidadInodos;
        superBloque.Sb_bloques_free = cantidadBloques;
        strcpy(superBloque.Sb_date_creacion, fecha.c_str());
        strcpy(superBloque.Sb_date_ultimo_montaje, fecha.c_str());
        superBloque.Sb_montajes_count = 1;
        //Bitmaps
        superBloque.Sb_ap_bitmap_arbol_directorio = InicioBitmapAvd;
        superBloque.Sb_ap_arbol_directorio = inicioAvd;
        superBloque.Sb_ap_bitmap_detalle_directorio = InicioBitmapDD;
        superBloque.Sb_ap_detalle_directorio = InicioDD;
        superBloque.Sb_ap_bitmap_tabla_inodo = InicioBitmapInodo;
        superBloque.Sb_ap_tabla_inodo = InicioInodo;
        superBloque.Sb_ap_bitmap_bloques = InicioBitmapBloque;
        superBloque.Sb_ap_bloques = InicioBloque;
        superBloque.Sb_ap_log = InicioBitacora;
        superBloque.Sb_size_struct_arbol_directorio = sizeof(avd);
        superBloque.Sb_size_struct_Detalle_directorio = sizeof(dd);
        superBloque.Sb_size_struct_inodo = sizeof(inodo);
        superBloque.Sb_size_struct_bloque = sizeof(bloque);
        superBloque.Sb_first_free_bit_arbol_directorio = InicioBitmapAvd;
        superBloque.Sb_first_free_bit_detalle_directoriio = InicioBitmapDD;
        superBloque.Sb_dirst_free_bit_tabla_inodo = InicioBitmapInodo;
        superBloque.Sb_first_free_bit_bloques = InicioBitmapBloque;
        superBloque.Sb_magic_num = 201712057;
        superBloque.InicioCopiaSB = InicioCopiaSB;
        superBloque.ConteoAVD = 0;
        superBloque.ConteoDD = 0;
        superBloque.ConteoInodo = 0;
        superBloque.ConteoBloque = 0;

        //Escribir en Particion
        FILE *arch;
        arch = fopen(pathDisco.c_str(), "rb+");
        if (arch == NULL)
        {
            cout << "Error! El disco no existe F\n"; //si da null es porque no se encontro el archivo
        }
        else
        {
            //Escribir super boot
            fseek(arch, InicioParticion, SEEK_SET);
            fwrite(&superBloque, sizeof(SB), 1, arch);
            //Escribir BitMap Arbol Virtual de Directorios
            fseek(arch, InicioBitmapAvd, SEEK_SET);

            char buff;
            buff = '\0';

            for (int j = 0; j < cantidadAvd; j++)
            {
                fwrite(&buff, sizeof(buff), 1, arch);
                fseek(arch, InicioBitmapAvd + j, SEEK_SET);
            }
            //escribir arbol de directorios
            fseek(arch, inicioAvd, SEEK_SET);

            for (int j = 0; j < cantidadAvd; j++)
            {
                fwrite(&avd, sizeof(avd), 1, arch);
            }
            //escribir bitmap detalle Directorio
            fseek(arch, InicioBitmapDD, SEEK_SET);
            for (int j = 0; j < cantidadDd; j++)
            {
                fwrite(&buff, sizeof(buff), 1, arch);
                fseek(arch, InicioBitmapDD + j, SEEK_SET);
            }

            //escribir detalle Directorio
            fseek(arch, InicioDD, SEEK_SET);
            dd.Dd_ap_detalle_directorio = -1;
            for (int j = 0; j < cantidadDd; j++)
            {
                fwrite(&dd, sizeof(DD), 1, arch);
                fseek(arch, InicioDD + sizeof(DD)*j, SEEK_SET);
            }
            
            //Escribir bitmap tabla inodo
            fseek(arch, InicioBitmapInodo, SEEK_SET);
            for (int j = 0; j < cantidadInodos; j++)
            {
                fwrite(&buff, sizeof(buff), 1, arch);
                fseek(arch, InicioBitmapInodo + j, SEEK_SET);
            }

            //Escribir tabla de inodos
            fseek(arch, InicioInodo, SEEK_SET);
            inodo.I_count_inodo = -1;
            for (int i = 0; i < cantidadInodos; i++)
            {
                fwrite(&inodo, sizeof(Inodo), 1, arch);
                fseek(arch, InicioInodo + sizeof(Inodo)*i, SEEK_SET);
            }
            //Escribir Bitmap Bloque de datos
            fseek(arch, InicioBitmapBloque, SEEK_SET);
            for (int j = 0; j < cantidadBloques; j++)
            {
                fwrite(&buff, sizeof(buff), 1, arch);
                fseek(arch, InicioBitmapBloque + j, SEEK_SET);
            }
            //Escribir Bloque de Datos
            fseek(arch, InicioBloque, SEEK_SET);
            string h = "";
            strcpy(bloque.Db_data,h.c_str());
            for (int j = 0; j < cantidadBloques; j++)
            {
                fwrite(&bloque, sizeof(Bloque), 1, arch);
                fseek(arch, InicioBloque + sizeof(Bloque)*j, SEEK_SET);
            }
            //Escribir Bitacoras
            fseek(arch, InicioBitacora, SEEK_SET);
            bitacora.Size=-1;
            for (int j = 0; j < Bitacoras; j++)
            {
                fwrite(&bitacora, sizeof(JOURNAL), 1, arch);
                fseek(arch, InicioBitacora + sizeof(JOURNAL)*j, SEEK_SET);
            }
            //escribir Copia Super Boot
            fseek(arch, InicioCopiaSB, SEEK_SET);
            fwrite(&superBloque, sizeof(SB), 1, arch);
            crearRaiz(pathDisco, InicioParticion);
            cout<<"No estructuras: "+ noEstructuras<<endl;
            cout<<"Particion a formatear "+nombreParticion+" "+ NoParticion<<endl;
            cout << sizeParticion<<endl;
            cout << "Fecha: "<<mbr1.mbr_fecha_creacion<<endl;

        }
        fclose(arch);
    }
    else
    {
        printf("El id no existe, ERROR \n");
    }
}

vector<MBR> mkfs::returnMbr(string path, string nombreParticion)
{
    MBR mbr1, size;
    vector<MBR> result;
    Particion parts[4];
    char nombre2[15];
    int size;
    strcpy(nombre2, nombreParticion.c_str());
    FILE *arch;
    arch = fopen(path.c_str(), "rb+");
    if (arch == NULL)
    {
        cout << "Error! El disco no existe\n"; //si da null es porque no se encontro el archivo
        result.push_back(mbr1);
        size.mbr_tamano = 0;
        size.No_identificacion = 0;
        result.push_back(size);
        fclose(arch);
        return result;
    }
    else
    {
        fseek(arch, 0, SEEK_SET);
        fread(&mbr1, sizeof(MBR), 1, arch);
        for (int i = 0; i < 4; i++)
        {
            parts[i] = mbr1.particiones[i];
        }
        for (int i = 0; i < 4; i++)
        {
            if (parts[i].NombreParticion == nombre2)
            {
                int s = parts[i].TamanioTotal;
                int f = parts[i].Inicio_particion;
                size.mbr_tamano = s;
                size.No_identificacion = f;
                result.push_back(mbr1);
                result.push_back(size);
                fclose(arch);
                return result;
            }
        }
        for (int i = 0; i < 4; i++)
        {
            if (parts[i].TipoParticion == 'E')
            {
                int inicioExtendida = parts[i].Inicio_particion;
                fseek(arch, inicioExtendida, SEEK_SET);
                EBR ebrTemp;
                fread(&ebrTemp, sizeof(EBR), 1, arch);
                if (ebrTemp.part_next == -1)
                {
                    printf("No hay particiones Logicas \n");
                }
                else
                {
                    fseek(arch, inicioExtendida, SEEK_SET);
                    fread(&ebrTemp, sizeof(EBR), 1, arch);
                    while (true)
                    {
                        if (ebrTemp.part_next == -1)
                        {
                            break;
                        }
                        else
                        {
                            fseek(arch, ebrTemp.part_next, SEEK_SET);
                            fread(&ebrTemp, sizeof(EBR), 1, arch);
                        }
                        if (ebrTemp.NombreParticion == nombre2)
                        {
                            printf("Logica Encontrada \n");
                            int s = ebrTemp.TamanioTotal;
                            int f = ebrTemp.Inicio_particion;
                            size.mbr_tamano = s;
                            size.No_identificacion = f;
                            result.push_back(mbr1);
                            result.push_back(size);
                            fclose(arch);
                            return result;
                        }
                    }
                }
            }
        }
    }

    fclose(arch);

    result.push_back(mbr1);
    size.mbr_tamano = 0;
    size.No_identificacion = 0;
    result.push_back(size);
    return result;
}

bool mkfs::idValido(string id, vector<DISCO> &listaDiscos)
{
    int pos = listaDiscos.size();
    for (int i = 0; i < pos; i++)
    {
        DISCO dis;
        dis = listaDiscos[i];
        if (dis.NombreDisco != "")
        {
            for (int j = 0; j < 100; j++)
            {
                MOUNT mountTemp = dis.Particiones[j];
                if (mountTemp.NombreParticion != "")
                {
                    if (mountTemp.id == id)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void mkfs::crearRaiz(string path, int InicioParticion){
    
}
