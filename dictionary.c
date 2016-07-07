/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "dictionary.h"

#define ALPHA 27 // alphabet (26 chars) + 1 apostrophe

// node structure
typedef struct node
{
    bool is_word;
    struct node* children[ALPHA];
}
node;

// declare root node
node* root;

//declare int for size() function
int wordCount;

// global variable to build a nodeBucket
long fileSize;

// declare nodBucket for load() and unload()
node* nodeBucket;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // declare and initialize traverse pointer to NULL
    node* trav = NULL;
    
    // set traverse pointer to root (points to root with each new word to check)
    trav = root;
    
    // variable to iterate char by char in a word
    int i = 0;
    
    // check whole word
    while(word[i] != '\0')
    {
        // distinguish between letter (handle case-insensitivity here if so) and apostrophe
        char c = (isalpha(word[i]))?tolower(word[i]):word[i];
        
        // case for appostrophe
        if(c == '\'')
        {
            // check if pointer in the index array is NULL, if so - no word found, else go to next node
            if(trav->children[ALPHA-1] == NULL)
            {
                return false;
            }
            trav = trav->children[ALPHA-1];
        }
        
        // case for letters
        else if(isalpha(c))
        {
            // check if pointer in the index array is NULL, if so - no word found, else go to next node
            if(trav->children[c - 'a'] == NULL)     
            {
                return false;
            }
            trav = trav->children[c - 'a'];
        }
        // go to next char
        i++;
    }
    
    // return boolean value from node after word is checked
    return trav->is_word;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // open dictionary file
    FILE* dict = fopen(dictionary, "rb");  // "b" mode is for optimization
    if(dict == false) 
    {
        printf("Could not open this dictionary (dictionary.c file)");
        return false;
    }
    
    // get size of file
    fseek(dict, 0, SEEK_END);
    fileSize = ftell(dict);
    
    // set file pointer back to the beginning 
    fseek(dict, 0, SEEK_SET);
    
    // initialize nodeBucket with enough memory 
    nodeBucket = calloc((fileSize), sizeof(node));  // (fileSize) allocates enough memory for different dictionaries
    
    // where to start
    node* nextFreeNode = nodeBucket;
    
    // read entire file, put data into buffer
    char* buffer = malloc(fileSize + 1);
    fread(buffer, 1, fileSize, dict);
    
    // mark end of file
    buffer[fileSize] = '\0';
    
    // initialize root
    root = nextFreeNode + 1;
    
    // declare and initialize traverse pointer to navigate through data
    node* trav = NULL;
    
    // make new string to load words into a trie
    char* words = buffer;
    
    //initialize int for size() function
    wordCount = 0;
    
    // loop until '\0' (or nul) is reached; "*words" needed instead of "words" to represent a char
    while(*words)
    {
        // traverse node points to root with each new word
        trav = root;
        
        // put all words into already allocated memory: check if a char represents 
        // a new line to find end of word, char by char
        for(; *words != '\n' && *words; words++)
        {
            // case for apostrophe
            if(*words == '\'')
            {
                // check the index of children array, if NULL, go to nextFreeNode, then put 
                // the right value into a trie
                if(trav->children[ALPHA-1] == NULL)       
                {
                    trav->children[ALPHA-1] = nextFreeNode++;
                }
                trav = trav->children[ALPHA-1];
            }
            
            // case for letters
            else
            {
                // check the index of children array, if NULL, go to nextFreeNode, then put 
                // the right value into a trie
                if(trav->children[*words - 'a'] == NULL)       
                {
                    trav->children[*words - 'a'] = nextFreeNode++;
                }
                trav = trav->children[*words - 'a'];
            }
        }
        
        //when word is found, set boolean value to true && increment wordCount 
        trav->is_word = true;
        wordCount++;
        
        // if new word is found (end of line), repeat with the rest of a buffer
        if(*words == '\n')
        {
            words++;
        }
    }
    
    // housekeeping
    fclose(dict);
    free(buffer);
    
    // loaded
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // already known by load() 
    return wordCount;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // only need to free() one big chunk of memory (save time!)
    free(nodeBucket);
    
    // unloaded
    return true;
}
