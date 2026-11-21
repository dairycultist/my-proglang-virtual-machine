#ifndef LEXER_H
#define LEXER_H

typedef int token_t;

#define TYPE_OF_TKN(token_ptr) (*(token_t *) (token_ptr))

#define TKN_WORD 0
#define TKN_EOF 1

typedef struct {
    token_t type;
} EmptyToken;

typedef struct {
    token_t type;
    char *value; // for string literals, keywords, and function/variable names
} StringToken;

typedef struct {
    token_t type;
    int value;
} IntToken;

typedef union {
    EmptyToken empty_token;
    StringToken string_token;
    IntToken int_token;
} Token;

int init_lexer();
int lex_file(const char *filepath, Token **tokens_out);

#endif
