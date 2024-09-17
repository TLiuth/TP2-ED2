#ifndef DADOS_H
#define DADOS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REGISTROS_MEMORIA 20
#define TOTAL_FITAS_ENTRADA 20
#define TOTAL_FITAS_SAIDA 20
#define CAMINHO_DAS_FITAS "storage_fitas"

typedef struct Dados {
    long inscricao;
    float nota;
    char estado[3];
    char cidade[51];
    char curso[31];
} Dados;

#endif
