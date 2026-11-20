#include <stdio.h>
#include <regex.h>

#include "lexer.h"

// returns true if the buffer matches the regex, false otherwise
static int regex_peek(const regex_t *regex, char buffer[512], int buffer_offset) {

    int value = regexec(regex, buffer + buffer_offset, 0, NULL, 0);

    return value != REG_NOMATCH;
}

// returns the length of the segment that matches the regex; if buffer_offset + match length > 256,
// the buffer is modified such that buffer_offset is zero (everything before the regex match is
// discarded and more data is read from the file to fill the remaining buffer space)
// also ensures the final character in buffer is \0
static int regex_consume(const regex_t *regex, char buffer[512], int *buffer_offset, FILE *file) {

}

static int regex_create(regex_t *restrict regex, const char *restrict pattern) {

    if (regcomp(regex, pattern, REG_EXTENDED)) {
        printf("Failed to compile regex\n");
        return 1;
    }
}

static regex_t test;

int init_lexer() {

    // create all our regexes
    return
       regex_create(&test, "^#");
}

int lex(const char *filepath) {

    FILE *file = fopen(filepath, "r");

    if (!file) {
        printf("Could not open %s\n", filepath);
        return 1;
    }

    char buffer[512] = {0};

    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s\n", buffer);
        
        if (regex_peek(&test, buffer, 0)) {
            printf("Pattern found.\n");
        } else {
            printf("Pattern not found.\n");
        }
    }

    printf("\n");

    fclose(file);
}