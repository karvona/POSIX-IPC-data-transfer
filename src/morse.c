#include <stdio.h>
#include <string.h>
#include "morse.h"

#define MORSE_MAP_SIZE 54

// Characters and their morse code representations
MorseMap morseMap[] = {
    {'A', ".-"    }, {'B', "-..."  }, {'C', "-.-."   }, {'D', "-.."   }, {'E', "."     },
    {'F', "..-."  }, {'G', "--."   }, {'H', "...."   }, {'I', ".."    }, {'J', ".---"  },
    {'K', "-.-"   }, {'L', ".-.."  }, {'M', "--"     }, {'N', "-."    }, {'O', "---"   },
    {'P', ".--."  }, {'Q', "--.-"  }, {'R', ".-."    }, {'S', "..."   }, {'T', "-"     },
    {'U', "..-"   }, {'V', "...-"  }, {'W', ".--"    }, {'X', "-..-"  }, {'Y', "-.--"  },
    {'Z', "--.."  }, {'0', "-----" }, {'1', ".----"  }, {'2', "..---" }, {'3', "...--" },
    {'4', "....-" }, {'5', "....." }, {'6', "-...."  }, {'7', "--..." }, {'8', "---.." },
    {'9', "----." }, {'&', ".-..." }, {'\'', ".----."}, {'@', ".--.-."}, {')', "-.--.-"},
    {'(', "-.--." }, {':', "---..."}, {',', "--..--" }, {'=', "-...-" }, {'!', "-.-.--"},
    {'.', ".-.-.-"}, {'-', "-....-"}, {'+', ".-.-."  }, {'\"', ".-..-."}, {'?', "..--.."},
    {'/', "-..-." }, {'\n', "\0"}
};

// Get the morse code representation corresponding to the input character.
const char* char2morse(char c) {
    for (int i = 0; i < MORSE_MAP_SIZE; i++) {
        if (morseMap[i].character == c) {
            return morseMap[i].morse;
        }
    }
    return NULL;
}

// Get the character corresponding to given morse code representation.
char morse2char(const char *morse_string) {
    for (int i = 0; i < MORSE_MAP_SIZE; i++) {
        if (strcmp(morseMap[i].morse, morse_string) == 0) {
            return morseMap[i].character;
        }
    }
    return '\0';
}
