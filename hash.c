#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "testing.h"
#include "lista.h"
#define TAM_INICIAL 10
#define COEF_REDIM 2
/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

struct campo_hash{
	char* clave;
	void* valor;
}typedef campo_hash_t;

struct hash{
	lista_t** tabla;
	size_t tam; //(m que es la capacidad maxima de la estructura)
	size_t cant; //(n que es la cantidad de elementos que esta en el hash)
	hash_destruir_dato_t destruir;
}typedef hash_t;

struct hash_iter{
	const hash_t* hash;
	lista_iter_t* lista_iter;
	size_t pos;
	size_t iterados;
}typedef hash_iter_t;

bool hash_redimensionar(hash_t* hash, size_t tam_nuevo);

/* *****************************************************************
 *                DEFINICION DE FUNCIONES AUXILIARES
 * *****************************************************************/


// tipo de función para destruir dato
typedef void (*hash_destruir_dato_t)(void *);

size_t funcion_hash(const char* s, size_t hash_tam){
	size_t hashvalue;
	for(hashvalue = 0; *s != '\0';s++)
		hashvalue = *s + 11 * hashvalue;
	return hashvalue % hash_tam;
}

campo_hash_t* crear_campo_hash(const char* clave, void* dato){
	campo_hash_t* campo_hash = malloc(sizeof(campo_hash_t*));
	if(!campo_hash) return NULL;
	campo_hash->clave = malloc(sizeof(const char)* strlen(clave)+1);
	strcpy(campo_hash->clave, clave);
	campo_hash->valor = dato;
	return campo_hash;
}

bool crear_tabla(hash_t* hash){
	hash->tabla = malloc(sizeof(lista_t*)* hash->tam);
	if(!hash->tabla){
		free(hash);
		return false;
	}
	for(int i = 0; i < hash->tam; i++){
		hash->tabla[i] = lista_crear();
		if(!hash->tabla[i]){
			free(hash->tabla);
			free(hash);
			return false;
		}
	}
	return true;
}

// Recibe un puntero a un struct hash y busca en el campo hash cuya 
// clave asignada sea la recibida por parametro, si tal campo no se 
// encontro devuelve NULL.
// Pre: el hash fue creado
// Post: Devuelve el campo si fue encontrado
campo_hash_t* buscar_campo_hash(const hash_t *hash, const char *clave){
	size_t indice = funcion_hash(clave, hash->tam);
    lista_iter_t* iter = lista_iter_crear(hash->tabla[indice]);
	campo_hash_t* registro = lista_iter_ver_actual(iter);
	while(!lista_iter_al_final(iter)){
		if(strcmp(registro->clave,clave)==0){
			free(iter);
			return registro;
		}
		lista_iter_avanzar(iter);
	}
	free(iter);
	return NULL;
}	

/* *****************************************************************
 *                    PRIMITIVAS DEL HASH
 * *****************************************************************/


/* Crea el hash
 */
hash_t* hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash = malloc(sizeof(hash_t*));
	if (!hash) return NULL;
	if (!crear_tabla(hash)) return NULL;
	if(destruir_dato){
		hash->destruir = destruir_dato;
	}else{
		hash->destruir = NULL;
	}
	hash->tam = TAM_INICIAL;
	hash->cant = 0;
	return hash;
}

/* Guarda un elemento en el hash, si la clave ya se encuentra en la
 * estructura, la reemplaza. De no poder guardarlo devuelve false.
 * Pre: La estructura hash fue inicializada
 * Post: Se almacenó el par (clave, dato)
 */
bool hash_guardar(hash_t *hash, const char *clave, void *dato){
	if(hash->cant/hash->tam >= 0.7){
		hash_redimensionar(hash, hash->tam*COEF_REDIM);
	}
	size_t indice = funcion_hash(clave, hash->tam);
	campo_hash_t* campo_nuevo = crear_campo_hash(clave, dato);
	if(!campo_nuevo) return false;
	campo_hash_t* campo_existe = buscar_campo_hash(hash, clave);
	if (!campo_existe){
		if(!lista_insertar_ultimo(hash->tabla[indice], campo_nuevo)){
			return false;
		}
		return true;
	}
	campo_existe->valor = campo_nuevo->valor;
	if(hash->destruir){
		hash->destruir(campo_existe->valor);
	}
	free(campo_nuevo);
	return true;
}


/* Borra un elemento del hash y devuelve el dato asociado.  Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura hash fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void* hash_borrar(hash_t *hash, const char *clave){
	if(hash->cant/hash->tam <= 0.3 && hash->tam >= TAM_INICIAL){
	   hash_redimensionar(hash, hash->tam/COEF_REDIM);
	}
	size_t indice = funcion_hash(clave, hash->tam);
	lista_iter_t* iter = lista_iter_crear(hash->tabla[indice]);
	campo_hash_t* registro = lista_iter_ver_actual(iter);
	while(registro){
		if(strcmp(registro->clave, clave) == 0){
			campo_hash_t* aux = lista_iter_borrar(iter);
			void* dato = aux->valor;
			free(aux->clave);
			free(aux);
			hash->cant--;
			free(iter);
			return dato;
		}
		lista_iter_avanzar(iter);
		registro = lista_iter_ver_actual(iter);
	}
	free(iter);
	return NULL;
}

bool hash_redimensionar(hash_t* hash, size_t tam_nuevo){
	lista_t** tabla_nueva = malloc(sizeof(lista_t*) * tam_nuevo);
	if(!tabla_nueva) return false;
}

/* Obtiene el valor de un elemento del hash, si la clave no se encuentra
 * devuelve NULL.
 * Pre: La estructura hash fue inicializada
 */
