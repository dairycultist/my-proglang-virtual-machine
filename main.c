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

        if (TYPE_OF_TKN(tokens + i) == TKN_WORD) {
            printf("<%d>: %s\n", TYPE_OF_TKN(tokens + i), ((StringToken *) tokens + i)->value);
        } else {
            printf("<%d>: (X)\n", TYPE_OF_TKN(tokens + i));
        }
    }

    return 0;
}