#ifndef DADOS_H
#define DADOS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REGISTROS_MEMORIA 20
#define TOTAL_FITAS_ENTRADA 20
#define TOTAL_FITAS_SAIDA 20
#define CAMINHO_DAS_FITAS "storage_fitas/"

typedef struct Registros{
    long int inscricao;
    float nota;
    char restante[150];
}Registros;



#endif