#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

// Function to check if a character is valid part of a word
int is_valid_char(char c) {
    return isalpha((unsigned char)c);
}

// Function to read the next word from the file
char *get_next_word(FILE *fp) {
    char *word = malloc(BUFFER_SIZE); // Allocate memory for the word
    if (!word) {
        perror("Failed to allocate memory for word");
        return NULL;
    }

    int index = 0, ch;

    // Read characters until a valid character is found or end of file
    while ((ch = fgetc(fp)) != EOF && !is_valid_char(ch)) {
        // Skip non-alphabetic characters
    }

    if (ch == EOF) {
        free(word);
        return NULL;
    }

    // Start storing valid characters into the word
    do {
        word[index++] = tolower(ch);  // Store in lowercase
        if (index >= BUFFER_SIZE) { // Check for buffer overflow
            fprintf(stderr, "Buffer overflow, word too long.\n");
            free(word);
            return NULL;
        }
    } while ((ch = fgetc(fp)) != EOF && is_valid_char(ch));

    word[index] = '\0'; // Null-terminate the string
    return word;
}
