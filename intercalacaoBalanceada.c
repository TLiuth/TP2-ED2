#include "dados.h"
#include "intercalacaoBalanceada.h"

void heapify(Dados arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left].nota > arr[largest].nota)
        largest = left;

    if (right < n && arr[right].nota > arr[largest].nota)
        largest = right;

    if (largest != i) {
        Dados temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        heapify(arr, n, largest);
    }
}

void buildHeap(Dados arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
}

void ordenaBlocoPorHeap(Dados alunos[], int n) {
    buildHeap(alunos, n);

    for (int i = n - 1; i > 0; i--) {
        Dados temp = alunos[0];
        alunos[0] = alunos[i];
        alunos[i] = temp;
        heapify(alunos, i, 0);
    }
}

void gerarBlocosOrdenados(FILE *arquivoDeEntrada, int quantidade) {
    FILE *fitasDeEntrada[TOTAL_FITAS_ENTRADA];
    Dados alunos[MAX_REGISTROS_MEMORIA];
    int registrosLidos = 0, contador = 0;

    // Abrir as fitas de entrada para escrita
    for (int i = 0; i < TOTAL_FITAS_ENTRADA; i++) {
        char nomeArquivo[50];
        sprintf(nomeArquivo, "%sfita_entrada%d.txt", CAMINHO_DAS_FITAS, i + 1);
        fitasDeEntrada[i] = fopen(nomeArquivo, "w+");
        if (fitasDeEntrada[i] == NULL) {
            printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
            fitasDeEntrada[i] = NULL;
            continue;
        }
    }

    while (registrosLidos < quantidade) {
        char linha[256];
        int registrosNaMemoria = -1;

        for (int i = 0; i < MAX_REGISTROS_MEMORIA && registrosLidos < quantidade; i++) {
            if (fgets(linha, sizeof(linha), arquivoDeEntrada) != NULL) {
                sscanf(linha, "%ld %f %2s %50[^\t\n] %30[^\t\n]",
                       &alunos[i].inscricao, &alunos[i].nota, alunos[i].estado,
                       alunos[i].cidade, alunos[i].curso);
                registrosNaMemoria++;
                registrosLidos++;
            }
        }

        if (registrosNaMemoria == 0) {
            break;
        }

        ordenaBlocoPorHeap(alunos, registrosNaMemoria);

        for (int i = 0; i < registrosNaMemoria; i++) {
            fprintf(fitasDeEntrada[contador % TOTAL_FITAS_ENTRADA],
                    "%08ld %05.1f %-2s %-50s %-30s\n",
                    alunos[i].inscricao, alunos[i].nota, alunos[i].estado,
                    alunos[i].cidade, alunos[i].curso);
        }

        contador++;
    }

    for (int i = 0; i < TOTAL_FITAS_ENTRADA; i++) {
        if (fitasDeEntrada[i] != NULL) {
            fclose(fitasDeEntrada[i]);
        }
    }
}

void intercalacaoBalanceada(FILE *fitasDeEntrada[], FILE *fitasDeSaida[], int quantidadeBlocos) {
    Dados heap[TOTAL_FITAS_ENTRADA];
    int totalFitasRestantes = TOTAL_FITAS_ENTRADA;
    int contadorSaida = 0; // Para controlar as fitas de saída

    // Inicializar o heap com o primeiro registro de cada fita de entrada
    for (int i = 0; i < totalFitasRestantes; i++) {
        if (fscanf(fitasDeEntrada[i], "%ld %f %2s %50[^\t\n] %30[^\t\n]",
                   &heap[i].inscricao, &heap[i].nota, heap[i].estado,
                   heap[i].cidade, heap[i].curso) == EOF) {
            totalFitasRestantes--;
            heap[i] = heap[totalFitasRestantes]; // Substitui com o último elemento do heap
            i--;
        }
    }

    // Criar o heap inicial
    buildHeap(heap, totalFitasRestantes);

    // Intercalar enquanto há registros
    while (totalFitasRestantes > 0) {
        Dados menor = heap[0];

        // Escrever no próximo arquivo de saída
        fprintf(fitasDeSaida[contadorSaida % TOTAL_FITAS_SAIDA], "%08ld %05.1f %-2s %-50s %-30s\n",
                menor.inscricao, menor.nota, menor.estado,
                menor.cidade, menor.curso);

        // Verificar de qual fita o menor elemento foi retirado
        int fita = 0; // Ajustar para determinar a fita correta
        if (fscanf(fitasDeEntrada[fita], "%ld %f %2s %50[^\t\n] %30[^\t\n]",
                   &heap[0].inscricao, &heap[0].nota, heap[0].estado,
                   heap[0].cidade, heap[0].curso) == EOF) {
            heap[0] = heap[totalFitasRestantes - 1];
            totalFitasRestantes--; // Diminuir o tamanho do heap
        }

        // Reheapify após inserir o novo registro
        heapify(heap, totalFitasRestantes, 0);

        contadorSaida++; // Avançar para a próxima fita de saída
    }
}
