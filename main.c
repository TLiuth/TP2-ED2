#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quicksort.h"
#include "lista.h"
#include "gerador.h"



bool EntradaInvalida(int argc, int metodo, int quantidade, int situacao);
char *retornaSituacao(int situacao);
char *retornaQuantidade(int quantidade);
int operador(int metodo,int quantidade,int situacao,char *flagP, int argc, char *argv[]);


int main(int argc, char *argv[]){
    int metodo = atoi(argv[1]);
    int quantidade = atoi(argv[2]);
    int situacao = atoi(argv[3]);
    char *flagP = (argc == 5) ? argv[4] : "";

    operador(metodo, quantidade, situacao, flagP, argc, argv);

    return 0; 
}


int operador(int metodo, int quantidade, int situacao, char *flagP, int argc, char *argv[]){
    if(EntradaInvalida(argc, metodo, quantidade, situacao))
        return 0;

    int op;

    printf("Deseja gerar o arquivo novamente?\n");
    scanf("%d", &op);
    if(op == 1)
        txtParaBin("PROVAO.TXT", "PROVAO.bin");
    

    switch (metodo)
    {
        case 1: // 2f caminhos sem seleção por substituição
            /* code */
            break;
            
        case 2: // 2f caminhos com seleção por substituição
            // code //
            break;

        case 3: // quicksort externo
            FILE *ArqLEs; 
            FILE *ArqLi;
            FILE *ArqEi; 
            FILE *output;
            TipoRegistro R;
            if((output = fopen("outputTotal.txt", "w")) == NULL)
                exit(1);
            ArqLi = fopen("PROVAO.bin", "r+b");
            if(ArqLi == NULL){
                printf("Erro na abertura do arquivo!");
                exit(1);
            }
            ArqEi = fopen("PROVAO.bin", "r+b");
            if(ArqEi == NULL){
                printf("Erro na abertura do arquivo!");
                exit(1);
            }
            ArqLEs = fopen("PROVAO.bin", "r+b");
            if(ArqLEs == NULL){
                printf("Erro na abertura do arquivo!");
                exit(1);
            }

            QuicksortExterno(&ArqLi, &ArqEi, &ArqLEs, 1, quantidade);
            // fclose(ArqEi);
            // fclose(ArqLEs);
            

            fflush(ArqLi); fclose(ArqEi); fclose(ArqLEs);
            read_and_print_records("PROVAO.bin");

            fseek(ArqLi, 0, SEEK_SET);

            // int i = 1;
            // while(fread(&R, sizeof(TipoRegistro), 1, ArqLi)){
            //     // printf("Registro %d: %ld %.2f %s %s %s\n", i, R.inscricao, R.nota, R.estado, R.cidade, R.curso);
            //     fprintf(output, "%08ld %05.1f %-82s\n", 
            //             R.inscricao,
            //             R.nota,
            //             R.restante);
            //     i++;
            // }

            // fclose(output);
            // fclose(ArqLi);


            break;


        default:
            break;
        }


    return 0;
        
    }


bool EntradaInvalida(int argc, int metodo, int quantidade, int situacao){
    if(argc > 5 || argc < 4){
        printf("Formato de entrada não compatível\n");
        return true;
    }

    if(metodo != 1 && metodo != 2 && metodo != 3){
        printf("Método indefinido ou inexistente\n");
        return true;
    }

    if(!(quantidade == 50 || quantidade == 100 || quantidade == 1000 || quantidade == 10000 || quantidade == 100000 || quantidade == 471705)){
        printf("Tamanho de arquivo inválido\n");
        return true;
    }

    if(situacao != 1 && situacao != 2 && situacao != 3){
        printf("Situação inválida\n");
        return true;
    }

    return false;
}

