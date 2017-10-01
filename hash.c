#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "lista.h"
#define TAM_INICIAL 10

/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

struct campo_hash{
	char* clave; (siempre cadena)
	void* valor;
};
struct hash{
	campo_hash** tabla;
	size_t tam; (m que es la capacidad maxima de la estructura)
	size_t cant; (n que es la cantidad de elementos que esta en el hash)
};
struct hash_iter{
	lista_t *hash; 
	lista_iter* iter = NULL;
	size_t* pos_actual;

}

typedef struct hash hash_t;
typedef struct campo_hash campo_hash_t;
typedef struct hash_iter hash_iter_t;

/* *****************************************************************
 *                DEFINICION DE FUNCIONES AUXILIARES
 * *****************************************************************/


// tipo de función para destruir dato
typedef void (*hash_destruir_dato_t)(void *);

campo_hash_t* crear_campo_hash(const char* clave, void* dato){
	campo_hash_t* campo_hash = malloc(sizeof(campo_hash_t*)
	if (!campo_hash) return NULL;
	campo_hash -> clave = strc(malloc(strlen(clave)+1),clave);
	campo_hash -> dato = dato;
	return campo_hash;
}
lista_t** crear_tabla(int tamaño){
	lista_t** tabla = malloc(sizeof(lista_t*)*TAM_INICIAL);
	if (!tabla){
		return NULL;
	}
	for(int i = 0; i < tamaño;i++){
		tabla[i] = lista_crear();
	}
	return tabla;

}
// Recibe un puntero a un struct hash y busca en el campo hash cuya 
// clave asignada sea la recibida por parametro, si tal campo no se 
// encontro devuelve NULL. Si bool borrar es true, elimina la refer
// encia el campo hash de la lista correspodiente.
// Pre: el hash fue creado
campo_hash_t* obtener_campo_hash(hash_t *hash, const char *clave, bool borrar){
	int indice = fhash(clave);
	campo_hash_t* registro = NULL;
	lista_iter* iter = lista_iter_crear(hash->tabla[indice]);
	
	while (!lista_iter_al_final(iter)){
		registro = lista_iter_ver_actual(iter);
		if (strcmp(registro->clave,clave)==0){
			if(borrar){
				lista_iter_borrar(iter);
				hash->cant--;
			}
			break;
		}
		lista_iter_avanzar(iter);
	}

	lista_iter_borrar(iter);
	return registro;
}	
int funcion_hash(const char* clave){

}
/* *****************************************************************
 *                    PRIMITIVAS DEL HASH
 * *****************************************************************/


/* Crea el hash
 */
hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t*  hash = malloc(sizeof(hash_t*))
	if (!hash){
		return NULL;
	}
	lista_t** tabla = generar_tabla(),
	if (!tabla){
		free(hash);
		return NULL;
	}
	hash -> destructor = destruir_dato;
	hash -> tabla = crear_tabla();
	hash -> tam = TAM_INICIAL;
	hash -> cant = 0;
	return hash;
}

/* Guarda un elemento en el hash, si la clave ya se encuentra en la
 * estructura, la reemplaza. De no poder guardarlo devuelve false.
 * Pre: La estructura hash fue inicializada
 * Post: Se almacenó el par (clave, dato)
 */
bool hash_guardar(hash_t *hash, const char *clave, void *dato){
	int indice = fhash(clave);
	if (hash_pertenece(hash,clave)){

	}
	lista_insertar_ultimo(hash->tabla[indice],crear_campo_hash(clave,dato));
}


/* Borra un elemento del hash y devuelve el dato asociado.  Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura hash fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void *hash_borrar(hash_t *hash, const char *clave){
	campo_hast_t* registro = obtener_campo_hash(hash,clave,true);
	if (!registro) return NULL;
	void* dato = registro->dato;
	free(registro->clave);
	free(registro);
	return dato;
}

/* Obtiene el valor de un elemento del hash, si la clave no se encuentra
 * devuelve NULL.
 * Pre: La estructura hash fue inicializada
 */
void *hash_obtener(const hash_t *hash, const char *clave);

/* Determina si clave pertenece o no al hash.
 * Pre: La estructura hash fue inicializada
 */
bool hash_pertenece(const hash_t *hash, const char *clave){
	return obtener_campo_hash(hash,clave,false)==NULL;

}

/* Devuelve la cantidad de elementos del hash.
 * Pre: La estructura hash fue inicializada
 */
size_t hash_cantidad(const hash_t *hash);

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir para cada par (clave, dato).
 * Pre: La estructura hash fue inicializada
 * Post: La estructura hash fue destruida
 */
void hash_destruir(hash_t *hash);

/* Iterador del hash */

// Crea iterador
hash_iter_t *hash_iter_crear(const hash_t *hash);

// Avanza iterador
bool hash_iter_avanzar(hash_iter_t *iter);

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char *hash_iter_ver_actual(const hash_iter_t *iter);

// Comprueba si terminó la iteración
bool hash_iter_al_final(const hash_iter_t *iter);

// Destruye iterador
void hash_iter_destruir(hash_iter_t* iter);
