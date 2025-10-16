#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

//Word Data Struct (10 pts)

  //these are the entries in the list of possible next words
  typedef struct Bigram{
    int bigramFreq;
    struct Bigram* nextNode;
    char* str;
  }Bigram;

  //I (asa) removed the Word Word_Data class because I just think it's kind of unintuitive and unnecessary

  //linked list of possible words that occur after a given word
  typedef struct BigramList{
    Bigram* head;
  }BigramList;

  //these are the words that are in the hash table. also functions as a node in the bucket linked list. Word needs to go at the end so that it can have a BigramList in it.
  typedef struct Word{
    int textFreq;
    BigramList bigramList;
    struct Word* nextInBucket;
    char* str; //compiler wants strings to be at the end of struct since they have flexible size
  }Word;

//Hash bucket table(10 points)
  
  
  //linked list of all the words with a given hash
  typedef struct Bucket{
    Word* head;
  }Bucket;

  //Hash Table Struct (5 pts)
  typedef struct HashTable{
    Bucket* array;
    int arrSize;
    int wordCount; //if you are implementing anything that involves adding new Word struct entries to the hash table, PLEASE UPDATE THIS. it makes resizing much easier.
    float loadFactor;
  }HashTable;

//Empty Hash Table (5 pts)

  //pretty inefficient prime number generator. if anyone else can think of a better way to implement this, please do.
  int prime_generator(int minValue){
    int currValue = minValue;
    if (currValue % 2 == 0){
      currValue++; //makes sure it's always starting with an odd number
    }
    int hasDivisor = 0;

    while (1) {
      hasDivisor = 0;
      for (int i = 2; i <= (currValue / 2); i++){
        if (currValue % i == 0){
          hasDivisor = 1;
          break;
        }
      }
      if (hasDivisor == 0){
        return currValue;
      }
      else{
        currValue += 2; //increments by two so we don't have to bother checking even numbers
      }
    }
  }

  HashTable* hash_table(int capacity, float loadFactor){
    HashTable* newHashTable = (HashTable*) malloc(sizeof(HashTable));

    int arraySize = prime_generator(capacity);
    newHashTable->array = (Bucket*) malloc(sizeof(Bucket) * arraySize);

    newHashTable->arrSize = arraySize;
    newHashTable->loadFactor = loadFactor;
    newHashTable->wordCount = 0;

    Bucket emptyBucket;
    emptyBucket.head = NULL;
    for (int i = 0; i < arraySize; i++){
      (newHashTable->array)[i] = emptyBucket;
    }

    return newHashTable;
  }


//Hash Functions (15 pts)

  int hash(int size, char string[]){
    //silly numbers taken from the instructions!
    uint64_t offset = 14695981039346656037ULL; //wouldn't compile because the offset was too big to be an integer, even though it's not supposed to be one. adding 'ULL' at the end fixes this.
    uint64_t prime = (uint64_t) 1099511628211;
    uint64_t binaryChar = 0;

    uint64_t hashValue = offset;
    for (int i = 0; i < (int) strlen(string); i++){
      binaryChar = (uint64_t) ((unsigned char) string[i]);
      hashValue = (hashValue ^ binaryChar) * prime;
    }

    return (int) (hashValue % size);
  }

