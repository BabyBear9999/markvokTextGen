#include <stdio.h>
#include <stdlin.h>
#include <string.h>

//Word Data Struct (10 pts)
  //hopefully save space by making only one word and having multiple types ref it 
  typedef struct Word{ 
    char str[];
    int textFreq;
  }Word;

  //tells how oft a word is aft a word, used for a list
  typedef struct WordFreq{
    Word w;
    int aftFreq;
  }WordFreq;

  //asign str and textFreq from w, so that users can have easier acess to returning values without having acess to WordFreq and Word in the header file
  typedef struct Word_Data{
    Word w;
    char str[];
    int textFreq;
    WordFreq aftList[];
  }Word_Dat;

//Hash bucket table(10 points)
  
  //hashtable node
  typdef struct Node{
    int key; //not sure abt the type for this 
    Word_Data value;
    struct Node* Next;
  }Node;
  
  //linked list
  typedef struct Bucket{
    Node* head; 
  }Bucket;

//Hash Table Struct (5 pts)
typedef Hash_Table{
  Bucket** arr;
  int arrCap; //current capasity of resizing arr
  int loadFact; //hash table allowed load factor
}Hash_Table;


//Empty Hash Table (5 pts)

//Hash Functions (15 pts)

//Add (20 pts)

//Resize (20 pts)

//Testing (0 pts)



