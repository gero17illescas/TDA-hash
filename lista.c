#include <stdlib.h>
#include <stdio.h>
#include "lista.h"

/* ****************************************************************
   *                   DEFINICION DE STRUCTS                      *
   **************************************************************** */

struct nodo{
    void* dato;
    struct nodo* prox;
}typedef nodo_t;


struct lista{
    nodo_t* primero;
    nodo_t* ultimo;
    size_t cantidad;
};

struct lista_iter{
    nodo_t* nodo_actual;
    nodo_t* nodo_anterior;
    lista_t* lista;
};


nodo_t* nodo_crear(void* valor){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if(!nodo){
        return NULL;
    }
    nodo->dato = valor;
    nodo->prox = NULL;
    return nodo;
}

/* ****************************************************************
   *                   PRIMITIVAS DE LA LISTA                     *
   **************************************************************** */

lista_t* lista_crear(void){
    lista_t* lista = malloc(sizeof(lista_t));
    if(!lista){
        return NULL;
    }
    lista->primero = NULL;
    lista->ultimo = NULL;
    lista->cantidad = 0;
    return lista;
}

bool lista_esta_vacia(const lista_t* lista){
    return (!lista->primero);
}

bool lista_insertar_primero(lista_t* lista, void* dato){
    nodo_t* nodo = nodo_crear(dato);
    nodo->prox = lista->primero;
    if(!lista->primero){
        lista->ultimo = nodo;
    }
    lista->primero = nodo;
    lista->cantidad++;
    return true;
}

bool lista_insertar_ultimo(lista_t* lista, void* dato){
    nodo_t* nodo = nodo_crear(dato);
    if(!nodo)return NULL;
    if(!lista->primero){
        lista->primero = nodo;
    }else{
        lista->ultimo->prox = nodo;
    }
    lista->ultimo = nodo;
    lista->cantidad++;
    return true;
}

void* lista_borrar_primero(lista_t* lista) {
    if(!lista->primero) return NULL;
    nodo_t *auxiliar = lista->primero;
    lista->primero = lista->primero->prox;
    if (!lista->primero) {
        lista->ultimo = NULL;
    }
    void* dato = auxiliar->dato;
    free(auxiliar);
    lista->cantidad--;
    return dato;
}

void* lista_ver_primero(const lista_t* lista){
    if(!lista->primero) return NULL;
    return lista->primero->dato;
}

void* lista_ver_ultimo(const lista_t* lista){
    if(!lista->ultimo) return NULL;
    return lista->ultimo->dato;
}

size_t lista_largo(const lista_t* lista){
    return lista->cantidad;
}

void lista_destruir(lista_t* lista, void destruir_dato(void*)){
    while(lista->primero){
        void* dato = lista_borrar_primero(lista);
        if(destruir_dato){
            destruir_dato(dato);
        }
    }
    free(lista);
}

/* ****************************************************************
   *                PRIMITIVAS DEL ITERADOR EXTERNO               *
   **************************************************************** */

lista_iter_t* lista_iter_crear(lista_t* lista){
    lista_iter_t* iter = malloc(sizeof(lista_iter_t));
    if(!iter) return NULL;
    iter->lista = lista;
    iter->nodo_actual = lista->primero;
    iter->nodo_anterior = NULL;
    return iter;
}

bool lista_iter_avanzar(lista_iter_t* iter){
    if(!iter->nodo_actual) return false;
    iter->nodo_anterior = iter->nodo_actual;
    iter->nodo_actual = iter->nodo_actual->prox;
    return true;
}

void* lista_iter_ver_actual(const lista_iter_t* iter){
    if(!iter->nodo_actual) return NULL;
    return iter->nodo_actual->dato;
}

bool lista_iter_al_final(const lista_iter_t* iter){
    if(iter->nodo_actual) return false;
    return true;
}

void lista_iter_destruir(lista_iter_t* iter){
    free(iter);
}

bool lista_iter_insertar(lista_iter_t* iter, void* dato){
    if(!iter->nodo_anterior){
        lista_insertar_primero(iter->lista, dato);
        iter->nodo_actual = iter->lista->primero;
        return true;
    }
    if(!iter->nodo_actual){
        lista_insertar_ultimo(iter->lista, dato);
        iter->nodo_actual = iter->lista->ultimo;
        return true;
    }
    nodo_t* nodo = nodo_crear(dato);
    if(!nodo) return false;
    iter->nodo_anterior->prox = nodo;
    nodo->prox = iter->nodo_actual;
    iter->nodo_actual = nodo;
    iter->lista->cantidad++;
    return true;
}

void* lista_iter_borrar(lista_iter_t* iter){
    if(!iter->nodo_actual) return NULL;
    iter->nodo_actual = iter->nodo_actual->prox;
    if(!iter->nodo_anterior){
        return lista_borrar_primero(iter->lista);
    }
    nodo_t* auxiliar = iter->nodo_anterior->prox;
    iter->nodo_anterior->prox = iter->nodo_actual;
    if(iter->nodo_actual == iter->lista->ultimo){
        iter->lista->ultimo = iter->nodo_anterior;
    }
    void* dato = auxiliar->dato;
    iter->lista->cantidad--;
    free(auxiliar);
    return dato;
}

/* ****************************************************************
   *                PRIMITIVAS DEL ITERADOR INTERNO               *
   **************************************************************** */

void lista_iterar(lista_t* lista, bool visitar(void* dato, void* extra), void* extra) {
    nodo_t *actual = lista->primero;
    while (actual && visitar(actual->dato, extra)) {
        actual = actual->prox;
    }
}