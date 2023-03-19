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
    int fd;
    size_t size;
    char buffer_string[BUFFER_SIZE];
    char *input_file, *output_file;
    getFileNames(argc, argv, &input_file, &output_file);

    char fifo_name[] = "file_io.fifo";

    (void)umask(0);

    fd = open(fifo_name, O_RDWR);
    if (fd < 0) {
        printf("File IO can\'t open FIFO for writing \n");
        exit(-1);
    }

    // read buffer_string from file
    readFromFile(input_file, buffer_string);

    size = write(fd, buffer_string, strlen(buffer_string) + 1);
    if (size < 0) {
        printf("File IO can\'t write all string to pipe \n");
        exit(-1);
    }

    sleep(1);
    size = read(fd, buffer_string, BUFFER_SIZE);
    if (size < 0) {
        printf("File IO can\'t read string from pipe \n");
        exit(-1);
    }
    // write buffer_string to file
    writeToFile(output_file, buffer_string);

    unlink(fifo_name);

    printf("File IO exit \n");

    return 0;
};
