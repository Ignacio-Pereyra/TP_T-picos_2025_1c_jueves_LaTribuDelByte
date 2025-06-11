#include "Funcion.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    fgets(linea, sizeTxt, archLectura); //Leo una vez para saltearme los nombres de los campos.
    while(fgets(linea, sizeTxt, archLectura)){
        //CommaToDot(linea, registro); // Punto 2
        //decryptSentences(linea, registro); //punto 3
        DataLoading(linea, registro);
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

    return false;
}

bool DataLoading(char* linea, void* registro){
    icc * iccGenerales = registro;
    char Fecha[20];
    char indiceTxt[17];
    char* act = strchr(linea, '\n');
    char* act2 = act;
    if (!act) {
        return ERROR_LINEA_LARGA;
    }
    //*act = '\0';  // lo saco por ahora para leer bien el archivo

    sscanf(linea, "\"%19[^\"]\";\"%29[^\"]\";%16s", Fecha, iccGenerales->nivel_general_aperturas, indiceTxt);

    CommaToDot2(indiceTxt); // punto 2

    iccGenerales->indice_icc = atof(indiceTxt); // paso a double por si es necesario trabajar con numero mas adelante

    act = strrchr(linea, ';');   // Voy al ultimo ";"
                                 // incremento en uno para posicionarme despues del ;
    strcpy(act + 1, indiceTxt);  // copio el indicie con . a la linea original para escribir poder hacer un fscanf cuandoo termine la funcion

    *act = '\0';
    act2 = act;
    decryptSentences(iccGenerales->nivel_general_aperturas); // punto 3, desencripto la cadena
    act2 = strrchr(linea, ';');                              // busco el siguiente ; (para atras)
    strcpy(act2 + 1, iccGenerales->nivel_general_aperturas); // copio la cadena a la linea original
    *act = ';';                                              // saco el \0 ya que no es mas fin de linea

    return true;
}

bool CommaToDot2(char* linea){

    while(*linea != '\0'){
        if(*linea == ','){
            *linea = '.';
            return true;
        }
        linea = linea + 1;
    }
    return false;
}

bool decryptSentences(char* linea){
    int pos = 1;
    while((*linea)){
        if(esLetra(*linea)){
            if(esPar(pos)){ // Si es par incrementa en 2
                *linea = tolower(*linea);
                printf(" %d - %d  \n", 122,*linea);
                ((122 - (*linea)) < 2)? (*linea = '`' + ((*linea + 2) % 'z')): (*linea = *linea + 2);
            }else{          // Si es impar incrementa en 4
                *linea = tolower(*linea);
                printf(" %d - %d  \n", 122,*linea);
                ((122 - (*linea)) < 4)? (*linea = '`' + ((*linea + 4) % 'z')): (*linea = *linea + 4);
            }
        }
        pos++;
        linea++;
    }
    return true;
}

bool esLetra(char letra)
{
    return (letra >= 'a' && letra <= 'z') || (letra >= 'A' && letra <= 'Z');
}

bool esPar(int num){
    return num % 2 == 0;
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
