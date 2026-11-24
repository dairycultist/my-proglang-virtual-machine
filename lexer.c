#include <stdio.h>
#include <regex.h>
#include <string.h>
#include <stdlib.h>

#include "lexer.h"

#define LEX_BUFFER_SIZE 512

// Returns the length of the segment in the buffer that matches the regex, or -1 if there is no match.
// Handles file reading and buffering automatically, but will not change the position of buffer_offset
// relative to the data; you must increment it by the consumed segment's length yourself
static int regex_peek(const regex_t *regex, regmatch_t matches[8], char buffer[LEX_BUFFER_SIZE], int *buffer_offset, FILE *file) {

    // check if we're running out of space in the buffer
    if (*buffer_offset > LEX_BUFFER_SIZE / 2) {

        int remaining_data_len = LEX_BUFFER_SIZE - *buffer_offset - 1;

        // move remaining data to the beginning of the buffer
        memcpy(buffer, buffer + *buffer_offset, remaining_data_len);

        // read from the file to fill the new buffer space (ensure null-termination)
        memset(buffer + remaining_data_len, 0, *buffer_offset + 1);
        fread(buffer + remaining_data_len, sizeof(char), *buffer_offset, file);

        *buffer_offset = 0;
    }

    // return the length of the consumed segment; capture and return matches
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

static regex_t tokens[TOKEN_TYPE_COUNT];

int init_lexer() {

    // create all our regexes
    // - they all start with ^ because duh
    // - they all end with \s* or \s+ to consume whitespace/newlines

    // [a-zA-Z](_?[a-zA-Z0-9])* gives us an identifier that must start with a letter,
    // can only have internal contiguous sequences of at most a single underscore, and
    // can otherwise contain both letters and numbers
    return 0
    || regex_create(&tokens[TKN_ASSIGN], "^=\\s*")
    || regex_create(&tokens[TKN_STMT_END], "^;\\s*")
    || regex_create(&tokens[TKN_BLOCK_START], "^\\{\\s*")
    || regex_create(&tokens[TKN_BLOCK_END], "^\\}\\s*")
    || regex_create(&tokens[TKN_PAREN_OPEN], "^\\(\\s*")
    || regex_create(&tokens[TKN_PAREN_CLOSE], "^\\)\\s*")
    || regex_create(&tokens[TKN_COMMA], "^\\,\\s*")
    || regex_create(&tokens[TKN_FUNCTION], "^fn\\s+")
    || regex_create(&tokens[TKN_MUTABLE], "^mut\\s+")
    || regex_create(&tokens[TKN_TYPE_INT], "^int\\s+")
    || regex_create(&tokens[TKN_LIB_IDENTIFIER], "^([a-zA-Z](_?[a-zA-Z0-9])*:[a-zA-Z](_?[a-zA-Z0-9])*)\\s*")
    || regex_create(&tokens[TKN_IDENTIFIER], "^([a-zA-Z](_?[a-zA-Z0-9])*)\\s*")
    || regex_create(&tokens[TKN_INT], "^(-?[0-9]+)\\s*")
    || regex_create(&tokens[TKN_EOF], "^$");
}

int lex_file(const char *filepath, Token **tokens_out) {

    FILE *file = fopen(filepath, "r");

    if (!file) {
        printf("Could not open %s\n", filepath);
        return 1;
    }

    // init output array
    // TODO remember to realloc if we need more space
    *tokens_out = malloc(sizeof(Token) * 1024);
    int token_count = 0;

    // init file buffer
    char buffer[LEX_BUFFER_SIZE] = {0};
    fread(buffer, sizeof(char), LEX_BUFFER_SIZE - 1, file);

    int buffer_offset = 0;

    // repeatedly lex
    while (1) {

        int state = 0; // 0 for unmatched, 1 for matched, 2 for EOF
        int match_length;

        regmatch_t matches[8];

        // try buffer against every token type
        for (token_t token_type = 0; token_type < TOKEN_TYPE_COUNT; token_type++) {

            match_length = regex_peek(&tokens[token_type], matches, buffer, &buffer_offset, file);

            if (match_length == -1)
                continue;

            // matched; add token object to list
            switch (TOKEN_TYPE_VALUE(token_type)) {

                case VALUE_EMPTY: {

                    EmptyToken *token = &((*tokens_out)[token_count++].empty_token);
                    token->type = token_type;
                    break;
                }

                case VALUE_STRING: {

                    StringToken *token = &((*tokens_out)[token_count++].string_token);
                    token->type = token_type;

                    int len = matches[1].rm_eo - matches[1].rm_so;

                    token->value = malloc(sizeof(char) * (len + 1));
                    token->value[len] = '\0';
                    memcpy(token->value, buffer + buffer_offset + matches[1].rm_so, len);
                    break;
                }

                case VALUE_INT: {

                    IntToken *token = &((*tokens_out)[token_count++].int_token);
                    token->type = token_type;
                    
                    // we don't wanna make a separate string to read from, so just temporarily null-terminate the match
                    char hold = buffer[buffer_offset + matches[1].rm_eo];
                    buffer[buffer_offset + matches[1].rm_eo] = '\0';

                    token->value = (int) strtol(buffer + buffer_offset + matches[1].rm_so, NULL, 10);

                    buffer[buffer_offset + matches[1].rm_eo] = hold;
                    break;
                }
                
                default:

                    printf("Invalid token type/value detected during lexing.");

                    fclose(file);
                    return 1;
            }

            state = token_type == TKN_EOF ? 2 : 1;
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

// for debugging and stuff
void print_tokens(Token *tokens) {

    for (int i = 0; TYPE_OF_TOKEN(tokens + i) != TKN_EOF; i++) {

        token_t type = TYPE_OF_TOKEN(tokens + i);
        int value = TOKEN_TYPE_VALUE(type);

        switch (value) {
            case VALUE_EMPTY:
                printf("<%d>;\n", type);
                break;
            case VALUE_STRING:
                printf("<%d>: %s\n", type, tokens[i].string_token.value);
                break;
            case VALUE_INT:
                printf("<%d>: %d\n", type, tokens[i].int_token.value);
                break;
            default:
                printf("<%d is an invalid token type/has an invalid value>", type);
                break;
        }
    }
}