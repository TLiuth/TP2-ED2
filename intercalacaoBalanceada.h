#ifndef INTERCALACAOBALANCEADA_H
#define INTERCALACAOBALANCEADA_H
#include "dados.h"

void gerarBlocosOrdenados(FILE* arquivoDeEntrada, int quantidade);
void ordenaBlocoPorHeap(Dados alunos[], int n);
void intercalacaoBalanceada(FILE *fitasDeEntrada[], FILE *fitasDeSaida[], int quantidadeBlocos);
#endif
