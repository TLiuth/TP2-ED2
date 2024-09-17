#include "dados.h"
#include "intercalacaoBalanceada.h"

// Função para ajustar o heap
void heapify(Dados arr[], int n, int i) {
    int smallest = i;  
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left].nota < arr[smallest].nota)
        smallest = left;

    if (right < n && arr[right].nota < arr[smallest].nota)
        smallest = right;

    if (smallest != i) {
        Dados temp = arr[i];
        arr[i] = arr[smallest];
        arr[smallest] = temp;
        heapify(arr, n, smallest);
    }
}

// Função para construir o heap
void buildHeap(Dados arr[], int n) {
    for (int i = (n / 2) - 1; i >= 0; i--)
        heapify(arr, n, i);
}

// Função para gerar blocos usando seleção por substituição
void gerarBlocosPorSelecaoSubstituicao(FILE *arquivoDeEntrada, int quantidade) {
    FILE *fitasDeEntrada[TOTAL_FITAS_ENTRADA];
    FILE *fitasDeSaida[TOTAL_FITAS_SAIDA];
    Dados alunos[MAX_REGISTROS_MEMORIA];
    Dados buffer[MAX_REGISTROS_MEMORIA];
    int registrosLidos = 0, contadorSaida = 0;
    int registrosBuffer = 0;

    // Abrir as fitas de entrada para escrita
    for (int i = 0; i < TOTAL_FITAS_ENTRADA; i++) {
        char nomeArquivo[50];
        sprintf(nomeArquivo, "%sfita_entrada%d.txt", CAMINHO_DAS_FITAS, i + 1);
        fitasDeEntrada[i] = fopen(nomeArquivo, "w+");
        if (fitasDeEntrada[i] == NULL) {
            printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
            return;
        }
    }

    // Abrir as fitas de saída para escrita
    for (int i = 0; i < TOTAL_FITAS_SAIDA; i++) {
        char nomeArquivo[50];
        sprintf(nomeArquivo, "%sfita_saida%d.txt", CAMINHO_DAS_FITAS, i + 1);
        fitasDeSaida[i] = fopen(nomeArquivo, "w+");
        if (fitasDeSaida[i] == NULL) {
            printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
            return;
        }
    }

    // Gerar blocos
    while (registrosLidos < quantidade) {
        int registrosNaMemoria = 0;

        // Preencher o heap inicial
        while (registrosNaMemoria < MAX_REGISTROS_MEMORIA && registrosLidos < quantidade) {
            char linha[256];
            if (fgets(linha, sizeof(linha), arquivoDeEntrada) != NULL) {
                sscanf(linha, "%ld %f %2s %50[^\t\n] %30[^\t\n]",
                       &alunos[registrosNaMemoria].inscricao, &alunos[registrosNaMemoria].nota,
                       alunos[registrosNaMemoria].estado, alunos[registrosNaMemoria].cidade,
                       alunos[registrosNaMemoria].curso);
                registrosLidos++;
                registrosNaMemoria++;
            } else {
                break;
            }
        }

        if (registrosNaMemoria == 0) {
            break;
        }

        buildHeap(alunos, registrosNaMemoria);  // Construir o heap inicial

        // Processar o heap e gravar nos blocos de saída
        while (registrosNaMemoria > 0) {
            Dados menor = alunos[0];
            fprintf(fitasDeEntrada[contadorSaida % TOTAL_FITAS_ENTRADA], "%08ld %05.1f %-2s %-50s %-30s\n",
                    menor.inscricao, menor.nota, menor.estado, menor.cidade, menor.curso);

            char linha[256];
            if (fgets(linha, sizeof(linha), arquivoDeEntrada) != NULL) {
                Dados novoRegistro;
                sscanf(linha, "%ld %f %2s %50[^\t\n] %30[^\t\n]",
                       &novoRegistro.inscricao, &novoRegistro.nota, novoRegistro.estado,
                       novoRegistro.cidade, novoRegistro.curso);

                if (novoRegistro.nota >= menor.nota) {
                    alunos[0] = novoRegistro;
                } else {
                    buffer[registrosBuffer++] = novoRegistro;
                    alunos[0] = alunos[--registrosNaMemoria];
                }
            } else {
                alunos[0] = alunos[--registrosNaMemoria];
            }

            heapify(alunos, registrosNaMemoria, 0);
        }

        // Transferir registros do buffer para a memória
        for (int i = 0; i < registrosBuffer; i++) {
            alunos[i] = buffer[i];
        }
        registrosNaMemoria = registrosBuffer;
        registrosBuffer = 0;
        contadorSaida++;
    }

    // Fechar os arquivos de fita de entrada
    for (int i = 0; i < TOTAL_FITAS_ENTRADA; i++) {
        if (fitasDeEntrada[i] != NULL) {
            fclose(fitasDeEntrada[i]);
        }
    }

    // Fechar os arquivos de fita de saída
    for (int i = 0; i < TOTAL_FITAS_SAIDA; i++) {
        if (fitasDeSaida[i] != NULL) {
            fclose(fitasDeSaida[i]);
        }
    }
}

