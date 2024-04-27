#include "tokenizer.h"
#include "stream_processor.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input file name> <K>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("Failed to open the file");
        return EXIT_FAILURE;
    }

    int K = atoi(argv[2]);
    if (K <= 0) {
        fprintf(stderr, "Please provide a positive integer for K\n");
        fclose(fp);
        return EXIT_FAILURE;
    }

    init_stream_processor();  // Initialize the stream processor

    char *word;
    while ((word = get_next_word(fp)) != NULL) {
        process_element(word);  // Process each word
        free(word);  
    }

    fclose(fp);

    output_top_k_elements(K);  // Output the top K frequent items

    return EXIT_SUCCESS;
}
