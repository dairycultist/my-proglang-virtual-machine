#include <stdio.h>
#include <regex.h>
#include "lexer.h"

// returns true if the buffer matches the regex, false otherwise
int regex_peek(const regex_t *regex, char buffer[512], int buffer_offset) {

    int value = regexec(regex, buffer + buffer_offset, 0, NULL, 0);

    if (value == REG_NOMATCH) {
        printf("Pattern not found.\n");
    } else {
        printf("Pattern found.\n");
    }
}

// returns the length of the segment that matches the regex; if buffer_offset + match length > 256,
// the buffer is modified such that buffer_offset is zero (everything before the regex match is
// discarded and more data is read from the file to fill the remaining buffer space)
// also ensures the final character in buffer is \0
int regex_consume(const regex_t *regex, char buffer[512], int *buffer_offset, FILE *file) {

}

int main(int argc, char **argv) {

    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    FILE *file = fopen(argv[1], "r");

    if (!file) {
        printf("Could not open %s\n", argv[1]);
        return 1;
    }

    char buffer[512] = {0};
    regex_t regex;

    if (regcomp(&regex, "^#", REG_EXTENDED)) {
        printf("Failed to compile regex\n");
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s\n", buffer);
        regex_peek(&regex, buffer, 0);
    }

    printf("\n");

    fclose(file);

    return 0;
}