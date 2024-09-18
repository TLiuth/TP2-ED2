#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quicksort.h"
#include "lista.h"
#include "gerador.h"
#include "dados.h"
#include "intercalacaoBalanceada.h"

// Funções auxiliares
char *retornaSituacao(int situacao);
char *retornaQuantidade(int quantidade);
int operador(int metodo, int quantidade, int situacao, char *flagP, int argc, char *argv[]);

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Uso: %s <metodo> <quantidade> <situacao> [-P]\n", argv[0]);
        return 1;
    }

    int metodo = atoi(argv[1]);
    int quantidade = atoi(argv[2]);
    int situacao = atoi(argv[3]);
    char *flagP = (argc == 5) ? argv[4] : "";

    operador(metodo, quantidade, situacao, flagP, argc, argv);

    return 0;
}

int operador(int metodo, int quantidade, int situacao, char *flagP, int argc, char *argv[]) {
    int op;
    printf("Deseja gerar o arquivo novamente? (1 - Sim / 0 - Não)\n");
    scanf("%d", &op);
    if (op == 1) {
        txtParaBin("PROVAO.TXT", "PROVAO.bin");
    }

    // Abrindo os arquivos de fitas de entrada e saída
    FILE *fitasDeEntrada[TOTAL_FITAS_ENTRADA], *fitasDeSaida[TOTAL_FITAS_SAIDA];
    for (int i = 0; i < TOTAL_FITAS_ENTRADA; i++) {
        char nomeArquivo[50];
        sprintf(nomeArquivo, "storage_fitas/fita_entrada%d.txt", i + 1);
        fitasDeEntrada[i] = fopen(nomeArquivo, "w+");
        if (fitasDeEntrada[i] == NULL) {
            printf("Erro ao abrir a fita de entrada %d\n", i + 1);
            return 1;
        }
    }
    for (int i = 0; i < TOTAL_FITAS_SAIDA; i++) {
        char nomeArquivo[50];
        sprintf(nomeArquivo, "storage_fitas/fita_saida%d.txt", i + 1);
        fitasDeSaida[i] = fopen(nomeArquivo, "w+");
        if (fitasDeSaida[i] == NULL) {
            printf("Erro ao abrir a fita de saída %d\n", i + 1);
            return 1;
        }
    }

    // Executa o método de ordenação conforme o caso selecionado
    switch (metodo) {
        case 1: // Intercalação balanceada sem seleção por substituição
            printf("Executando intercalação balanceada sem seleção por substituição...\n");
            FILE *arqDeEntrada = fopen("PROVAO.bin", "rb");
            if (arqDeEntrada == NULL) {
                printf("Erro ao abrir o arquivo de entrada!\n");
                return 1;
            }

            gerarBlocosOrdenados(arqDeEntrada, fitasDeEntrada, quantidade);
            interBalanFinal(fitasDeEntrada, fitasDeSaida, quantidade);

            fclose(arqDeEntrada);
            printf("Intercalação sem substituição concluída!\n");
            break;

        case 2: // Intercalação balanceada com seleção por substituição
            printf("Executando intercalação balanceada com seleção por substituição...\n");
            FILE *arqEntradaComHeap = fopen("PROVAO.bin", "rb");
            if (arqEntradaComHeap == NULL) {
                printf("Erro ao abrir o arquivo de entrada!\n");
                return 1;
            }

            gerarBlocosComHeap(arqEntradaComHeap, fitasDeEntrada, quantidade);
            interBalanFinal(fitasDeEntrada, fitasDeSaida, quantidade);

            fclose(arqEntradaComHeap);
            printf("Intercalação com substituição concluída!\n");
            break;

        case 3: // Quicksort externo
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

    // Fechando as fitas de entrada e saída
    for (int i = 0; i < TOTAL_FITAS_ENTRADA; i++) {
        fclose(fitasDeEntrada[i]);
    }
    for (int i = 0; i < TOTAL_FITAS_SAIDA; i++) {
        fclose(fitasDeSaida[i]);
    }

    return 0;
}
