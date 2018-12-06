//Victoria Severin
//1539768
//March 16, 2018
//DictionaryTest.c
//Test procedures 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Dictionary.h"


int main(int argc, char* argv[]) {
   Dictionary A = newDictionary();
   char* k;
   char* v;
   char* word1[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
   char* word2[] = {"honda", "acura", "toyota", "ford", "nissan", "bmw", "lexus", "vw", "mercedes"};

   for(int i=0; i < 8; i++) {
      insert(A, word1[i], word2[i]);
   }

   printDictionary(stdout,A);

   for(int i = 0; i < 4; i++){
      k = word1[i];
      v = lookup(A, k);
      printf("key=\"%s\" %s\"%s\"\n", k, (v==NULL?"not found ":"value="), v);
   }


   delete(A, "one");
   delete(A, "two");
   delete(A, "eight");

   printDictionary(stdout, A);

   insert(A, "one", "meow");
   insert(A, "two", "bark");

   printf("\nThis is the re-inserted list: \n");
   printDictionary(stdout, A);


   for(int i = 0; i < 4; i++){
      k = word1[i];
      v = lookup(A, k);
      printf("key=\"%s\" %s\"%s\"\n", k, (v==NULL?"not found ":"value="), v);
   }

   printf("%s\n", (isEmpty(A)?"true":"false"));
   printf("%d\n", size(A));

   makeEmpty(A);
   printf("%s\n", (isEmpty(A)?"true":"false"));

   freeDictionary(&A);

   return(EXIT_SUCCESS);
}
