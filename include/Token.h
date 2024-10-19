#ifndef TOKEN_H
#define TOKEN_H


typedef enum {
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    INT,
    STRING,
    IDENTIFIER,
    LET,
    END_OF_FILE,
    EQUALS,
    SEMICOLON,
    LPAREN,
    RPAREN,
    COMMA,
    TTEQUALS,
    TDOT,
    IF,
    ELSE_IF,
    ELSE,
    WHILE,
    LBRACE,
    RBRACE
} TokenType;

typedef struct {
    TokenType type;  
    char *value;
} Token;

static inline char* getTokenTypeString(TokenType type) {
    switch (type) {
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case MULTIPLY: return "MULTIPLY";
        case DIVIDE: return "DIVIDE";
        case INT: return "INT";
        case STRING: return "STRING";
        case IDENTIFIER: return "IDENTIFIER";
        case LET: return "LET";
        case END_OF_FILE: return "END_OF_FILE";
        case EQUALS: return "EQUALS";
        case SEMICOLON: return "SEMICOLON";
        case LPAREN: return "LPAREN";
        case RPAREN: return "RPAREN";
        case COMMA: return "COMMA";
        case TTEQUALS: return "TTEQUALS";
        case TDOT: return "TDOT";
        case IF: return "IF";
        case ELSE_IF: return "ELSE IF";
        case ELSE: return "ELSE";
        case WHILE: return "WHILE";
        case LBRACE: return "LBRACE";
        case RBRACE: return "RBRACE";
        default: return "UNKNOWN";
    }
}

static inline Token createToken(TokenType type,char *value) {
    Token token;
    token.type = type;
    token.value = value;
    return token;
}


#endif
