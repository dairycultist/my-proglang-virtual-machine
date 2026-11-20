#include <stdio.h>
#include <regex.h>
#include <string.h>

#include "lexer.h"

#define LEX_BUFFER_SIZE 512

// returns true if the buffer matches the regex, false otherwise
static int regex_peek(const regex_t *regex, char buffer[512], int buffer_offset) {

    int value = regexec(regex, buffer + buffer_offset, 0, NULL, 0);

    return value != REG_NOMATCH;
}

// returns the length of the consumed segment. after running the function, the buffer_offset will
// still point to the beginning of the consumed segment; you must increment it by the consumed
// segment's length yourself
static int regex_consume(const regex_t *regex, char buffer[LEX_BUFFER_SIZE], int *buffer_offset, FILE *file) {

    // check if buffer_offset is beyond the halfway point, which means that we're running out of space!
    if (*buffer_offset > LEX_BUFFER_SIZE / 2) {

        // this isn't AI I swear I just had trouble visualizing the process I'm implementing so there's a lot of comments

        // move data beyond buffer_offset to the beginning of the buffer
        memcpy(buffer, buffer + *buffer_offset, LEX_BUFFER_SIZE - *buffer_offset - 1);
        
        // set buffer_offset to the point right after that data
        *buffer_offset = LEX_BUFFER_SIZE - *buffer_offset - 1;

        // zero out everything after that point to ensure the character following valid buffer data stays \0
        memset(buffer + *buffer_offset, 0, LEX_BUFFER_SIZE - *buffer_offset);

        // read from the file to fill the remaining buffer space
        fread(buffer + *buffer_offset, sizeof(char), LEX_BUFFER_SIZE - *buffer_offset - 1, file);

        // set buffer_offset to 0 to reflect the new position of the data
        *buffer_offset = 0;
    }

    // return the length of the consumed segment
    // TODO capture and return matches
    regmatch_t matches[8];

    regexec(regex, buffer + *buffer_offset, 8, matches, 0);

    return matches[0].rm_eo;
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
       regex_create(&test, "^#[a-z]*");
}

int lex(const char *filepath) {

    FILE *file = fopen(filepath, "r");

    if (!file) {
        printf("Could not open %s\n", filepath);
        return 1;
    }

    // init buffer
    char buffer[LEX_BUFFER_SIZE] = {0};
    fread(buffer, sizeof(char), LEX_BUFFER_SIZE - 1, file);

    int buffer_offset = 0;

    if (regex_peek(&test, buffer, 0)) {
        printf("Pattern found. Here is the match:\n");
        int match_length = regex_consume(&test, buffer, &buffer_offset, file);
        printf("%.*s\n", match_length, buffer + buffer_offset);
    } else {
        printf("Pattern not found.\n");
    }

    printf("\n");

    fclose(file);
}