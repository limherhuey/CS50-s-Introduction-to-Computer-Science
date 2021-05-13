// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table, == 65536 == 2^16
const unsigned int N = 65536;

// Hash table
node *table[N];

//global variable for total number of words in dictionary
unsigned int wordcount = 0;

//global boolean track whether dictionary is loaded
bool loaded = false;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //copy word into another char array since word is read-only
    int wordlen = strlen(word);
    char w[wordlen + 1];
    for (int i = 0; i < wordlen; i++)
    {
        w[i] = tolower(word[i]);
    }
    //remember to add NUL when string ends
    w[wordlen] = '\0';
    
    int h = hash(w);

    //using a traversal node pointer, search for word in hash table of dictionary
    for (node *searcher = table[h]; searcher != NULL; searcher = searcher->next)
    {
        if (strcasecmp(searcher->word, word) == 0)
        {
            return true;
        }
    }
    
    //if not found, return false
    return false;
}

// Hashes word to a number
//hash function by u/delipity's husband
//https://www.reddit.com/r/cs50/comments/1x6vc8/pset6_trie_vs_hashtable/cf9nlkn
unsigned int hash(const char *word)
{
    unsigned int nhash = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        nhash = (nhash << 2) ^ word[i];
    }
    return nhash % N;
}


// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    char temp[LENGTH + 1];
    node *n;

    //read each word in dictionary for each new loop and insert into hash table
    while (fscanf(dict, "%s", temp) != EOF)
    {
        //allocate memory for new node for the word at hand
        n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        //copy word into node and let node point to nothing
        strcpy(n->word, temp);
        n->next = NULL;

        int h = hash(n->word);
        if (table[h] == NULL)
        {
            //if the hash code is new, node is first in that linked list 
            table[h] = n;
        }
        else
        {
            //if hash code has been used before, new node is attached to start of linked list
            n->next = table[h];
            table[h] = n;
        }
        
        //add one to global variable for wordcount
        wordcount++;
    }

    //DO NOT free now since it will free the memory storing the last word
    //close dictionary file
    fclose(dict);
    
    //turn global variable for whether dictionary is loaded to true, end function
    loaded = true;
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (loaded)
    {
        return wordcount;
    }
    else
    {
        return 0;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    //run through total number of buckets in hash table
    for (int h = 0; h < N; h++)
    {
        //assign a head temporary pointer to first node in each linked list
        node *fhead = table[h];
        
        //as long as not end of linked list, assign another temporary pointer to
        //current node, let head node point to next node, free current node
        while (fhead != NULL)
        {
            //assign another temporary pointer to delete 
            node *ftail = fhead;
            fhead = fhead->next;
            free(ftail);
        }
    }
    //function finishes its job, all nodes are freed
    return true;
}
