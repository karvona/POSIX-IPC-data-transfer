#define _POSIX_C_SOURCE 202009L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>

#include "morse.h"   // Include header for morse library map and functions
#include "parseio.h" // Include io parser function   

#define MORSE_CODE_MAX_LEN 6    // Morse symbol lenght for buffer initialization
#define ERROR_MSG_MAX_LEN 256   // Error message length for errors in I/O parsing


int main(int argc, char *argv[]) {

    int ifd, ofd;  // Initialize input and output file descriptors
    char *buf;     // Initialize buffer to store input characters

    // Initialize buffer to store I/O possbile error message from parseio function
    char *err_msg; 
    err_msg = malloc(ERROR_MSG_MAX_LEN);
    if (err_msg == NULL) {
    fprintf(stderr, "Failed to allocate memory for error message");
    return -1;
    }

    // Parse command line arguments, setup input & output file I/O and read input to buffer
    int total_bytes_read = parseio(argc, argv, &ifd, &ofd, &buf, &err_msg);
    if (total_bytes_read < 0) {
        fprintf(stderr, "%s", err_msg);
        return -1;
    }

    // Initialize sigaction struct with zeros
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));

    // Set SA_SIGINFO flag to access siginfo_t data used to mark the end of morse symbol
    sa.sa_flags = SA_SIGINFO; 

    // Assing actions to signals used
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        fprintf(stderr, "Failed to assign action to signal USR1\n");
        return -1;
    }
    if (sigaction(SIGUSR2, &sa, NULL) == -1) {
        fprintf(stderr, "Failed to assign action to signal USR2\n");
        return -1;
    }
    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        fprintf(stderr, "Failed to assign action to signal ALRM\n");
        return -1;
    }

    // Create child process with fork()
	pid_t pid = fork();
	if (pid == -1) {
		fprintf(stderr, "Fork failed\n");
        free(buf);
        free(err_msg);
        close(ifd);
        close(ofd);
		return -1;
	}
    


	if (pid == 0) {
		/*
        Child process (Program B in diagram)
        Converts input characters to morse code and transmits them to the
        parent process using signals SIGUSR1 and SIGUSR2. When transmission 
        is completed, sends SIGALRM to parent to indicate the end of transmission.
        */ 

        pid_t parent_pid = getppid(); // Get parent id
        
        union sigval value = {0}; // Initialize union sigval with zeros

        size_t i = 0;

        while (i < total_bytes_read) {

            char char2send = toupper(buf[i]); // Convert input to upper case

            // Skip whitespace in input
            if (char2send == ' '){
                i++;
                continue;
            }

            // Convert character to its morse code representation
            // using the char2morse function defined in morse.c
            const char* morse_code = char2morse(char2send);
            if (strcmp(morse_code, "\0") == 0) { // EOF
                break;
            }
            if (morse_code == NULL) {
                fprintf(stderr, "Unknown character. Exiting...");
                break;
            }

            // Send morse code symbols with signals to parent
            size_t morse_len = strlen(morse_code);
            for (size_t j = 0; j < morse_len; j++) {
                
                // If last morse symbol of current character, assign value 1 to transmitted signal
                value.sival_int = (j == morse_len - 1) ? 1 : 0;

                // Send signal SIGUSR1 for dots, SIGUSR2 for dashes
                if (morse_code[j] == '.') {
                    sigqueue(parent_pid, SIGUSR1, value);
                }
                else if (morse_code[j] == '-') {
                    sigqueue(parent_pid, SIGUSR2, value);
                }

                sleep(0); // Sleep minimal time for signal handling in parent process
            }
            i++;
            sleep(0);
        }

        // Send SIGALRM to announce end of transmission to parent
        sigqueue(parent_pid, SIGALRM, value);
        exit(0);
	} 



    else {
        // Parent process (Program A in diagram)
        
        // Initialize signal set with signals used in transmission
        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGUSR1);
        sigaddset(&set, SIGUSR2);
        sigaddset(&set, SIGALRM);

        // Block the default actions of the used signals
        if (sigprocmask(SIG_BLOCK, &set, NULL) == -1) {
            fprintf(stderr, "Failed to block signals\n");
            return -1;
        }

        char morse_buf[MORSE_CODE_MAX_LEN + 1]; // Initialize buffer to store morse symbols
        int buf_index = 0;

        while(1) {
            siginfo_t info;

            // Wait for a signal from the set and store its information to 'siginfo_t info'
            sigwaitinfo(&set, &info);

            // If SIGALRM received, transmitting has ended and parent process can abort receiving.
            if (info.si_signo == SIGALRM){ 
                break; 
            }      

            // If SIGUSR1 received, add '.' to morse code buffer.
            else if (info.si_signo == SIGUSR1) { 
                morse_buf[buf_index++] = '.'; 
            } 

            // If SIGUSR2 received, add '-' to morse code buffer.
            else if (info.si_signo == SIGUSR2) { 
                morse_buf[buf_index++] = '-'; 
            }
            

            // Check if the received signal carries `sival_int` value 1, indicating last symbol for current character
            if (info.si_value.sival_int == 1) {
                morse_buf[buf_index] = '\0';
                // Decode received morse string to character using morse2char function defined in morse.c
                char decoded_char = morse2char(morse_buf); 
                write(ofd, &decoded_char, 1); // Write decoded character to output file descriptor
                buf_index = 0; // Reset index in buffer 
            }
        }
    }


    // Finally, free the buffer used to store the input
    // and close the input and output before exiting.
    free(buf);
    free(err_msg);
    close(ifd);
    close(ofd);
        
	return 0;
} 