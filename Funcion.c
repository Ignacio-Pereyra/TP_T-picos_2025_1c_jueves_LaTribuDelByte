#include "Funcion.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int leerArchivoTxt(const char *txtName, int sizeTxt, EsErrorFatal esErrorFatalEmpleado)
{
    char linea[TAM_LINEA];
    void* registro = malloc(sizeTxt);

    if(!registro){
        return ERROR_MEMORIA;
    }

    FILE* archLectura = fopen(txtName, "r");
    FILE* archEscritura = fopen("salida.csv", "w");

    if(!archLectura || !archEscritura){
        free(registro);
        if(archLectura) fclose(archLectura);
        if(archEscritura) fclose(archEscritura);
        return ERROR_ARCHIVO_TXT;
    }

    while(fgets(linea, sizeTxt, archLectura)){
        CommaToDot(linea, registro);
        fprintf(archEscritura, linea);
    }

    fclose(archLectura);
    fclose(archEscritura);
    free(registro);

    return TODO_OK;
}


bool dateFormatChange(char* linea, void* registro, FILE *txtEscribir) {
    char fechatxt[20];
    icc *iicLinea = registro;

    // Validar salto de línea
    char* act = strchr(linea, '\n');
    if (!act) {
        return ERROR_LINEA_LARGA;
    }
    *act = '\0'; // Cortar línea en '\n'

    // Cortar en punto y coma
    act = strchr(linea, ';');
    if (!act) return ERROR_LINEA_LARGA;
    *act = '\0';  // Terminar la fecha

    // Extraer y convertir fecha
    sscanf(linea, "\"%[^\"]\"", fechatxt);
    sscanf(fechatxt, "%d/%d/%d", &iicLinea->Fecha.dia, &iicLinea->Fecha.mes, &iicLinea->Fecha.anio);

    // Escribir en archivo en nuevo formato
    fprintf(txtEscribir, "%04d-%02d-%02d;", iicLinea->Fecha.anio, iicLinea->Fecha.mes, iicLinea->Fecha.dia);

    return true;
}

bool CommaToDot(char* linea, void* registro){
    char* act = strchr(linea, '\n');
    if (!act) {
        return ERROR_LINEA_LARGA;
    }
    //*act = '\0';  // lo saco por ahora para leer bien el archivo
    act = strrchr(linea, ';');
    act = act + 1;

    while(*act != '\0'){
        if(*act == ','){
            *act = '.';
            return true;
        }
        act = act + 1;
    }

    return true;
}

bool esErrorFatalEmpleado(int cod)
{
    switch(cod)
    {
        case 1:
            return true;

        default:
            return false;
    }
}
