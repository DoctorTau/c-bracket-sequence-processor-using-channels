#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "constants.h"
#include "brackets_checker.h"

int main(int argc, char* argv[]) {
    int fd;
    size_t size;
    char buffer_string[BUFFER_SIZE];

    char fifo_name[] = "file_io.fifo";

    (void)umask(0);

    if (mkfifo(fifo_name, __S_IFIFO | 0666) < 0) {
        // printf("Can\'t create FIFO \n");
    }

    fd = open(fifo_name, O_RDWR);
    if (fd < 0) {
        printf("Parent can\'t open FIFO for reading \n");
        exit(-1);
    }

    size = read(fd, buffer_string, BUFFER_SIZE);
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

    size = write(fd, buffer_string, strlen(buffer_string) + 1);
    if (size < 0) {
        printf("Main can\'t write all string to pipe \n");
        exit(-1);
    }

    printf("Parent exit \n");

    return 0;
};
