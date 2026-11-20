#include <stdio.h>
#include <regex.h>
#include <string.h>

#include "lexer.h"

#define LEX_BUFFER_SIZE 512

// Returns the length of the segment in the buffer that matches the regex, or -1 if there is no match.
// Handles file reading and buffering automatically, but will not change the position of buffer_offset
// relative to the data; you must increment it by the consumed segment's length yourself
static int regex_peek(const regex_t *regex, char buffer[LEX_BUFFER_SIZE], int *buffer_offset, FILE *file) {

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

    if (regexec(regex, buffer + *buffer_offset, 8, matches, 0) == REG_NOMATCH)
        return -1;

    return matches[0].rm_eo;
}

static int regex_create(regex_t *restrict regex, const char *restrict pattern) {

    if (regcomp(regex, pattern, REG_EXTENDED)) {
        printf("Failed to compile regex\n");
        return 1;
    }
}

#define TOKEN_TYPE_COUNT 2

#define TKN_WORD 0
#define TKN_EOF 1

static regex_t tokens[TOKEN_TYPE_COUNT];

int init_lexer() {

    // create all our regexes
    // - they all start with ^ because duh
    // - they all end with \s* to consume whitespace/newlines
    return
       regex_create(&tokens[TKN_WORD], "^[a-zA-Z_]+\\s*")
    || regex_create(&tokens[TKN_EOF], "^$");
}

int lex_file(const char *filepath) {

    FILE *file = fopen(filepath, "r");

    if (!file) {
        printf("Could not open %s\n", filepath);
        return 1;
    }

    // init buffer
    char buffer[LEX_BUFFER_SIZE] = {0};
    fread(buffer, sizeof(char), LEX_BUFFER_SIZE - 1, file);

    int buffer_offset = 0;

    // repeatedly lex
    while (1) {

        int state = 0; // 0 for unmatched, 1 for matched, 2 for EOF
        int match_length;

        // try buffer against every token type
        for (int i = 0; i < TOKEN_TYPE_COUNT; i++) {

            match_length = regex_peek(&tokens[i], buffer, &buffer_offset, file);

            if (match_length == -1)
                continue;

            // matched
            printf("<%d>: %.*s\n", i, match_length, buffer + buffer_offset);
            state = i == TKN_EOF ? 2 : 1;
            break;
        }

        if (state) {

            // reached EOF
            if (state == 2)
                break;

            // otherwise, move to the next portion of buffer
            buffer_offset += match_length;

        } else {

            printf("No token matches buffer:\n\n----------\n\n%s\n\n----------\n", buffer + buffer_offset);

            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}