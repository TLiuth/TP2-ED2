#include "intercalacaoBalanceada.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Função para ordenar um bloco na memória usando inserção (sem substituição por seleção)
void ordenaBloco(Registros alunos[], int n) {
    int i, j;
    Registros key;

    for (i = 1; i < n; i++) {
        key = alunos[i];
        j = i - 1;

        while (j >= 0 && alunos[j].nota > key.nota) {
            alunos[j + 1] = alunos[j];
            j--;
        }

        alunos[j + 1] = key;
    }
}

// Função para gerar blocos ordenados sem substituição por seleção
void gerarBlocosOrdenados(FILE* arquivoDeEntrada, FILE* fitasDeEntrada[], int quantidade) {
    Registros alunos[MAX_REGISTROS_MEMORIA];
    int registrosLidos = 0, contador = 0;

    while (registrosLidos < quantidade) {
        int registrosNaMemoria = 0;

        for (int i = 0; i < MAX_REGISTROS_MEMORIA && registrosLidos < quantidade; i++) {
            if (fread(&alunos[i], sizeof(Registros), 1, arquivoDeEntrada) == 1) {
                registrosNaMemoria++;
                registrosLidos++;
            } else {
                if (feof(arquivoDeEntrada)) {
                    break;
                } else {
                    perror("Erro ao ler linha do arquivo de origem (arquivoDeEntrada)");
                    exit(EXIT_FAILURE);
                }
            }
        }

        if (registrosNaMemoria == 0) {
            break;
        }

        ordenaBloco(alunos, registrosNaMemoria);

        for (int j = 0; j < registrosNaMemoria; j++) {
            if (fprintf(fitasDeEntrada[contador % TOTAL_FITAS_ENTRADA], "%08ld %05.1f %s\n",
                        alunos[j].inscricao, alunos[j].nota, alunos[j].restante) < 0) {
                perror("Erro ao escrever no arquivo de saída");
                exit(EXIT_FAILURE);
            }

            fflush(fitasDeEntrada[contador % TOTAL_FITAS_ENTRADA]);
        }

        contador++;
    }
}

// Função para reorganizar o heap (min-heapify)
void minHeapify(Registros heap[], int n, int i) {
    int menor = i;
    int esquerda = 2 * i + 1;
    int direita = 2 * i + 2;

    if (esquerda < n && heap[esquerda].nota < heap[menor].nota)
        menor = esquerda;

    if (direita < n && heap[direita].nota < heap[menor].nota)
        menor = direita;

    if (menor != i) {
        Registros temp = heap[i];
        heap[i] = heap[menor];
        heap[menor] = temp;
        minHeapify(heap, n, menor);
    }
}

// Constrói o heap inicial
void construirHeap(Registros heap[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        minHeapify(heap, n, i);
    }
}

