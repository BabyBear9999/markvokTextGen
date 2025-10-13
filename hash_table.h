#ifdef _HASH_TABLE_H_
#define _HASH_TABLE_H_

//Word Data Struct (10 pts)
//type that contains main data about a word.
typedef struct Word Word;
//char* str for the actual word itself
//int textFreq for the number of times is occurs over-all in the reference text
//BigramList bigramList is a linked list of all of the words that could occur after the word
//Word* nextInBucket used to iterate through a bucket.

//type that contains contextual information about a word when it is following another word
typedef struct Bigram Bigram;
//char* str stores the string of the word
//int bigramFreq is the number of times it occurs after the preceding word
//Bigram* nextNode iterates through the linked list of possible next words

//linked list of BigramNodes
typedef struct BigramList BigramList;
//Bigram* head is just the head of the linked list

//Hash Bucket Struct (10 pts)
//linked list of words that go to the same hash index
typedef struct Bucket Bucket;
//Word* head is the start of the linked list of words in the bucket

//Hash Table Struct (5 pts)
//array of buckets
typedef struct HashTable HashTable;
//Bucket* array is the actual hash table itself
//int arrSize is the capacity of the array
//int wordCount is the total number of word entries the hash table has
//int loadFactor is the allowable load factor of the hash table

//Empty Hash Table (5 pts)

// creates an empty hash table on the heap, returns a pointer to it. the hash table will have the size of a prime number >= the input capacity. loadFactor sets the allowable load factor of the hath table.
HashTable* hash_table(int capacity, float loadFactor);

//Add (20 pts)

//given a sequence of two consecutive words (word 1 followed by word2), adds and/or updates their values in the hash table.
void add(char word1[], char word2[], HashTable* hashTable);

#endif
