#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Funcion.h"



int main()
{
    leerArchivoTxt("indices_icc_general_capitulos.csv", sizeof(icc), esErrorFatalEmpleado);
    return 0;
}
