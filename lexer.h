#ifndef LEXER_H
#define LEXER_H

#define TYPE_OF_TOKEN(token_ptr) (*(token_t *) (token_ptr))
#define TOKEN_TYPE_VALUE(token_type) ( \
    token_type == TKN_WORD ?         \
        VALUE_STRING :               \
    token_type == TKN_INT ?          \
        VALUE_INT :                  \
    token_type == TKN_EOF ?          \
        VALUE_EMPTY :                \
        VALUE_INVALID                \
)

#define VALUE_INVALID -1
#define VALUE_EMPTY 0
#define VALUE_STRING 2
#define VALUE_INT 3

typedef enum {
    TKN_WORD,
    TKN_INT,
    TKN_EOF,
    TOKEN_TYPE_COUNT // not a token type, but gets conveniently assigned the amount of token types
} token_t;

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
