//Victoria Severin
//1539768
//CS12B
//March 16, 2018
//Dictionary.c
//recreate the Dictionary ADT from pa3 and lab5 in c
//based on a hash table instead of a linked list


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Dictionary.h"

const int tableSize=5; // or some prime other than 101

//NodeObj
typedef struct NodeObj {
    char* key;
    char* value;
    struct NodeObj* next;
} NodeObj;

//Node
typedef NodeObj* Node;

//newNode() constructor for Node type
Node newNode(char* k, char* v) {
   Node N = malloc(sizeof(NodeObj));
   assert(N != NULL);
   N->key = k;
   N->value = v;
   N->next = NULL;
   return(N);
}

//SlotObj
typedef struct SlotObj {
    Node head;
    Node tail;
    int numItems;
} SlotObj;

typedef struct SlotObj* Slot;

//constructor for Slot type
Slot newSlot(void) {
   Slot S = malloc(sizeof(SlotObj));
   assert(S != NULL);
   S->head = NULL;
   S->tail = NULL;
   S->numItems = 0;
   return S;
}

// isSlotEmpty()
// returns 1 (true) if S is empty, 0 (false) otherwise
// call stderr if Slot is null
// otherwise return false if the number of items in S are greater than 0
// return true if the number of items in S are not greater than 0 (empty)
int isSlotEmpty(Slot S) {
   if( S == NULL ){
      fprintf(stderr,  "Dictionary  Error: calling isEmpty() on NULL Slot reference\n");
      exit(EXIT_FAILURE);
   }
   if (S->numItems > 0) {
      return 0;
   }
   return 1;
}

// emptySlot()
// re-sets S to the empty state.
// if head is null and tail is null then numItems changes to 0
void emptySlot(Slot S){
   S->head = NULL;
   S->tail = NULL;
   S->numItems = 0;
}

// insertInSlot()
// inserts pair (k,v) in S
// 1. check if S is empty,
// 2. if empty, (k,v) is added as the first pair in the Nodes
// 3. otherwise, add it to the existing Nodes
void insertInSlot(Slot S, char* k, char* v) {
   Node N = newNode(k, v);
   if (isSlotEmpty(S)) {
      S->head = S->tail = N;
   } else {
      S->tail->next = N;
      S->tail = N;
   }
   S->numItems++;
}

//lookupInSlot()
//returns the value v such that (k,v) is in S
// or returns NULL if no such value v exists
char* lookupInSlot(Slot S, char* k){
   if( S == NULL ){
      fprintf(stderr, "Dictionary Error: calling lookup() on NULL Slot\n");
      exit(EXIT_FAILURE);
   }

   Node N = S->head;

   while(N != NULL) {
      if(strcmp(N->key,k)== 0)
         return N->value;
      N = N->next;
   }
   return NULL;
}

// deletes pair with the key k
// if the Slot currently contains a pair whose key field matches the argument
// key, then that pair is removed from the Slot.
// If no such pair exists then stderr is called with a message of
// "key not found"
void deleteFromSlot(Slot S, char* k) {
   //check if list is empty
   if( lookupInSlot(S, k) == NULL ) {
      fprintf(stderr, "Dictionary error: key not found\n");
      exit(EXIT_FAILURE);
   }

   Node N = S->head;
   while(N != NULL) {
      // check if the node of interest is head
      if (N == S->head) {
         if (strcmp(N->key, k) == 0) {
            S->head = N->next;
            if (S->head == NULL)
               S->tail = NULL;
            free(N);
            S->numItems--;
            break;
         }
      } else {
         // else check for the next one
         // if it's a match, re-connect current with the next of next
         // and only then delete next
         if (N->next != NULL) {
            if (strcmp(N->next->key, k) == 0) {
               Node P = N->next;
               N->next = N->next->next;
               free(P);
               S->numItems--;
               break;
            }
         }
      }
      N = N->next;
   }
}

//DictionaryObj
typedef struct DictionaryObj {
    Slot slots[tableSize];
} DictionaryObj;

