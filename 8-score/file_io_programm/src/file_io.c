#include "file_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "constants.h"

void readFromFile(const char* file_name, char* buffer_string) {
    int fd = open(file_name, O_RDONLY);
    if (fd < 0) {
        printf("Can\'t open file %s \n", file_name);
        exit(-1);
    }
    size_t size = read(fd, buffer_string, BUFFER_SIZE);
    if (size < 0) {
        printf("Can\'t read string from file %s \n", file_name);
        exit(-1);
    }
    close(fd);
}

void writeToFile(const char* file_name, const char* buffer_string) {
    int fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) {
        printf("Can\'t open file %s \n", file_name);
        exit(-1);
    }
    size_t size = write(fd, buffer_string, strlen(buffer_string));
    if (size < 0) {
        printf("Can\'t write all string to file %s \n", file_name);
        exit(-1);
    }
    close(fd);
}
