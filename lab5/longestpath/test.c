#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PATH_LENGTH 1024 // Adjust if needed

// Function to split a path into its components (directories)
int split_path(char *path, char *components[MAX_PATH_LENGTH])
{
    int i = 0;
    char *token;

    token = strtok(path, "/");
    while (token != NULL)
    {
        components[i++] = token;
        token = strtok(NULL, "/");
    }

    return i;
}

// Map function (simulated)
// Input: A line containing a full file path
// Output: Emits key-value pairs where key is the path and value is 1
void map(char *line)
{
    char path[MAX_PATH_LENGTH];
    strcpy(path, line); // Copy input line to avoid modifying original data

    printf("Map: Emitting key-value pair: (%s, 1)\n", path);
}

// Reduce function (simulated)
// Input: Key-value pairs (path, count)
// Output: Prints the longest path(s)
void reduce(char *key, int count)
{
    static int longest_depth = 0;                                // Track the longest depth seen so far
    static char longest_paths[MAX_PATH_LENGTH][MAX_PATH_LENGTH]; // Store longest paths

    int current_depth = count - 1; // Account for the root directory
    if (current_depth > longest_depth)
    {
        longest_depth = current_depth;
        strcpy(longest_paths[0], key);
        for (int i = 1; i < MAX_PATH_LENGTH; i++)
        { // Clear previous longest paths
            longest_paths[i][0] = '\0';
        }
    }
    else if (current_depth == longest_depth)
    {
        int j = 0;
        while (longest_paths[j][0] != '\0')
        {
            j++;
        }
        strcpy(longest_paths[j], key);
    }

    printf("Reduce: ");
    if (current_depth == longest_depth)
    {
        printf("Longest path(s):\n");
        for (int i = 0; i < MAX_PATH_LENGTH; i++)
        {
            if (longest_paths[i][0] != '\0')
            {
                printf("  - %s\n", longest_paths[i]);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <file1> [file2 ...]\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++)
    {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL)
        {
            perror("fopen");
            continue;
        }

        char line[MAX_PATH_LENGTH];
        while (fgets(line, sizeof(line), fp) != NULL)
        {
            map(line); // Simulate Map phase
        }

        fclose(fp);
    }

    printf("Reduce phase:\n");
    // Simulate Reduce phase by calling reduce with dummy key and count
    reduce("", 0); // Trigger final processing of longest paths

    return 0;
}