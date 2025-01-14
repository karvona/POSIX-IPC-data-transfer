#ifndef MORSE_H
#define MORSE_H

typedef struct {
    char character;
    const char *morse;
} MorseMap;

const char* char2morse(char c);
char morse2char(const char *morse_string);

#endif