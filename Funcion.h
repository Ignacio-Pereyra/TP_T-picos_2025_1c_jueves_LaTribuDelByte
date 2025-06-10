#ifndef FUNCION_H_INCLUDED
#define FUNCION_H_INCLUDED
#include <stdbool.h>
#include <stdio.h>

#define TAM_LINEA 500
#define TODO_OK 0

// Lista de los distintos tipoos de errores
#define ERROR_MEMORIA 1
#define ERROR_ARCHIVO_TXT 2
#define ERROR_LINEA_LARGA 3
typedef struct
{
    int dia;
    int mes;
    int anio;
}Periodo;

typedef struct{
    Periodo Fecha;
    char nivel_general_aperturas[30];
    double indice_icc;
}icc;
typedef bool (*EsErrorFatal)(int cod);

int leerArchivoTxt(const char *txtName, int sizeTxt, EsErrorFatal esErrorFatalEmpleado);

//Punto 1
bool dateFormatChange(char* linea, void* registro, FILE *archTxt);
bool esErrorFatalEmpleado(int cod);

#endif // FUNCION_H_INCLUDED
