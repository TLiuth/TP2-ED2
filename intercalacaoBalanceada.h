#ifndef INTERCALACAOBALANCEADA_H
#define INTERCALACAOBALANCEADA_H
#include "dados.h"

void gerarBlocosPorSelecaoSubstituicao(FILE* arquivoDeEntrada, int quantidade);
void intercalacaoBalanceadaComSelecaoSubstituicao(FILE *fitasDeEntrada[], FILE *fitasDeSaida[], int quantidadeBlocos);
#endif
