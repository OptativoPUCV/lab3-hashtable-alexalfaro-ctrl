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
  if(map == NULL || key == NULL) return;// primero se verifica si el mapa y la llave son nulos
  
  long indice = hash(key, map->capacity);//se calcula el indice con la funcion hash
  if(map->buckets[indice] == NULL){ //si el indice esta vacio
    
    map->buckets[indice] = createPair(key,value);// se agrega el pair o bucket
    map->current = indice;// se actualiza el indice
    map->size++; // se aumenta el tamaño del mapa
    
  } else { 
    
    while (map->buckets[indice] != NULL && map->buckets[indice]->key != NULL){
      
      if(strcmp(map->buckets[indice]->key,key)==0){// si la clave ya existe no se hace nada
        return;
        
      }
      
      indice = (indice+1) % map->capacity; // se aumenta el indice
    }
    
    map->buckets[indice] = createPair(key,value);
    map->current = indice;
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
  
  Pair **aux=map->buckets; // aux representa **old_buckets**
  map->capacity *= 2;// se aumenta la capcidad al doble
  map->buckets = (Pair**)malloc(map->capacity*sizeof(Pair*)); //se crea el nuevo arreglo con la nueva capacidad
  map->size = 0;// se inicializa size en 0
    
  for(long i = 0; i < map->capacity/2 ; i++){  // se recorre el mapa hasta la mitad para insertar los datos anterioes

    if( aux[i] != NULL && aux[i]->key != NULL){ 
      insertMap(map, aux[i]->key, aux[i]->value);//se insertan los valores
      
    }  
  }
  free(aux); 

}


HashMap * createMap(long capacity) {
  HashMap* mapa = (HashMap*)malloc(sizeof(HashMap));//se hace la reserva de memoria para la estructura
  
  if(mapa==NULL) return NULL;
  
   mapa->capacity = capacity; // se inicializan
   mapa->size = 0;
   mapa->current = -1;
   mapa->buckets = (Pair**)calloc(capacity, sizeof(Pair*)); // se inicializan los espacios con 0
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
if (map==NULL || key==NULL) return; //si el mapa o la llave son nulas se retorna
  
long indice = hash(key, map->capacity); // calculo de indice con la funcion hash

  while ( map->buckets[indice] != NULL ){ // si el indice es distinto de null,se recorre el mapa para buscar el elemento a eliminar
  
  if((map->buckets[indice]->key != NULL) && (strcmp(map->buckets[indice]->key,key)==0)){ // si el bucket no esta vacio y las keys son iguales
                                                                                          //encontramos el elemento que queremos borra
    map->buckets[indice]->key = NULL;  //se marca como null
    map->size--;// se reduce el espacio
  }
  
  indice = (indice+1)%map->capacity;// se aumenta el indice
}

}


Pair * searchMap(HashMap * map,  char * key) {  
  if (map==NULL || key==NULL) return NULL; //si el mapa o la llave son nulas se retorna
  
  long indice = hash(key,map->capacity);//calculo de l indice por la funcion hash
  

    while(map->buckets[indice]!=NULL){
      
      if(strcmp(map->buckets[indice]->key,key)==0){  
        map->current = indice;
        return map->buckets[indice];
      }
      
      indice=(indice+1)%map->capacity;
    }
    return NULL;
  }



Pair * firstMap(HashMap * map) {
  if (map == NULL) return NULL;//si el mapa  se retorna null
  
  long cont = 0;
  while (map->buckets[cont]==NULL || map->buckets[cont]->key==NULL){
    cont++;
  }
  
  map->current = cont;
  return map->buckets[cont];

  
}

Pair * nextMap(HashMap * map) {
  if(map != NULL){//si el mapa es distinto de NULL entra ala condicion si no retorna null
    
    int temp = map->current+1;
    while(temp<map->capacity){  
      if(map->buckets[temp] != NULL && map->buckets[temp]->key != NULL){ 
        
        map->current = temp;
        return map->buckets[temp];
      }
      temp++;
    }
    
  }
  return NULL;
}
