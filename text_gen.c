#include <stdlib.h>
#include <stdio.h>

//Read Text (40 pts)

    HashTable* read_text(file text) {

        //turn the file into a string

        //create linked list so that the text is separated by 'word' and more easy to read
        //this makes sense to me (asa), but is not necessary. you can just do away with this when implementing if you want to, but it might cause a couple more headaches at the expense of saving this one... idk

        //for (i < number_of_chars)

            //while current_char == 'a'|'b'|'c'|'d'...

                //build a string of a word

            //if current_char == ' '

                //put the word on the end of the linked list

            //else

                //put the word on the linked list, and the current special character also on the linked list


        //create new hash table with size = length of linked list maybe?

        //zip through the linked list, add()ing current and prev bigrams

        //return hash table

    }


//Generate Word (40 pts)

    char generate_word(char word[], HashTable* hashTable) {

        //generate random int between 0 and the total text frequency of the current word

        //for i < random int

            //zip through current word's bigram linked list

            //increment i by however much the bigram frequency of the current item is

        //when the for loop exits, return the string of the current bigram

    }

//Generate Text (20 pts)

    char generate_text(int length, HashTable* hashTable) {

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

    }

//Bigrams to Trigrams (40 pts)

    //bro...... fuck

