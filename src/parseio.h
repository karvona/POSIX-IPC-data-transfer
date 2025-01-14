#ifndef PARSEIO_H
#define PARSEIO_H

#include <unistd.h>

int parseio(int argc, char *argv[], int *ifd, int *ofd, char **input_buffer, char** err_msg);

#endif