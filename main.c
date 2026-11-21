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

    print_tokens(tokens);

    return 0;
}