#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "parseio.h"

#define BLOCKSIZE 4096          // Input buffer size for buffer initialization
#define ERROR_MSG_MAX_LEN 256   // Error message length for errors in I/O parsing

/*
 * Parses input/output file descriptors and allocates a buffer for reading input.
 *
 * @argc: The number of command line arguments passed when calling the main program.
 * @argv: The command line arguments used when calling the main program.
 * @ifd:  Pointer to input file descriptor
 * @ofd:  Pointer to output file descriptor
 * @buf:  Pointer to allocated buffer for input
 * @err_msg: Pointer to error message for main program
 *
 * Returns the total bytes read on success, or -1 on error with error message in err_msg
 */

int parseio(int argc, char *argv[], int *ifd, int *ofd, char **buf, char **err_msg) {

    /* Input:  
     - If file name, use that file as input
     - If '-', use standard input (stdin)

    Output: 
     - If file name, use that file for output
     - If '-', use standard output. (stdout)
    */

    // Only input given, output to stdout
    if (argc == 2) { // 
        *ofd = STDOUT_FILENO;
        if (strcmp(argv[1],"-") == 0) { // Check given input argument for stdin
            *ifd = STDIN_FILENO;
        } else { // Attempt to use given input file as input
            *ifd = open(argv[1], O_RDONLY);
            if (*ifd < 0) {
                snprintf(*err_msg, ERROR_MSG_MAX_LEN,
                        "Opening input file (%s) failed\n"
                        "Usage: %s [input|-] [output|-]\n", 
                        argv[1], argv[0]);
                return -1;
            }
        }
        // Both input and output given
        // Handle input first
    } else if (argc == 3) { 
        if (strcmp(argv[1],"-") == 0) { // Check given input argument for stdin
            *ifd = STDIN_FILENO;
        } else {  // Attempt to use given input file as input
            *ifd = open(argv[1], O_RDONLY);
            if (*ifd < 0) {
                snprintf(*err_msg, ERROR_MSG_MAX_LEN,
                        "Opening input file (%s) failed\n"
                        "Usage: %s [input|-] [output|-]\n", 
                        argv[1], argv[0]);
                return -1;
            }
        }
        // Then handle output
        if (strcmp(argv[2],"-") == 0) { // Check given output argument for stdout
            *ofd = STDOUT_FILENO;
        } else { // Attempt to use given output file for output
            *ofd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (*ofd < 0) {
                snprintf(*err_msg, ERROR_MSG_MAX_LEN,
                        "Creating output file (%s) failed\n"
                        "Usage: %s [input|-] [output|-]\n", 
                        argv[2], argv[0]);
                return -1;
            }
        }
    } else {
        snprintf(*err_msg, ERROR_MSG_MAX_LEN,"Usage: %s [input|-] [output|-]\n", argv[0]);
        return -1;
    }

    // Allocate memory to buffer for reading input data
    *buf = malloc(BLOCKSIZE);
    if (*buf == NULL) {
        close(*ifd);
        close(*ofd);
        return -1;
    }

    // Read the input data into the buffer
    size_t total_bytes_read = 0;
    while (1) {
        size_t bytes_read = read(*ifd, *buf + total_bytes_read, BLOCKSIZE - total_bytes_read);
        if (bytes_read < 0) {
            snprintf(*err_msg, ERROR_MSG_MAX_LEN, "Read error\n");
            close(*ifd);
            close(*ofd);
            return -1;
        }
        if (bytes_read == 0) break; // EOF
        
        total_bytes_read += bytes_read;
    }
    return total_bytes_read;
}