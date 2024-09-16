#include "quicksort.h"
#include "lista.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define DEBUG_MODE false

void ListaCria(Lista *lista){
    lista->tam = 0;
    lista->itens = (TipoRegistro*) malloc(TAM_AREA * sizeof(TipoRegistro));
}

void ListaDestroi(Lista* pLista) {
    free(pLista);
    pLista = NULL;
}

bool ListaEhVazia(Lista* pLista) {
    return pLista->tam == 0;
}

// bool ListaInsereFinal(Lista* pLista, TipoRegistro x) {
//     if (pLista->tam == TAM_AREA)
//         return false;
//     pLista->itens[pLista->tam++] = x;
//     insertion(pLista);
//     if(DEBUG_MODE) printf("Inseriu no pivo: %.2f | %ld <<<<<<<<<<<<<<<<<<<<<<<<<<\n", x.nota, x.inscricao);
//     return true;
// }

bool ListaInsereInicio(Lista* pLista, TipoRegistro x) {
    if (pLista->tam == TAM_AREA)
        return false;
    for (int i=pLista->tam;i>0;i--)
        pLista->itens[i] = pLista->itens[i - 1];
    pLista->itens[0] = x;
    pLista->tam++;
    return true;
}

bool ListaRetiraFinal(Lista* pLista, TipoRegistro *pX) {
    if (ListaEhVazia(pLista))
        return false;
    (*pX) = pLista->itens[--pLista->tam];
    return true;
}

bool ListaRetiraInicio(Lista* pLista, TipoRegistro *pX) {
    if (ListaEhVazia(pLista)) {
        return false;
    }

    // Salva o primeiro item da lista para retornar
    (*pX) = pLista->itens[0];

    // Desloca todos os elementos para a esquerda
    for (int i = 0; i < pLista->tam - 1; i++) {
        pLista->itens[i] = pLista->itens[i + 1];
    }

    // Diminui o tamanho da lista
    pLista->tam--;

    return true;
}

void ListaImprime(Lista* pLista) {
    if (ListaEhVazia(pLista)) {
        printf("Pivot vazio. \n");
        return;
    }

    //printf("++++ Lista de Notas: %.2f ", pLista->itens[0].nota);
    printf("Pivot: ");
    for (int i = 0; i < pLista->tam; i++) {
        TipoRegistro reg = pLista->itens[i];
        printf("| %.2f ", reg.nota);
    }
    printf("\n");
}



int ListaTamanho(Lista* pLista) {
    return pLista->tam;
}

void insere(Lista *area, TipoRegistro ultimoLido, int i) {
    // Find the correct position to insert the new item
    while ((i >= 0) && (area->itens[i].nota > ultimoLido.nota)) {
        area->itens[i + 1] = area->itens[i];
        i--;
    }
    // Insert the new item
    area->itens[i + 1] = ultimoLido;
    area->tam++;
}
