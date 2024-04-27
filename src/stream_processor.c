#include "stream_processor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ELEMENTS 1024  // Maximum number of elements to monitor

typedef struct {
    char *element;
    int count;
    int heap_index; // Index of this node in the heap array
} ElementNode;

ElementNode *heap[MAX_ELEMENTS];
int heap_size = 0;

// Hash table entry
typedef struct HashTableEntry {
    char *key;
    ElementNode *node;
    struct HashTableEntry *next;
} HashTableEntry;

HashTableEntry *hash_table[MAX_ELEMENTS];

// Hash function
unsigned int hash(char *str) {
    unsigned int hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c

    return hash % MAX_ELEMENTS;
}

// Function to initialize heap and hash table
void init_stream_processor() {
    heap_size = 0;
    memset(heap, 0, sizeof(heap));
    memset(hash_table, 0, sizeof(hash_table));
}

// Function to swap two elements in the heap
void swap(int i, int j) {
    ElementNode *temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
    heap[i]->heap_index = i;
    heap[j]->heap_index = j;
}

// Function to heapify down from the given index
void heapify_down(int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap_size && heap[left]->count < heap[smallest]->count)
        smallest = left;
    if (right < heap_size && heap[right]->count < heap[smallest]->count)
        smallest = right;

    if (smallest != index) {
        swap(index, smallest);
        heapify_down(smallest);
    }
}

// Function to heapify up from the given index
void heapify_up(int index) {
    while (index != 0 && heap[(index - 1) / 2]->count > heap[index]->count) {
        swap(index, (index - 1) / 2);
        index = (index - 1) / 2;
    }
}

// Function to add or update an element in the stream summary
void process_element(char *element) {
    unsigned int index = hash(element);
    HashTableEntry *entry = hash_table[index];

    // Check if element is already in the hash table
    while (entry != NULL && strcmp(entry->key, element) != 0) {
        entry = entry->next;
    }

    if (entry) {
        // Element is found, increase count
        entry->node->count++;
        heapify_down(entry->node->heap_index);
    } else {
        // Element is not found, add new
        if (heap_size < MAX_ELEMENTS) {
            // There is space in the heap, add new element
            ElementNode *node = malloc(sizeof(ElementNode));
            node->element = strdup(element);
            node->count = 1;
            node->heap_index = heap_size;
            heap[heap_size] = node;

            HashTableEntry *new_entry = malloc(sizeof(HashTableEntry));
            new_entry->key = strdup(element);
            new_entry->node = node;
            new_entry->next = hash_table[index];
            hash_table[index] = new_entry;

            heapify_up(heap_size++);
        } else {
            // Replace the least frequent element (root of the heap)
            ElementNode *least = heap[0];
            // Remove old entry from hash table
            HashTableEntry **prev_entry = &hash_table[hash(least->element)];
            while ((*prev_entry)->node != least) {
                prev_entry = &(*prev_entry)->next;
            }
            HashTableEntry *to_delete = *prev_entry;
            *prev_entry = to_delete->next;
            free(to_delete->key);
            free(to_delete);

            // Update heap and hash with new element
            least->element = strdup(element);
            least->count = 1;
            heapify_down(0);

            // Add new hash entry
            HashTableEntry *new_entry = malloc(sizeof(HashTableEntry));
            new_entry->key = strdup(element);
            new_entry->node = least;
            new_entry->next = hash_table[index];
            hash_table[index] = new_entry;
        }
    }
}


// Helper function to compare two elements by their count
int compare(const void *a, const void *b) {
    ElementNode *nodeA = *(ElementNode **)a;
    ElementNode *nodeB = *(ElementNode **)b;
    return (nodeB->count - nodeA->count);
}

// Function to output the top K elements
void output_top_k_elements(int K) {
    // Allocate an array to copy the element pointers
    ElementNode **sorted_elements = malloc(heap_size * sizeof(ElementNode *));
    if (!sorted_elements) {
        perror("Memory allocation failed");
        return;
    }

    // Copy pointers from the heap to the array
    for (int i = 0; i < heap_size; i++) {
        sorted_elements[i] = heap[i];
    }

    // Sort the elements by their count in descending order
    qsort(sorted_elements, heap_size, sizeof(ElementNode *), compare);

    // Output the top K elements, or all elements if there are less than K
    for (int i = 0; i < K && i < heap_size; i++) {
        printf("%s\n", sorted_elements[i]->element);
    }

    // Free the allocated memory
    free(sorted_elements);
}
