#ifndef INTERCALACAOBALANCEADA_H
#define INTERCALACAOBALANCEADA_H

#include <stdio.h>
#include "dados.h"

// Função para ordenar blocos sem substituição por seleção
void gerarBlocosOrdenados(FILE* arquivoDeEntrada, FILE* fitasDeEntrada[], int quantidade);

// Função para gerar blocos usando substituição por seleção (heap)
void gerarBlocosComHeap(FILE* arquivoDeEntrada, FILE* fitasDeEntrada[], int quantidade);

// Função de intercalação balanceada final
void interBalanFinal(FILE* fitasDeEntrada[TOTAL_FITAS_ENTRADA], FILE* fitasDeSaida[], int quantidade);

#endif
