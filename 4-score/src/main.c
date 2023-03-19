#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "constants.h"
#include "file_io.h"
#include "brackets_checker.h"

/// @brief Gets file names from command line arguments.
/// If there are no arguments, then input.txt and output.txt are used.
/// @param argc number of arguments
/// @param argv  array of arguments
/// @param input_file input file name
/// @param output_file  output file name
void getFileNames(int argc, char* argv[], char** input_file, char** output_file) {
    if (argc == 1) {
        *input_file = "input.txt";
        *output_file = "output.txt";
    } else if (argc == 2) {
        *input_file = argv[1];
        *output_file = "output.txt";
    } else if (argc == 3) {
        *input_file = argv[1];
        *output_file = argv[2];
    } else {
        printf("Too many arguments \n");
        exit(-1);
    }
}

int main(int argc, char* argv[]) {
    int reader_pipe[2], writer_pipe[2], reader_process, writer_process;
    size_t size;
    char buffer_string[BUFFER_SIZE];
    char *input_file, *output_file;
    getFileNames(argc, argv, &input_file, &output_file);

    if (pipe(reader_pipe) < 0 || pipe(writer_pipe) < 0) {
        printf("Can\'t create pipe \n");
        exit(-1);
    }

    reader_process = fork();

    if (reader_process < 0) {
        printf("Can\'t fork child \n");
        exit(-1);
    } else if (reader_process == 0) {
        close(reader_pipe[0]);

        // read buffer_string from file
        readFromFile(input_file, buffer_string);

        size = write(reader_pipe[1], buffer_string, strlen(buffer_string) + 1);
        if (size < 0) {
            printf("Can\'t write all string to pipe \n");
            exit(-1);
        }

        printf("Reader exit \n");
    } else {
        writer_process = fork();
        if (writer_process < 0) {
            printf("Can\'t fork child \n");
            exit(-1);
        } else if (writer_process == 0) {
            close(writer_pipe[1]);

            size = read(writer_pipe[0], buffer_string, BUFFER_SIZE);
            if (size < 0) {
                printf("Can\'t read string from pipe \n");
                exit(-1);
            }

            // write buffer_string to file
            writeToFile(output_file, buffer_string);
    
            printf("Writer exit \n");
        } else {
            close(reader_pipe[1]);
            close(writer_pipe[0]);

            size = read(reader_pipe[0], buffer_string, BUFFER_SIZE);
            if (size < 0) {
                printf("Can\'t read string from pipe \n");
                exit(-1);
            }

            bool is_correct = checkBracketsSequence(buffer_string);
            if (is_correct) {
                strcpy(buffer_string, "YES");
            } else {
                strcpy(buffer_string, "NO");
            }

            size = write(writer_pipe[1], buffer_string, strlen(buffer_string) + 1);
            if (size < 0) {
                printf("Can\'t write all string to pipe \n");
                exit(-1);
            }

            printf("Parent exit \n");
        }
    }

    return 0;
};
