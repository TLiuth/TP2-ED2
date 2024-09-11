#include "quicksort.h"
#include "lista.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

Lista* ListaCria() {
    Lista* lista = (Lista*) malloc(sizeof(Lista));
    if (lista == NULL)
        exit(1);
    lista->tam = 0;
    return lista;
}

void ListaDestroi(Lista** pLista) {
    free(*pLista);
    *pLista = NULL;
}

bool ListaEhVazia(Lista* pLista) {
    return pLista->tam == 0;
}

bool ListaInsereFinal(Lista* pLista, TipoRegistro x) {
    if (pLista->tam == TAM_AREA)
        return false;
    pLista->itens[pLista->tam++] = x;
    insertion(pLista);
    return true;
}

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



bool ListaGet(Lista* pLista, int p, TipoRegistro *pX) {
    if (p >= ListaTamanho(pLista) || p < 0 )
        return false;
    (*pX) = pLista->itens[p];
    return true;
}

int ListaTamanho(Lista* pLista) {
    return pLista->tam;
}

void insertion(Lista *v) {
    for(int i = 1; i < v->tam; i++) {
        TipoRegistro aux = v->itens[i];
        int j = i - 1;
        while(j >= 0 && aux.nota < v->itens[j].nota) {
            v->itens[j + 1] = v->itens[j];
            j--;
        }
        v->itens[j + 1] = aux;
    }
}