int intercalacaoBalanceadaComSelecaoSubstituicao(FILE *fitasDeEntrada[], FILE *fitasDeSaida[], int quantidadeBlocos) {
    Dados heap[TOTAL_FITAS_ENTRADA];
    int totalFitasRestantes = TOTAL_FITAS_ENTRADA;
    int contadorSaida = 0;
    int fitaOrigem[TOTAL_FITAS_ENTRADA];

    // Guardar os nomes dos arquivos de entrada e saída para reabertura e limpeza
    char nomeFitasEntrada[TOTAL_FITAS_ENTRADA][50];
    char nomeFitasSaida[TOTAL_FITAS_SAIDA][50];

    // Designar uma fita de saída final para conter todos os dados ao final
    int fitaFinal = 0;  // Supondo que a fita 0 será usada para o resultado final

    // Preencher os nomes dos arquivos de entrada e saída
    for (int i = 0; i < TOTAL_FITAS_ENTRADA; i++) {
        sprintf(nomeFitasEntrada[i], "%sfita_entrada%d.txt", CAMINHO_DAS_FITAS, i + 1);
    }
    for (int i = 0; i < TOTAL_FITAS_SAIDA; i++) {
        sprintf(nomeFitasSaida[i], "%sfita_saida%d.txt", CAMINHO_DAS_FITAS, i + 1);
    }

    // Inicializar heap com o primeiro registro de cada fita de entrada
    for (int i = 0; i < totalFitasRestantes; i++) {
        if (fscanf(fitasDeEntrada[i], "%ld %f %2s %50[^\t\n] %30[^\t\n]",
                   &heap[i].inscricao, &heap[i].nota, heap[i].estado,
                   heap[i].cidade, heap[i].curso) == EOF) {
            totalFitasRestantes--;
            heap[i] = heap[totalFitasRestantes];
            fitaOrigem[i] = fitaOrigem[totalFitasRestantes];
            i--;
        } else {
            fitaOrigem[i] = i;
        }
    }

    buildHeap(heap, totalFitasRestantes);

    // Processar o heap até que todas as fitas de entrada estejam vazias
    while (totalFitasRestantes > 0) {
        Dados menor = heap[0];

        // Escrever o menor elemento na fita de saída final
        fprintf(fitasDeSaida[fitaFinal], "%08ld %05.1f %-2s %-50s %-30s\n",
                menor.inscricao, menor.nota, menor.estado,
                menor.cidade, menor.curso);

        int fita = fitaOrigem[0];
        if (fscanf(fitasDeEntrada[fita], "%ld %f %2s %50[^\t\n] %30[^\t\n]",
                   &heap[0].inscricao, &heap[0].nota, heap[0].estado,
                   heap[0].cidade, heap[0].curso) == EOF) {
            heap[0] = heap[--totalFitasRestantes];
            fitaOrigem[0] = fitaOrigem[totalFitasRestantes];
        } else {
            fitaOrigem[0] = fita;
        }

        heapify(heap, totalFitasRestantes, 0);
        contadorSaida++;
    }

    // Fechar as fitas de entrada
    for (int i = 0; i < TOTAL_FITAS_ENTRADA; i++) {
        if (fitasDeEntrada[i] != NULL) {
            fclose(fitasDeEntrada[i]);
        }
    }

    // Fechar todas as fitas de saída, exceto a fita final
    for (int i = 0; i < TOTAL_FITAS_SAIDA; i++) {
        if (fitasDeSaida[i] != NULL && i != fitaFinal) {
            fclose(fitasDeSaida[i]);
            // Limpar as outras fitas (reabrir para sobrescrever)
            FILE *fitaLimpar = fopen(nomeFitasSaida[i], "w"); 
            fclose(fitaLimpar);
        }
    }

    // Limpar as fitas de entrada (reabrir para sobrescrever)
    for (int i = 0; i < TOTAL_FITAS_ENTRADA; i++) {
        FILE *fitaLimpar = fopen(nomeFitasEntrada[i], "w"); 
        fclose(fitaLimpar);
    }

    fclose(fitasDeSaida[fitaFinal]);

    return 0;
}