//newDictionary()
//constructor for Dictionary type
Dictionary newDictionary(void) {
   Dictionary D = malloc(sizeof(DictionaryObj));
   for (int i = 0; i < tableSize; i++) {
      D->slots[i] = newSlot();
   }
   assert(D != NULL);
   return D;
}

// freeDictionary()
// destructor for the Dictionaty type
void freeDictionary(Dictionary* pD) {
   if( pD != NULL && *pD != NULL ) {
      if( !isEmpty(*pD) ) {
         makeEmpty(*pD);
      }
      free(*pD);
      *pD = NULL;
   }
}

// isEmpty()
// returns 1 (true) if S is empty, 0 (false) otherwise
// call stderr if Dictionary is null
// otherwise return false if the number of items in D are greater than 0
// return true if the number of items in D are not greater than 0 (empty)
int isEmpty(Dictionary D) {
   if( D == NULL ){
      fprintf(stderr,  "Dictionary  Error: calling isEmpty() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }
   for (int i = 0; i < tableSize; i++) {
      if (D->slots[i]->numItems > 0)
         return 0;
   }
   return 1;
}

//size()
//returns the number of(key,value) pairs in D
int size(Dictionary D) {
   int size = 0;
   for (int i = 0; i < tableSize; i++ ) {
      size += D->slots[i]->numItems;
   }
   return size;
}

//lookup()
//returns the value v such that (k,v) is in D
// or returns NULL if no such value v exists
char* lookup(Dictionary D, char* k){
    if( D == NULL ){
        fprintf(stderr, "Dictionary Error: calling lookup() on NULL Dictionary\n");
        exit(EXIT_FAILURE);
    }
   return lookupInSlot(D->slots[hash(k)], k);
}

// insert()
// in the Dictionary find the Slot with the index = h(k)
// insert pair (k,v) into that slot.
void insert(Dictionary D, char* k, char* v){
   insertInSlot(D->slots[hash(k)], k, v);
}

//makeEmpty()
// re-sets D to the empty state.
void makeEmpty(Dictionary D){
   if( D == NULL ){
      fprintf(stderr, "Dictionary Error: calling lookup() on NULL Dictionary\n");
      exit(EXIT_FAILURE);
   }

   for (int i = 0; i < tableSize; i++) {
      emptySlot(D->slots[i]);
   }
}

// deletes pair with the key k
// pre: lookup(D, k)!=NULL
// if the Dictionary currently contains a pair whose key field matches the argument
// key, then that pair is removed from the Dictionary.
// If no such pair exists then stderr is called with a message of
// "key not found"
void delete(Dictionary D, char* k) {
   //check if list is empty
   if( lookup(D,k) == NULL ) {
         fprintf(stderr, "Dictionary error: key not found\n");
         exit(EXIT_FAILURE);
   }
   deleteFromSlot(D->slots[hash(k)], k);
}

// printDictionary()
// prints a text representation of D to the file pointed to by out
void printDictionary(FILE* out, Dictionary D) {
   if( D == NULL ){
      fprintf(stderr, "Dictionary Error: calling printDictionary() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }

   for (int i=0; i< tableSize; i++) {
      Slot S = D->slots[i];
      if( S == NULL ){
         fprintf(stderr, "Dictionary Error: calling printDictionary() on NULL Slot reference\n");
         exit(EXIT_FAILURE);
      }
      for(Node N = S->head; N != NULL; N = N->next)
         fprintf(out, "%s %s \n" , N->key, N->value);
   }
}

// rotate_left()
// rotate the bits in an unsigned int
unsigned int rotate_left(unsigned int value, int shift) {
   int sizeInBits = 8*sizeof(unsigned int); shift = shift & (sizeInBits - 1);
   if ( shift == 0 )
      return value;
   return (value << shift) | (value >> (sizeInBits - shift));
}

// pre_hash()
// turn a string into an unsigned int
unsigned int pre_hash(char* input) {
   unsigned int result = 0xBAE86554;
   while (*input) {
      result ^= *input++;
      result = rotate_left(result, 5);
   }
   return result;
}

// hash()
// turns a string into an int in the range 0 to tableSize-1
int hash(char* key){
   return pre_hash(key)%tableSize;
}
