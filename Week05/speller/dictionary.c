// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

int dictionary_size;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int hash_code = hash(word);

    node *current_node = table[hash_code];

    while (current_node)
    {
        if (strcasecmp(current_node->word, word) == 0)
        {
            return true;
        }

        current_node = current_node->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    if (isupper(*word))
    {
        return tolower(*word) % 26;
    }

    return *word % 26;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *loaded_file = fopen(dictionary, "r");

    // If loaded_file points to null it means that there's no file, so we must return false.
    if (loaded_file == NULL)
    {
        return false;
    }

    char current_word[LENGTH + 1];


    while(fscanf(loaded_file, "%s", current_word) != EOF)
    {
        // Base case to check if the computer has sufficent memory.
        node *current = malloc(sizeof(node));

        if(current == NULL)
        {
            return false;
        }

        // Copys current_word to node current property word.
        strcpy(current->word, current_word);

        //call hash function to get the bucket of word;
        int hash_code = hash(current_word);

        if(table[hash_code])
        {
            current->next = table[hash_code];
        }

        table[hash_code] = current;

        dictionary_size++;
    }

    fclose(loaded_file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return dictionary_size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
// TODO
    for (int i = 0; i < N; i++)
    {
        node *chain = table[i];

        while(chain)
        {
            node *flush = chain;
            chain = chain->next;
            free(flush);
        }


        if (i == N - 1 && chain == NULL)
        {
            return true;
        }
    }

    return false;
}

