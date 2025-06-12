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
        DataLoading(linea, registro); //Dentro esta el punto 2, 3 y 4
        fprintf(archEscritura, linea);
    }

    fclose(archLectura);
    fclose(archEscritura);
    free(registro);

    return TODO_OK;
}

/*
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
*/
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

    CommaToDot2(indiceTxt);                                  // punto 2

    iccGenerales->indice_icc = atof(indiceTxt);              // paso a double por si es necesario trabajar con numero mas adelante

    act = strrchr(linea, ';');                               // Voy al ultimo ";"
                                                             // incremento en uno para posicionarme despues del ;
    strcpy(act + 1, indiceTxt);                              // copio el indicie con . a la linea original para escribir
                                                             // y poder hacer un fscanf cuandoo termine la funcion

    *act = '\0';
    act2 = act;
    decryptSentences(iccGenerales->nivel_general_aperturas); // punto 3, desencripto la cadena
    normalizarCad(iccGenerales->nivel_general_aperturas);    // punto 4, normalizo la cadena
    act2 = strrchr(linea, ';');                              // busco el siguiente ; (para atras)
    strcpy(act2 + 1, iccGenerales->nivel_general_aperturas); // copio la cadena a la linea original
    *act = ';';                                              // saco el \0 ya que no es mas fin de linea
    act2 = strchr(linea, '\0');
    *act2 = ';';
    Clasificador(iccGenerales->nivel_general_aperturas, strchr(linea, '\0')); //Punto 5, agrega un nuevo campo al final de la linea

    linea++;                                                 //Se agrega salto de linea para visualizar mejor el texto y corroborar los resultados
    *linea = '\n';
    return true;
}

bool CommaToDot2(char* linea){      //Punto 1
    char* ptr = linea;
    while(*ptr != '\0'){
        if(*ptr == ','){
            *ptr = '.';
            return true;
        }
        ptr = ptr + 1;
    }
    return false;
}

bool decryptSentences(char* linea){ //Punto 2
    int pos = 1;
    while((*linea)){
        if(esLetra(*linea)){
            if(esPar(pos)){         // Si es par incrementa en 2
                *linea = tolower(*linea);
                (('z' - (*linea)) < 2)? (*linea = '`' + ((*linea + 2) % 'z')): (*linea = *linea + 2);
            }else{                  // Si es impar incrementa en 4
                *linea = tolower(*linea);
                (('z' - (*linea)) < 4)? (*linea = '`' + ((*linea + 4) % 'z')): (*linea = *linea + 4);
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

bool normalizarCad(char* linea){  //Punto 4
    if(esLetra(*linea)){
        *linea = toupper(*linea); //Si el primer caracter es letra, lo paso a mayuscula
    }
    linea++;
    while(*linea){                //Cambio los guiones bajos por espacios hasta llegar al final de la cadena
        if(*linea == '_'){
           *linea = ' ';
        }
        linea++;
    }
    return true;
}

bool Clasificador(char* linea, char* finCad){ // Punto 5
    char texto[2][15] = {"Nivel general","Capitulos"};
    if(strcmp(linea, texto[0]) == 0){
         finCad =  strrchr(finCad, '\0');
        *finCad = ';';
        strcpy(finCad + 1, texto[0]);
        return true;
    }else{
        *finCad = ';';
        strcpy(finCad + 1, texto[1]);
        return true;
    }
    return false;
}

bool esErrorFatalEmpleado(int cod)
{
    switch(cod)
    {
        case ERROR_MEMORIA:
            return true;

        case ERROR_ARCHIVO_TXT:
            return true;

        case ERROR_LINEA_LARGA:
            return true;
        default:
            return false;
    }
}
