#include <stdio.h>

#include "lexer.h"

int main(int argc, char **argv) {

    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    if (init_lexer())
        return 1;
    
    Token *tokens;

    if (lex_file(argv[1], &tokens))
        return 1;

    for (int i = 0; TYPE_OF_TOKEN(tokens + i) != TKN_EOF; i++) {

        token_t type = TYPE_OF_TOKEN(tokens + i);
        int value = TOKEN_TYPE_VALUE(type);

        if (value == VALUE_EMPTY) {
            printf("<%d>;\n", type);
        } else if (value == VALUE_STRING) {
            printf("<%d>: %s\n", type, tokens[i].string_token.value);
        } else if (value == VALUE_INT) {
            printf("<%d>: %d\n", type, tokens[i].int_token.value);
        } else {
            printf("Invalid token type/value detected following lexing.");
            return 1;
        }
    }

    return 0;
}