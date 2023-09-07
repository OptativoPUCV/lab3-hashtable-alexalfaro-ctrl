#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}

/*
2.- Implemente la función void insertMap(HashMap * map, char * key, void * value). Esta función inserta un nuevo dato (key,value) en el mapa y actualiza el índice current a esa posición.

a - Aplicar la función hash a la clave para obtener la posición donde debería insertar el nuevo par

b - Si la casilla se encuentra ocupada, avance hasta una casilla disponible (método de resolución de colisiones). Una casilla disponible es una casilla nula, pero también una que tenga un par inválido (key==NULL).

c - Ingrese el par en la casilla que encontró.

listo 100%
*/
void insertMap(HashMap * map, char * key, void * value) {
  if(map==NULL||key==NULL)return;
  long indice=hash(key,map->capacity);
  
  if(map->buckets[indice]==NULL){
    map->buckets[indice]= createPair(key,value);
    map->current=indice;
    map->size++;
  } else { 
    while (map->buckets[indice]!=NULL&&map->buckets[indice]->key!=NULL){
      if(strcmp(map->buckets[indice]->key,key)==0){
        return;
      }
      indice=(indice+1) % map->capacity; //me piro 
    }
    map->buckets[indice]= createPair(key,value);
    map->current=indice;
    map->size++;
    
  }
  


}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)


}


HashMap * createMap(long capacity) {
  HashMap* mapa = (HashMap*)malloc(sizeof(HashMap));
  if(mapa==NULL) return NULL;
   mapa->capacity = capacity;
   mapa->size = 0;
   mapa->current = -1;
   mapa->buckets = (Pair**)calloc(capacity, sizeof(Pair*)); //usa calloc aqui 
   if(mapa->buckets==NULL){ 
     free(mapa);
     return NULL; 
   }
    return mapa;
}

/*//no cachaba ese protip
malloc(argumento1 * sizeof(argumento2)); -> inicializa con datos basura
calloc(argumento1 , sizeof(argumento2)); -> inicializa con 0s
*/


void eraseMap(HashMap * map,  char * key) {    


}
/*/3.- Implemente la función Pair * searchMap(HashMap * map,  char * key), la cual retorna el **Pair** asociado a la clave ingresada. 
Recuerde que para buscar el par debe:

a - Usar la función hash para obtener la posición donde puede encontrarse el paIr con la clave

b - Si la clave no se encuentra avance hasta encontrarla (*método de resolución de colisiones*)

c - Si llega a una casilla nula, retorne NULL inmediatamente (no siga avanzando, la clave no está)

Recuerde actualizar el índice current a la posición encontrada.
Recuerde que el arreglo es **circular**.
*/
Pair * searchMap(HashMap * map,  char * key) {  
  if (map==NULL||key==NULL) return NULL; 
  long indice=hash(key,map->capacity);
  if(strcmp(map->buckets[indice]->key,key)==0){
    map->current=indice;
    return map->buckets[indice];
    
  }else{
    while(map->buckets[indice]!=NULL){
      if(strcmp(map->buckets[indice]->key,key)==0){
        map->current=indice;
        return map->buckets[indice];
      }
      indice=(indice+1)%map->capacity;
    }
    return NULL;
  }
    return NULL;
}

Pair * firstMap(HashMap * map) {

    return NULL;
}

Pair * nextMap(HashMap * map) {

    return NULL;
}
