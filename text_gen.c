#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "hash_table.h"

//Read Text (40 pts)

    typedef struct textUnit {
        struct textUnit* nextUnit;
        char* str;
    }textUnit;

    typedef struct textList {
        textUnit* head;
    }textList;

    void append(char* string, char newChar) {
        int length = strlen(string);
        string[length] = newChar;
        string[length + 1] = '\0';
    }

    HashTable* read_text(char textFileName[]) {

        //get the file
        FILE* textFile = fopen(textFileName, "r");

        //create linked list so that the text is separated by 'word' and more easy to read
        textList linkedList;
        linkedList.head = NULL;

        //start building the list
        textUnit* currentPos = linkedList.head;
        char currentChar = fgetc(textFile);
        char currentWord[50]; //just give it a bunch of space so that it'll like never overflow
        while (currentChar != EOF) {
            //set the char to lowercase
            currentChar = tolower(currentChar);

            //start building a word
            if (currentChar == 'a' || currentChar == 'b' || currentChar == 'c' || currentChar == 'd' || currentChar == 'e' || currentChar == 'f' || currentChar == 'g' || currentChar == 'h' || currentChar == 'i' || currentChar == 'j' || currentChar == 'k' || currentChar == 'l' || currentChar == 'm' || currentChar == 'n' || currentChar == 'o' || currentChar == 'p' || currentChar == 'q' || currentChar == 'r' || currentChar == 's' || currentChar == 't' || currentChar == 'u' || currentChar == 'v' || currentChar == 'w' || currentChar == 'x' || currentChar == 'y' || currentChar == 'z'){ //lmao
                append(currentWord, currentChar);
            }

            else {
                //if we're at the end of a word, dump the current word to the linked list and reset it
                textUnit newWord;
                newWord.str = strdup(currentWord);
                currentPos->nextUnit = &newWord;
                currentPos = &newWord;
                strcpy(currentWord, "\0"); // THIS DOESN'T WORK. FIND OUT HOW TO MAKE THIS WORK!!
            }
            if (currentChar != ' ') {
                //special characters should also be added as a unit
                textUnit newSpecChar;
                newSpecChar.str = "\0";
                append(newSpecChar.str, currentChar);
                currentPos->nextUnit = &newSpecChar;
                currentPos = &newSpecChar;
            }
            currentChar = fgetc(textFile);
        }

        //adds the last word before the EOF quits the loop
        textUnit lastWord;
        lastWord.str = strdup(currentWord);
        currentPos->nextUnit = &lastWord;
        lastWord.nextUnit = NULL;
        free(currentWord);

        //create new hash table
        HashTable* hashTable = hash_table(1000, 2.5); // numbers picked arbitrarily...

        //load the linked list into the hash table
        textUnit* parentWord = linkedList.head;
        textUnit* childWord = linkedList.head->nextUnit;
        while (childWord != NULL) {
            add(parentWord->str, childWord->str, hashTable);
            free(parentWord->str);
            parentWord = childWord;
            childWord = childWord->nextUnit;
        }

        fclose(textFile);

        return hashTable;

    }


//Generate Word (40 pts)

    // char generate_word(char word[], HashTable* hashTable) {

        //generate random int between 0 and the total text frequency of the current word

        //for i < random int

            //zip through current word's bigram linked list

            //increment i by however much the bigram frequency of the current item is

        //when the for loop exits, return the string of the current bigram

    // }

//Generate Word (40 pts) 
    
    char* generate_word(char word[], HashTable* hashTable) {
        if (hashTable == NULL || word == NULL) {
            return "";
        }
    
        int index = hash(hashTable->arrSize, word);
        Bucket* array = hashTable->array;
    
        Word* currWord = array[index].head;
        while (currWord != NULL) {
            if (strncmp(currWord->str, word, strlen(word)) == 0) {
                break;
            }
            currWord = currWord->nextInBucket;
        }
        if (currWord == NULL || (currWord->bigramList).head == NULL) {
            return "";
        }
    
        int totalFreq = 0;
        Bigram* it = (currWord->bigramList).head;
        while (it != NULL) {
            totalFreq += it->bigramFreq;
            it = it->nextNode;
        }
        if (totalFreq <= 0) {
            return "";
        }
        int r = rand() % (totalFreq + 1);
        int running = 0;
        it = (currWord->bigramList).head;
        while (it != NULL) {
            running += it->bigramFreq;
            if (running > r) {
                return it->str;
            }
            it = it->nextNode;
        }
        return (currWord->bigramList).head->str;
    }


