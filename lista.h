#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stdio.h>

/* *****************************************************************
   *              DEFINICION DE LOS TIPOS DE DATOS                 *
   ***************************************************************** */

struct lista;
typedef struct lista lista_t;

struct lista_iter;
typedef  struct lista_iter lista_iter_t;

/* ****************************************************************
   *                   PRIMITIVAS DE LA LISTA                     *
   **************************************************************** */
// Crea una lista.
// Post: Devuelve una nueva lista vacia.
lista_t* lista_crear(void);

// Devuelve verdadero o falso, segun si la lista tiene elementos o no.
// Pre: La lista fue creada.
bool lista_esta_vacia(const lista_t* lista);

// Agrega un elemento en la primera posicion de la lista. Devuelve falso en caso de error
// Pre: La lista fue creada
// Post: Se agrego un elemento nuevo al inicio de la lista
bool lista_insertar_primero(lista_t* lista, void* dato);

// Agrega un elemento al final de la lista. Devuelve falso en caso de error,
// Pre: La lista fue creada
// Post: Se agrego un elemento en la ultima posicion de la lista.
bool lista_insertar_ultimo(lista_t* lista, void* dato);

// Saca el primer elemento de la lista y devuelve su valor, si esta vacia, devuelve NULL.
// Pre: La lista fue creada
// Post: Se devolvio el primer elemento de la lista, la lista tiene un elemento menos
// si la lista no estaba vacia
void* lista_borrar_primero(lista_t* lista);

// Obtiene el valor del primer elemento de la lista. Si la lista esta vacia, devuelve NULL.
// Pre: La lista fue creada
// Post: Se devolvio el primer elemento de la lista, cuando no esta vacia
void* lista_ver_primero(const lista_t* lista);

// Obtiene el valor del ultimo elemento de la lista. Si la lista esta vacia, devuelve NULL.
// Pre: La lista fue creada
// Post: Se devuelve el ultimo elemento de la lista, cuando no esta vacia
void* lista_ver_ultimo(const lista_t* lista);

// Obtiene el largo de la lista.
// Pre: La lista fue creada.
// Post: Devuelve el largo de la lista
size_t lista_largo(const lista_t* lista);

// Destruye la lista. Si se recibe la funcion destruir_dato por parametro,
// cada uno de los elementos de la lista llama a destruir_dato.
// Pre: La lista fue creada. destrui_dato es una funcion capaz de destruir.
// Post: Se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t* lista, void destruir_dato(void*));

/* ****************************************************************
   *                PRIMITIVAS DEL ITERADOR EXTERNO               *
   **************************************************************** */
//Crea un iter para una lista
//Pre: Haya una lista creada
//Post: Crea un iterador
lista_iter_t *lista_iter_crear(lista_t* lista);

//Avanza sobre la lista y devuelve verdadero o falso verificando si pudo avanzar
//Pre: El iterador fue creado
//Post: Avanza una posicion el iterador
bool lista_iter_avanzar(lista_iter_t* iter);

//Obtiene el valor del elemento actual.
//Pre: El iter fue creado
void* lista_iter_ver_actual(const lista_iter_t* iter);

//Checkea si esta al final
//Pre: El iter fue creado
bool lista_iter_al_final(const lista_iter_t* iter);

//Destruye el iterador
//Pre: El iter fue creado
void lista_iter_destruir(lista_iter_t* iter);

//Inserta un nodo en la posicion en la que se encuentre
//Pre: El iter fue creado
//Post: Inserto el elemento y el iter se encuentra apuntando a ese elemento
bool lista_iter_insertar(lista_iter_t* iter, void *dato);


//Borre el elemento el cual apunta el iter y devuelve su valor.
//Pre: El iter fue creado
//Post: Se devolvio el elemento y la lista tiene un elemento menos.
void* lista_iter_borrar(lista_iter_t* iter);

/* ****************************************************************
   *                PRIMITIVAS DEL ITERADOR INTERNO               *
   **************************************************************** */

// Itera sobra una lista y aplicar la funcion visitar a cada nodo. Si la funcion devuelve false,
// no avanza mas sobre la lista.
// Pre: La lista fue creada. visitar es una funcion valida capaz de
// modificar el valor dentro del nodo.
// Post: Se modificar los elementos de la lista segun la funcion visitar
void lista_iterar(lista_t *lista, bool visitar(void* dato, void* extra), void* extra);

#endif //LISTA_LISTA_H