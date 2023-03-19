#include <sys/types.h>
#include <sys/stat.h>
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
    int reader_pipe, writer_pipe, reader_process, writer_process;
    size_t size;
    char buffer_string[BUFFER_SIZE];
    char *input_file, *output_file;
    getFileNames(argc, argv, &input_file, &output_file);

    char reader_fifo_name[] = "./reader.fifo", writer_fifo_name[] = "./writer.fifo";

    (void)umask(0);

    if (mkfifo(reader_fifo_name, __S_IFIFO | 0666) < 0 ||
        mkfifo(writer_fifo_name, __S_IFIFO | 0666) < 0) {
        // printf("Can\'t create FIFO \n");
    }

    reader_process = fork();

    if (reader_process < 0) {
        printf("Can\'t fork child \n");
        exit(-1);
    } else if (reader_process == 0) {
        reader_pipe = open(reader_fifo_name, O_WRONLY);
        if (reader_pipe < 0) {
            printf("Reader can\'t open FIFO for writing \n");
            exit(-1);
        }

        // read buffer_string from file
        readFromFile(input_file, buffer_string);

        size = write(reader_pipe, buffer_string, strlen(buffer_string) + 1);
        if (size < 0) {
            printf("Reader can\'t write all string to pipe \n");
            exit(-1);
        }

        printf("Reader exit \n");
    } else {
        writer_process = fork();
        if (writer_process < 0) {
            printf("Can\'t fork writer \n");
            exit(-1);
        } else if (writer_process == 0) {
            writer_pipe = open(writer_fifo_name, O_RDONLY);
            if (writer_pipe < 0) {
                printf("Writer can\'t open FIFO for reading \n");
                exit(-1);
            }

            size = read(writer_pipe, buffer_string, BUFFER_SIZE);

            if (size < 0) {
                printf("Writer can\'t read string from pipe \n");
                exit(-1);
            }

            // write buffer_string to file
            writeToFile(output_file, buffer_string);

            unlink(writer_fifo_name);
            printf("Writer exit \n");
        } else {
            reader_pipe = open(reader_fifo_name, O_RDONLY);
            if (reader_pipe < 0) {
                printf("Parent can\'t open FIFO for reading \n");
                exit(-1);
            }
            writer_pipe = open(writer_fifo_name, O_WRONLY);
            if (writer_pipe < 0) {
                printf("Parent can\'t open FIFO for writing \n");
                exit(-1);
            }

            size = read(reader_pipe, buffer_string, BUFFER_SIZE);
            if (size < 0) {
                printf("Main can\'t read string from pipe \n");
                exit(-1);
            }

            bool is_correct = checkBracketsSequence(buffer_string);
            if (is_correct) {
                strcpy(buffer_string, "YES");
            } else {
                strcpy(buffer_string, "NO");
            }

            size = write(writer_pipe, buffer_string, strlen(buffer_string) + 1);
            if (size < 0) {
                printf("Main can\'t write all string to pipe \n");
                exit(-1);
            }

            unlink(reader_fifo_name);
            printf("Parent exit \n");
        }
    }


    return 0;
};
