#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quicksort.h"
#include "lista.h"
#include "gerador.h"
#include "dados.h"
#include "intercalacaoBalanceada.h"

bool EntradaInvalida(int argc, int metodo, int quantidade, int situacao);
char *retornaSituacao(int situacao);
char *retornaQuantidade(int quantidade);
int operador(int metodo, int quantidade, int situacao, char *flagP, int argc, char *argv[]);


int main(int argc, char *argv[]) {
    int metodo = atoi(argv[1]);
    int quantidade = atoi(argv[2]);
    int situacao = atoi(argv[3]);
    char *flagP = (argc == 5) ? argv[4] : "";

    operador(metodo, quantidade, situacao, flagP, argc, argv);

    return 0;
}

int operador(int metodo, int quantidade, int situacao, char *flagP, int argc, char *argv[]) {
    if (EntradaInvalida(argc, metodo, quantidade, situacao))
        return 0;

    int op;
    printf("Deseja gerar o arquivo novamente?\n");
    scanf("%d", &op);
    if (op == 1)
        txtParaBin("PROVAO.TXT", "PROVAO.bin");

    switch (metodo) {
        case 1: // 2f caminhos sem seleção por substituição
           break;

        case 2: // 2f caminhos com seleção por substituição
            printf("Executando intercalação balanceada com seleção por substituição...\n");
            // Abrir arquivo de entrada
            FILE *arqDeEntrada = fopen("PROVAO.TXT", "r");
            if (arqDeEntrada == NULL) {
                printf("Erro ao abrir o arquivo de entrada\n");
                return 1;
            }
            // Gerar blocos utilizando seleção por substituição
            gerarBlocosPorSelecaoSubstituicao(arqDeEntrada, quantidade);
            fclose(arqDeEntrada);
            // Abertura das fitas de entrada e saída para intercalação
            FILE *fitasDeEntrada[TOTAL_FITAS_ENTRADA];
            FILE *fitasDeSaida[TOTAL_FITAS_SAIDA];
            // Abrir fitas de entrada para leitura
            for (int i = 0; i < TOTAL_FITAS_ENTRADA; i++) {
                char nomeArquivo[50];
                sprintf(nomeArquivo, "%sfita_entrada%d.txt", CAMINHO_DAS_FITAS, i + 1);
                fitasDeEntrada[i] = fopen(nomeArquivo, "r");
                if (fitasDeEntrada[i] == NULL) {
                    printf("Erro ao abrir a fita de entrada %d\n", i + 1);
                    return 1;
                }
            }
            // Abrir fitas de saída para escrita
            for (int i = 0; i < TOTAL_FITAS_SAIDA; i++) {
                char nomeArquivo[50];
                sprintf(nomeArquivo, "%sfita_saida%d.txt", CAMINHO_DAS_FITAS, i + 1);
                fitasDeSaida[i] = fopen(nomeArquivo, "w+");
                if (fitasDeSaida[i] == NULL) {
                    printf("Erro ao abrir a fita de saída %d\n", i + 1);
                    return 1;
                }
            }
            // Executar a intercalação balanceada com seleção por substituição
            intercalacaoBalanceadaComSelecaoSubstituicao(fitasDeEntrada, fitasDeSaida, quantidade);
            printf("Intercalação balanceada com seleção por substituição concluída!\n");
                    break;

        case 3: // quicksort externo
            printf("Executando quicksort externo...\n");
            FILE *ArqLEs, *ArqLi, *ArqEi, *output;
            TipoRegistro R;

            if ((output = fopen("outputTotal.txt", "w")) == NULL)
                exit(1);
            ArqLi = fopen("PROVAO.bin", "r+b");
            if (ArqLi == NULL) {
                printf("Erro na abertura do arquivo!\n");
                exit(1);
            }
            ArqEi = fopen("PROVAO.bin", "r+b");
            if (ArqEi == NULL) {
                printf("Erro na abertura do arquivo!\n");
                exit(1);
            }
            ArqLEs = fopen("PROVAO.bin", "r+b");
            if (ArqLEs == NULL) {
                printf("Erro na abertura do arquivo!\n");
                exit(1);
            }

            QuicksortExterno(&ArqLi, &ArqEi, &ArqLEs, output, quantidade);

            fclose(ArqLi);
            fclose(ArqEi);
            fclose(ArqLEs);
            fclose(output);

            printf("Quicksort externo concluído!\n");

            break;

        default:
            printf("Método inválido!\n");
            break;
    }

    return 0;
}
