#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define the maximum length of a word
#define MAX_WORD_LENGTH 100

// Data structure for intermediate key-value pairs
typedef struct {
    char key[MAX_WORD_LENGTH];
    int value;
} KeyValuePair;

// Mapper function
void map(char *line, KeyValuePair *pairs, int *pair_count) {
    char word[MAX_WORD_LENGTH];
    int i = 0;
    while (*line) {
        if (isalnum(*line)) {
            word[i++] = tolower(*line);
        } else if (i > 0) {
            word[i] = '\0';
            // Add key-value pair to the array
            strcpy(pairs[*pair_count].key, word);
            pairs[*pair_count].value = 1;
            (*pair_count)++;
            i = 0;
        }
        line++;
    }
}

// Reducer function
void reduce(KeyValuePair *pairs, int pair_count) {
    int i, j, total_count;
    for (i = 0; i < pair_count; i++) {
        // Check if the word already exists in the array
        total_count = pairs[i].value;
        for (j = i + 1; j < pair_count; j++) {
            if (strcmp(pairs[i].key, pairs[j].key) == 0) {
                total_count += pairs[j].value;
                pairs[j].value = 0; // Mark as visited
            }
        }
        // Print the word and its count
        if (pairs[i].value != 0) {
            printf("%s\t%d\n", pairs[i].key, total_count);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    char line[1024];
    KeyValuePair pairs[1000];
    int pair_count = 0;

    // Map phase
    while (fgets(line, sizeof(line), file)) {
        map(line, pairs, &pair_count);
    }

    fclose(file);

    // Reduce phase
    reduce(pairs, pair_count);

    return 0;
}