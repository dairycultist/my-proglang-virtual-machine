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

    for (int i = 0; TYPE_OF_TKN(tokens + i) != TKN_EOF; i++) {

        token_t type = TYPE_OF_TKN(tokens + i);
        int value = TKN_TYPE_VALUE(type);

        if (value == VALUE_STRING) {
            printf("<%d>: %s\n", type, ((StringToken *) tokens + i)->value);
        } else if (value == VALUE_INT) {
            printf("<%d>: %d\n", type, ((IntToken *) tokens + i)->value);
        } else if (value == VALUE_EMPTY) {
            printf("<%d>: (X)\n", type);
        } else {
            printf("Invalid token type/value detected.");
            return 1;
        }
    }

    return 0;
}