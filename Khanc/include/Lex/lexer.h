#ifndef LEXER_H
#define LEXER_H

#include "Token.h"

// Lexer struct ve fonksiyon bildirimleri
typedef struct {
    const char *input;
    int cur; 
    int line;
    int column;
} Lexer;

void initLexer(Lexer *lexer, const char *input);
Token nextToken(Lexer *lexer);

#endif // LEXER_H
