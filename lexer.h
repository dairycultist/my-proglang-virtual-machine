#ifndef LEXER_H
#define LEXER_H

#define TYPE_OF_TOKEN(token_ptr) (*(token_t *) (token_ptr))
#define TOKEN_TYPE_VALUE(token_type) ( \
    token_type == TKN_LIB_IDENTIFIER ||\
    token_type == TKN_IDENTIFIER ?   \
        VALUE_STRING :               \
    token_type == TKN_INT ?          \
        VALUE_INT :                  \
    token_type == TKN_ASSIGN ||      \
    token_type == TKN_STMT_END ||    \
    token_type == TKN_BLOCK_START || \
    token_type == TKN_BLOCK_END ||   \
    token_type == TKN_PAREN_OPEN ||  \
    token_type == TKN_PAREN_CLOSE || \
    token_type == TKN_COMMA ||       \
    token_type == TKN_FUNCTION ||    \
    token_type == TKN_MUTABLE ||     \
    token_type == TKN_TYPE_INT ||    \
    token_type == TKN_EOF ?          \
        VALUE_EMPTY :                \
        VALUE_INVALID                \
)

#define VALUE_INVALID -1
#define VALUE_EMPTY 0
#define VALUE_STRING 2
#define VALUE_INT 3

typedef enum {
    // symbolic
    TKN_ASSIGN,
    TKN_STMT_END,
    TKN_BLOCK_START,
    TKN_BLOCK_END,
    TKN_PAREN_OPEN,
    TKN_PAREN_CLOSE,
    TKN_COMMA,

    // reserved keywords
    TKN_FUNCTION,
    TKN_MUTABLE,
    TKN_TYPE_INT,

    // value string
    TKN_LIB_IDENTIFIER,
    TKN_IDENTIFIER,

    // value int
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
void print_tokens(Token *tokens);

#endif