void* hash_obtener(const hash_t *hash, const char *clave){
	campo_hash_t* auxiliar = buscar_campo_hash(hash, clave);
	if(!auxiliar) return NULL;
	void* dato = auxiliar->valor;
	free(auxiliar->clave);
	free(auxiliar);
	return dato;
}

/* Determina si clave pertenece o no al hash.
 * Pre: La estructura hash fue inicializada
 */
bool hash_pertenece(const hash_t *hash, const char *clave){
	return !buscar_campo_hash(hash, clave);

}

/* Devuelve la cantidad de elementos del hash.
 * Pre: La estructura hash fue inicializada
 */
size_t hash_cantidad(const hash_t* hash){
	return hash->cant;
}


/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir para cada par (clave, dato).
 * Pre: La estructura hash fue inicializada
 * Post: La estructura hash fue destruida
 */
void hash_destruir(hash_t *hash){
	for(int i = 0; i < hash->tam; i++){
		lista_iter_t* iter_lista = lista_iter_crear(hash->tabla[i]);
		campo_hash_t* campo_hash = lista_iter_borrar(iter_lista);
		while(campo_hash){
			if(hash->destruir){
				hash->destruir(campo_hash->valor);
			}
			free(campo_hash->valor);
			free(campo_hash);
		}
		lista_iter_destruir(iter_lista);
		lista_destruir(hash->tabla[i], free);
	}
	free(hash->tabla);
	free(hash);
}

void hash_destruir(hash_t *hash){
	for(int i = 0; i < hash->tam; i++){
		campo_hash_t* campo_hash = lista_borrar_primero(hash->tabla[i]);
		while(campo_hash){
			if(hash->destruir){
				hash->destruir(campo_hash->valor);
			}
			free(campo_hash->valor); //Ya deberia estar liberado
			free(campo_hash->clave);
			free(campo_hash);
			campo_hash = lista_borrar_primero(hash->tabla[i]);
		}
		lista_destruir(hash->tabla[i], free);
	}
	free(hash->tabla);
	free(hash);
}

/* Iterador del hash */

/* Crea iterador. Asigna el iterador al primer elemento de la tabla de
 * hash cuya lista no sea vacia. Si todas las listas estan vacias hace
 * referencia a NULL.
 * Pre: el hash fue creado.
 */
hash_iter_t* hash_iter_crear(const hash_t *hash){
	hash_iter_t* iter =  malloc(sizeof(hash_iter_t));
	if (!iter)     return NULL;

	iter -> hash = hash;
	iter -> iterados = 0;
	iter -> lista_iter = NULL;

	int i = 0;
	while(lista_esta_vacia(hash->tabla[i])){
		i++;
	}

	iter -> lista_iter = lista_iter_crear(hash->tabla[i]);
	iter -> pos = i;

	return iter;
}

/* Avanza iterador sobre un mismo
 * Pre: el iterador fue creado.
 */
   
bool hash_iter_avanzar(hash_iter_t *iter){
	//Si llegamos al final de una lista de un elemento de la tabal 
	//vamos l siguiente elemento
	if(hash_iter_al_final(iter)) return false;
	if(lista_iter_al_final(iter->lista_iter)){
		size_t i = iter->pos;
		lista_iter_destruir(iter->lista_iter);
		while (i<iter->hash->tam && lista_esta_vacia(iter->hash->tabla[i])){
			i++;
		}
		iter -> lista_iter = lista_iter_crear(iter->hash->tabla[i]);
		return true;
	}
	lista_iter_avanzar(iter-> lista_iter);
	return true;

}

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char* hash_iter_ver_actual(const hash_iter_t *iter){
	if(!iter) return NULL;
	campo_hash_t* actual = lista_iter_ver_actual(iter->lista_iter);
	return actual->clave;
}
const char* hash_iter_ver_actual(const hash_iter_t *iter);

/* Comprueba si existen nodos siguientes en la lista actual o si
 * existen elemetos de la tabla hash no vacios. De esta manera si hay
 * algun elemento siguiente es posible iterar o no.
*/
bool hash_iter_al_final(const hash_iter_t *iter){
	size_t i = iter->pos;
	if(!lista_iter_al_final(iter->lista_iter)) return false;
	while (i<iter->hash->tam && lista_esta_vacia(iter->hash->tabla[i])){
		i++;
	}

	return i==iter->hash->tam;
}

// Destruye iterador
void hash_iter_destruir(hash_iter_t* iter){
	if (iter->lista_iter){
		lista_iter_destruir(iter->lista_iter);
	}
	free(iter);
}
