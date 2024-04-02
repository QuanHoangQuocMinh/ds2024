#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define FILENAME "example.txt"

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        printf("This program requires at least two processes.\n");
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) { // Server process
        FILE *file = fopen("./test.txt", "rb");
        if (file == NULL) {
            printf("Error opening file.\n");
            MPI_Finalize();
            return 1;
        }

        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        rewind(file);

        char *buffer = (char *)malloc(file_size);
        fread(buffer, 1, file_size, file);
        fclose(file);

        MPI_Send(buffer, file_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        free(buffer);
    } else if (rank == 1) { // Client process
        MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
        int count;
        MPI_Get_count(&status, MPI_CHAR, &count);

        char *buffer = (char *)malloc(count);
        MPI_Recv(buffer, count, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        FILE *file = fopen("received_file.txt", "wb");
        fwrite(buffer, 1, count, file);
        fclose(file);
        free(buffer);
    }

    MPI_Finalize();
    return 0;
}