//Add (20 pts)

  void resize(HashTable* hashTable);

  void add(char word1[], char word2[], HashTable* hashTable){
    //taking out all of the variables from our HashTable struct for convencience of coding
    int tableSize = hashTable->arrSize;
    Bucket* array = hashTable->array;
    float loadFactor = hashTable->loadFactor;

    //resize if exceeding load factor
    if (hashTable->wordCount > (tableSize * loadFactor)){
      resize(hashTable);
    }

    //gets hash value for word1, searches for it in its bucket
    int index = hash(tableSize, word1);
    Word* currWord = (array[index]).head;
    Word* prevWord = NULL;
    while (currWord != NULL){
      if (strcmp(currWord->str, word1) == 0){
        break;
      }
      prevWord = currWord;
      currWord = currWord->nextInBucket;
    }
    Word* parentWord = currWord;

    //if it could not find word1 in the bucket, create a new Word struct for it
    if (parentWord == NULL){
      Word* newWord = malloc(sizeof(Word));
      newWord->str = strdup(word1);
      newWord->textFreq = 0;
      newWord->nextInBucket = NULL;
      (newWord->bigramList).head = NULL;

      //places the new Word struct at the right place in the linked list
      if (prevWord != NULL){
        prevWord->nextInBucket = newWord;
      }
      else{
        (array[index].head) = newWord;
      }
      parentWord = newWord;

      //since we have added a new word entry to the hash table, increment the counter
      hashTable->wordCount++;
    }

    //increment the frequency of word1's Word struct
    parentWord->textFreq++;

    //search for word2 in word1's bigram list

    Bigram* currBigram = (parentWord->bigramList).head;
    Bigram* prevBigram = NULL;
    while (currBigram != NULL){
      if (strcmp(currBigram->str, word2) == 0){
        break;
      }
      prevBigram = currBigram;
      currBigram = currBigram->nextNode;
    }
    Bigram* childWord = currBigram;

    //if it couldn't find word2 in the bigram list, create new Bigram struct for it
    if (childWord == NULL){
      Bigram* newBigram = malloc(sizeof(Bigram));
      newBigram->str = strdup(word2);
      newBigram->bigramFreq = 0;
      newBigram->nextNode = NULL;

      //same as previously with placing the new Bigram struct in the linked list
      if (prevBigram != NULL){
        prevBigram->nextNode = newBigram;
      }
      else{
        (parentWord->bigramList).head = newBigram;
      }

      childWord = newBigram;
    }

    //increment the Bigram struct's frequency
    (childWord->bigramFreq)++;
  }


//Resize (20 pts)

  void resize(HashTable* oldHashTable){
    //taking out all of the variables from our old HashTable struct for convencience of coding
    int oldTableSize = oldHashTable->arrSize;
    Bucket* oldArray = oldHashTable->array;
    float oldLoadFactor = oldHashTable->loadFactor;

    //creates new hash table with updated size and gets some of its variables for convenience sake
    HashTable* newHashTable = hash_table((oldTableSize * 2), oldLoadFactor);
    int newTableSize = newHashTable->arrSize;
    Bucket* newArray = newHashTable->array;

    //since we won't be adding any new words, we can just set the new hash table to have the same word count as the old one
    newHashTable->wordCount = oldHashTable->wordCount;

    //iterates through the old hash table
    Word* prevWord;
    Word* currWord;
    for (int i = 0; i < oldTableSize; i++){
      //iterates through the bucket at the current index
      currWord = oldArray[i].head;
      prevWord = NULL;
      while (currWord != NULL) {
        //gets new index for the current Word struct in the new hash table
        int newIndex = hash(newTableSize, currWord->str);

        //zips to the end of the bucket at the new index
        Word* currZipper = newArray[newIndex].head;
        Word* prevZipper = NULL;
        while (currZipper != NULL){
          prevZipper = currZipper;
          currZipper = currZipper->nextInBucket;
        }

        //puts the word at the right place in the new bucket
        if (prevZipper == NULL){
          newArray[newIndex].head = currWord;
        }
        else{
          prevZipper->nextInBucket = currWord;
        }

        //iterates to the next word in the old bucket
        prevWord = currWord;
        currWord = currWord->nextInBucket;

        //makes sure that after being added to the new bucket, the Word struct's next node is reset to NULL
        prevWord->nextInBucket = NULL;
      }
    }

    //cleans up old mallocs
    free(oldArray);
    free(oldHashTable);
  }

//Testing (0 pts)

int main() {

  HashTable* hashTable = hash_table(20, (float) 5);

  add("lorem", "ipsum", hashTable);
  add("ipsum", "dolor", hashTable);
  add("dolor", "test1", hashTable);
  add("dolor", "test2", hashTable);
  add("dolor", "test2", hashTable);


  for (int i = 0; i < 100; i++) {
    Bucket currentBucket = (hashTable->array)[i];
    Word* currentWord = currentBucket.head;
    // if (currentWord == NULL) {
    //   printf("%d: we've got a null boys\n", i);
    // }
    while (currentWord != NULL) {
      printf("current word: %s with %d occurances\n", currentWord->str, currentWord->textFreq);

      Bigram* currentBigram = (currentWord->bigramList).head;
      while (currentBigram != NULL) {
        printf("after %s: %s with %d occurances\n ", currentWord->str, currentBigram->str, currentBigram->bigramFreq);
        currentBigram = currentBigram->nextNode;
      }

      currentWord = currentWord->nextInBucket;
    }
  }

  return 0;
}

