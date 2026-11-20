#ifndef LEXER_H
#define LEXER_H

typedef int token_t;

typedef struct EmptyToken {

    token_t type; // a unique int saying this is a MyElementType token object so we know what type to cast a (token *) pointing to a token object to
    token_t *next; // a pointer to the start of the next token object (whose first property is always its type)

    // whatever data the token stores, if any, goes here

};

int init_lexer();
int lex(const char *filepath);

#endif
