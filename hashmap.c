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
      indice=(indice+1) % map->capacity; 
    }
    map->buckets[indice]= createPair(key,value);
    map->current=indice;
    map->size++;
    
  }
  


}
/*Implemente la función void enlarge(HashMap * map). Esta función agranda la capacidad del arreglo buckets y reubica todos sus elementos. Para hacerlo es recomendable mantener referenciado el arreglo *actual/antiguo* de la tabla con un puntero auxiliar. Luego, los valores de la tabla se reinicializan con un nuevo arreglo con el **doble de capacidad**. Por último los elementos del arreglo antiguo se insertan en el mapa *vacío* con el método *insertMap*.
Puede seguir los siguientes pasos:

a - Cree una variable auxiliar de tipo Pair** para matener el arreglo map->buckets (*old_buckets*);

b - Duplique el valor de la variable capacity.

c - Asigne a map->buckets un nuevo arreglo con la nueva capacidad.

d - Inicialice size a 0.

e - Inserte los elementos del arreglo *old_buckets* en el mapa (use la función insertMap que ya implementó).
*/
void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)
  
  if (map == NULL){
    return;
  }
    
  Pair **aux=map->buckets;
  map->capacity *= 2;
  map->buckets=(Pair**)malloc(map->capacity*sizeof(Pair*));
  map->size=0;
    
  for(long i=0;i<map->capacity / 2 ;i++){ 

    if( aux[i] != NULL && aux[i]->key != NULL){ 
      insertMap(map,aux[i]->key,aux[i]->value);
      
    }  
  }
  free(aux);


}


HashMap * createMap(long capacity) {
  HashMap* mapa = (HashMap*)malloc(sizeof(HashMap));
  if(mapa==NULL) return NULL;
   mapa->capacity = capacity;
   mapa->size = 0;
   mapa->current = -1;
   mapa->buckets = (Pair**)calloc(capacity, sizeof(Pair*)); 
   if(mapa->buckets==NULL){ 
     free(mapa);
     return NULL; 
   }
    return mapa;
}

/*//
Implemente la función void eraseMap(HashMap * map,  char * key). Está función elimina el dato correspondiente a la clave key. Para hacerlo debe buscar el dato y luego *marcarlo* para que no sea válido.
**No elimine el par**, sólo invalídelo asignando NULL a la clave (pair->key=NULL).
Recuerde actualizar la variable size.
*/


void eraseMap(HashMap * map,  char * key) { 
if(map==NULL||key==NULL) return ;
long indice=hash(key,map->capacity);
while(map->buckets[indice]!=NULL){
  if(map->buckets[indice]->key!=NULL&&strcmp(map->buckets[indice]->key,key)==0){
    map->buckets[indice]->key=NULL;
    map->size--;
  }
  indice=(indice+1)%map->capacity;
}

}

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
  if(map==NULL)return NULL;
  long cont=0;
  while (map->buckets[cont]==NULL||map->buckets[cont]->key==NULL){
    cont++;
    
  }
  map->current=cont;
  return map->buckets[cont];

  
}

Pair * nextMap(HashMap * map) {
  if(map!=NULL){
    int temp=map->current+1;
    while(temp<map->capacity){
      if(map->buckets[temp]!=NULL&&map->buckets[temp]->key!=NULL){ 
        map->current=temp;
        return map->buckets[temp];
      }
      temp++;
    }
    
  }
  return NULL;
}