// Gera blocos usando seleção por substituição (com heap)
void gerarBlocosComHeap(FILE* arquivoDeEntrada, FILE* fitasDeEntrada[], int quantidade) {
    Registros heap[MAX_REGISTROS_MEMORIA];
    Registros substituto;
    int registrosLidos = 0, contador = 0, tamanhoHeap = 0;

    // Inicializa o heap com os primeiros registros
    while (registrosLidos < MAX_REGISTROS_MEMORIA && registrosLidos < quantidade) {
        if (fread(&heap[registrosLidos], sizeof(Registros), 1, arquivoDeEntrada) == 1) {
            registrosLidos++;
            tamanhoHeap++;
        } else {
            if (feof(arquivoDeEntrada)) {
                break;
            } else {
                perror("Erro ao ler linha do arquivo de origem (arquivoDeEntrada)");
                exit(EXIT_FAILURE);
            }
        }
    }

    construirHeap(heap, tamanhoHeap);

    while (registrosLidos < quantidade) {
        // Escreve o menor elemento do heap na fita
        if (fprintf(fitasDeEntrada[contador % TOTAL_FITAS_ENTRADA], "%08ld %05.1f %s\n",
                    heap[0].inscricao, heap[0].nota, heap[0].restante) < 0) {
            perror("Erro ao escrever no arquivo de saída");
            exit(EXIT_FAILURE);
        }

        fflush(fitasDeEntrada[contador % TOTAL_FITAS_ENTRADA]);

        // Lê o próximo registro
        if (fread(&substituto, sizeof(Registros), 1, arquivoDeEntrada) == 1) {
            if (substituto.nota >= heap[0].nota) {
                heap[0] = substituto;
            } else {
                break;
            }
        } else {
            if (feof(arquivoDeEntrada)) {
                break;
            } else {
                perror("Erro ao ler linha do arquivo de origem (arquivoDeEntrada)");
                exit(EXIT_FAILURE);
            }
        }

        minHeapify(heap, tamanhoHeap, 0);
    }

    // Escreve o restante do heap
    for (int i = 0; i < tamanhoHeap; i++) {
        if (fprintf(fitasDeEntrada[contador % TOTAL_FITAS_ENTRADA], "%08ld %05.1f %s\n",
                    heap[i].inscricao, heap[i].nota, heap[i].restante) < 0) {
            perror("Erro ao escrever no arquivo de saída");
            exit(EXIT_FAILURE);
        }

        fflush(fitasDeEntrada[contador % TOTAL_FITAS_ENTRADA]);
    }

    contador++;
}
void interBalanFinal(FILE *fitasDeEntrada[TOTAL_FITAS_ENTRADA],FILE *fitasDeSaida[], int quantidade){
    int indices[TOTAL_FITAS_ENTRADA] = { 0 };
    Registros memoryInt[MAX_REGISTROS_MEMORIA]; 
    int contador = 0, fitasRestantes = 0;
    int registrosLidos[TOTAL_FITAS_ENTRADA], tamBloco = 20;
    char line[100];int cont=0;
    int qtd_blocos = 0;
    for(int i = 0; i < TOTAL_FITAS_ENTRADA; i++){
        rewind(fitasDeEntrada[i]);
    }
    
    while(tamBloco < quantidade) 
    {
        contador = 0;
       
        cont ++;
        printf("quantidade de veze que entrou no primeiro while:%d\n",cont);
        // loop de fase
        /*
        const int qtt_blocos = quantidade / TOTAL_FITAS_ENTRADA;
        const int qtt_dos_cara = min(TOTAL_FITAS_ENTRADA, quantidade);
        

        1000 / 20 = 50
        ceil{50 / 20} = 3
        */
        qtd_blocos = (int) ceil( ((double) quantidade / (double) TOTAL_FITAS_ENTRADA) / MAX_REGISTROS_MEMORIA);
        while(contador < qtd_blocos)
        {
            printf("RESULTADO = %d\n", qtd_blocos);
            
            for (int i = 0; i < TOTAL_FITAS_ENTRADA; i++)
            {
                
                if (fgets(line, sizeof(line), fitasDeEntrada[i]) != NULL){
                    sscanf(line, "%8ld %5f %[^\n]", &memoryInt[i].inscricao, &memoryInt[i].nota, memoryInt[i].restante);
                
                    indices[i] = 0;
                    registrosLidos[i] = 1;
                    fitasRestantes++;

                }else{
                    indices[i] = -1;
                    registrosLidos[i] = -1;
                    break;
                }
            }
            
            // loop de intercalação
            while (fitasRestantes > 0)
            {   
                // int menorIndice = -1;
                int menorIndice = 0;
                float menorNota = 101.0;
            //  printf("\nvalor do menorIndice antes do for: %d\n",menorIndice);
                // Encontrar a fita com o menor registro (baseado na nota, por exemplo)

                for (int i = 0; i < TOTAL_FITAS_ENTRADA; i++) { 
                    if ((indices[i] == -1) && (registrosLidos[i] == -1))
                        break;   
                    else
                    {
                        if ((indices[i] != -1) && (memoryInt[i].nota < menorNota)) {
                            menorNota = memoryInt[i].nota;
                            menorIndice = i;
                        }
                    }
                }

            
            //    printf("iniciando arquivo de saida aqui!!\n");
                // Para deconsiderar o \n
                fgets(line, sizeof(line), fitasDeEntrada[menorIndice]);
                //printf("line = %s", line);

                // Escreve o menor registro na fita de saída atual
              //  printf("\nfita:%d\n",contador % TOTAL_FITAS_SAIDA);
                fprintf(fitasDeSaida[contador % TOTAL_FITAS_SAIDA], "%08ld %05.1f %s\n", 
                    memoryInt[menorIndice].inscricao, memoryInt[menorIndice].nota, memoryInt[menorIndice].restante);

                // Lê o próximo registro da fita de onde foi retirado o menor valor
                if (fgets(line, sizeof(line), fitasDeEntrada[menorIndice]) != NULL && line[0] != '\n' && registrosLidos[menorIndice] < tamBloco) 
                {
                    // Faz a leitura do novo registro da fita e armazena no buffer
                    if (sscanf(line, "%8ld %5f %[a-z A-Z]'\n'", &memoryInt[menorIndice].inscricao, &memoryInt[menorIndice].nota, memoryInt[menorIndice].restante) == 3) {
                        registrosLidos[menorIndice]++;

                // Caso o registro não seja lido corretamente, indica que a fita terminou
                    } else{
                        printf("Erro sscanf\n");
                    }

                }else
                {
                    // Se não for possível ler um novo registro da fita, indica que a fita terminou
                    indices[menorIndice] = -1;
                    fitasRestantes--;
                    printf("Fim do bloco %d da fita %d\n", contador, menorIndice);
                    printf("fitasRestantes = %d\n\n", fitasRestantes);
                // printf("Fita %d terminada ou sem registros para ler\n", menorIndice);
                }
                
            }
           
            contador++;
            printf("contador %d\n",contador);
        }
        
        fprintf(stderr, "[%s] trocando trocando...\n", __func__); fflush(stderr);
        
        tamBloco *= TOTAL_FITAS_ENTRADA;

        if(tamBloco < quantidade){
            FILE *temp[TOTAL_FITAS_ENTRADA];
            for (int i = 0; i < TOTAL_FITAS_ENTRADA; i++) {
                temp[i] = fitasDeEntrada[i];
                fitasDeEntrada[i] = fitasDeSaida[i];
                fitasDeSaida[i] = temp[i];
                rewind(fitasDeSaida[i]);
                rewind(fitasDeEntrada[i]);
            }
        }
    }
}