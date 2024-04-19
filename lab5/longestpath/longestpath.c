#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH_LENGTH 1024
#define MAX_ITEMS 10000 // Maximum number of key-value pairs

struct KeyValue
{
    size_t key;                  // Represents path length
    char value[MAX_PATH_LENGTH]; // Represents the file path
};

void map(FILE *input, struct KeyValue *output, int *num_items)
{
    char line[MAX_PATH_LENGTH];
    int initial_num_items = *num_items;

    while (fgets(line, sizeof(line), input) != NULL)
    {
        // Remove newline character if present
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
        {
            line[len - 1] = '\0';
        }

        // Calculate path length
        size_t path_length = strlen(line);

        // Emit key-value pair
        output[*num_items].key = path_length;
        strcpy(output[*num_items].value, line);
        (*num_items)++;

        // Check if we exceed the maximum number of items
        if (*num_items >= MAX_ITEMS)
        {
            fprintf(stderr, "Exceeded maximum number of items\n");
            break;
        }
    }

    printf("Mapped %d items from file\n", *num_items - initial_num_items);
}

void reduce(struct KeyValue *input, int num_items)
{
    char longest_path[MAX_PATH_LENGTH];
    size_t max_length = 0;

    for (int i = 0; i < num_items; i++)
    {
        size_t length = input[i].key;
        char *path = input[i].value;

        // Check if this path has the longest length encountered so far
        if (length > max_length)
        {
            max_length = length;
            strcpy(longest_path, path);
        }
        else if (length == max_length)
        {
            // If same length, output this path (for multiple longest paths)
            printf("%s\n", path);
        }
    }

    // Output the longest path found
    printf("Longest Path(s):\n%s\n", longest_path);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <input_file1> [<input_file2> ...]\n", argv[0]);
        return 1;
    }

    // Allocate memory for key-value pairs
    struct KeyValue *output = malloc(sizeof(struct KeyValue) * MAX_ITEMS);

    if (!output)
    {
        perror("Memory allocation failed");
        return 1;
    }

    int total_items = 0;

    // Process each input file
    for (int i = 1; i < argc; i++)
    {
        FILE *input = fopen(argv[i], "r");
        if (!input)
        {
            perror("Error opening input file");
            continue; // Skip to the next file
        }

        // Perform mapping for the current file
        map(input, output, &total_items);

        fclose(input);
    }

    // Perform reducing on all mapped items
    reduce(output, total_items);

    free(output);

    return 0;
}