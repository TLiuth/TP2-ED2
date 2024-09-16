#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <stdio.h>

#define TAM_AREA 220


typedef struct {
    long int inscricao;
    float nota;
    char restante[150];
} TipoRegistro;


typedef struct {
    TipoRegistro *itens;
    int tam;
} TipoArea;

typedef TipoArea Lista;


void quicksortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, int Dir);
void LeSup(FILE **ArqLEs, TipoRegistro *UltLido, int *Ls, short *OndeLer);
void LeInf(FILE **ArqLi, TipoRegistro *UltLido, int *Li, short *OndeLer);
void insereItem(TipoRegistro ultimoLido, TipoArea *area);
void escreveMax(FILE **ArqLEs, TipoRegistro R, int *eSup);
void escreveMin(FILE **arqEInf, TipoRegistro R, int *eInf);
void retiraUltimo(TipoArea *area, TipoRegistro *R);
void retiraPrimeiro(TipoArea *area, TipoRegistro *R);
void particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoArea Area, int esq, int dir, int *i, int *j);
void debugger(const char* linha);
void insereItem(TipoRegistro ultimoLido, TipoArea *area);
int obterNumCelOcupadas(TipoArea area);

#endif