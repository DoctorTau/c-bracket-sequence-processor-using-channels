#include <sys/types.h>
#include <sys/wait.h>
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
    int fd[2], file_io_process;
    size_t size;
    char buffer_string[BUFFER_SIZE];
    char *input_file, *output_file;
    getFileNames(argc, argv, &input_file, &output_file);

    if (pipe(fd) < 0) {
        printf("Can\'t create pipe \n");
        exit(-1);
    }

    file_io_process = fork();

    if (file_io_process < 0) {
        printf("Can\'t fork child \n");
        exit(-1);
    } else if (file_io_process == 0) {
        // read buffer_string from file
        readFromFile(input_file, buffer_string);

        size = write(fd[1], buffer_string, strlen(buffer_string) + 1);
        if (size < 0) {
            printf("Can\'t write all string to pipe \n");
            exit(-1);
        }

        sleep(1);

        size = read(fd[0], buffer_string, BUFFER_SIZE);
        if (size < 0) {
            printf("Can\'t read string from pipe \n");
            exit(-1);
        }

        writeToFile(output_file, buffer_string);

        printf("File IO exit \n");
    } else {
        size = read(fd[0], buffer_string, BUFFER_SIZE);
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

        size = write(fd[1], buffer_string, strlen(buffer_string) + 1);
        if (size < 0) {
            printf("Can\'t write all string to pipe \n");
            exit(-1);
        }

        printf("Parent exit \n");
    }

    return 0;
};