//Generate Text (20 pts)

    // char generate_text(int length, HashTable* hashTable) {

        //generate random int between 0 and word count of hash table

        //for loop down the hash table array

            //if the current bucket isn't empty

                //zoop through the bucket, each time incrementing i by whatever the text frequency of the word is

                //if i > the random int we generated earlier, pick the current word as the starting point and break

        //create new string (figure out how to do this without causing like memory issues cuz we don't really know how much space it needs to be alloced)

        //for i < length

            //append the result of generate_word to the string
            //make sure this correctly applies spaces with non-word special characters

        //return the string

   // }
    int hash(int size, char string[]);
    char* generate_word(char word[], HashTable* hashTable); 
    
    static int no_space_before(const char* tok) {
        return tok && (strcmp(tok, ".")==0 || strcmp(tok, ",")==0 || strcmp(tok, ";")==0 || strcmp(tok, ":")==0 || strcmp(tok, "!")==0 || strcmp(tok, "?")==0 || strcmp(tok, ")")==0 || strcmp(tok, "]")==0 || strcmp(tok, "}")==0);
    }
    static int no_space_after(const char* tok) {
        return tok && (strcmp(tok, "(")==0 || strcmp(tok, "[")==0 || strcmp(tok, "{")==0 || strcmp(tok, "\"")==0 || strcmp(tok, "'")==0);
    }
    
    static void append_token(char** buf, int* cap, int* len, const char* prev, const char* tok) {
        if (tok == NULL) return;
        int need_space = 0;
    
        if (*len == 0) {
            need_space = 0;
        } else if (no_space_before(tok)) {
            need_space = 0;            
        } else if (prev && no_space_after(prev)) {
            need_space = 0;                    
        } else {
            need_space = 1;
        }
    
        int add_len = (need_space ? 1 : 0) + (int)strlen(tok);
        if (*len + add_len + 1 >= *cap) {
            int new_cap = (*cap < 64 ? 128 : (*cap * 2));
            while (*len + add_len + 1 >= new_cap) new_cap *= 2;
            char* nb = (char*)realloc(*buf, new_cap);
            if (!nb) return; 
            *buf = nb; *cap = new_cap;
        }
        if (need_space) (*buf)[(*len)++] = ' ';
        memcpy((*buf) + *len, tok, strlen(tok));
        *len += (int)strlen(tok);
        (*buf)[*len] = '\0';
    }

    static char* pick_start_word(HashTable* hashTable) {
        if (!hashTable) return NULL;
    
        int total = 0;
        for (int i = 0; i < hashTable->arrSize; i++) {
            Word* w = hashTable->array[i].head;
            while (w) {
                total += w->textFreq;
                w = w->nextInBucket;
            }
        }
        if (total <= 0) return NULL;
    
        int r = rand() % total;
        int run = 0;
        for (int i = 0; i < hashTable->arrSize; i++) {
            Word* w = hashTable->array[i].head;
            while (w) {
                run += w->textFreq;
                if (run > r) return w->str;
                w = w->nextInBucket;
            }
        }
        for (int i = 0; i < hashTable->arrSize; i++) {
            if (hashTable->array[i].head) return hashTable->array[i].head->str;
        }
        return NULL;
    }
    
    char* generate_text(int sentences, HashTable* hashTable) {
        if (hashTable == NULL || sentences <= 0) {
            return NULL;
        }
    
        char* curr = pick_start_word(hashTable);
        if (curr == NULL) {
            return NULL;
        }
        int cap = 256, len = 0;
        char* out = (char*)malloc(cap);
        if (!out) return NULL;
        out[0] = '\0';
        append_token(&out, &cap, &len, NULL, curr);
        int period_count = (strcmp(curr, ".") == 0) ? 1 : 0;
    
        while (period_count < sentences && max_steps-- > 0) {
            char* next = generate_word(curr, hashTable);
            if (next == NULL) break;
    
            append_token(&out, &cap, &len, curr, next);
    
            if (strcmp(next, ".") == 0) {
                period_count++;
            }
            curr = next;
        }
    
        return out;
    }

//Bigrams to Trigrams (40 pts)

    //bro...... fuck

