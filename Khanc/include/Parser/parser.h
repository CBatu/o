#ifndef PARSER_H
#define PARSER_H

#include "Lex/lexer.h"
#include "Token.h"

typedef char* string;

typedef struct {
    Lexer *lexer;        
    Token currentToken;  
} Parser;


void initParser(Parser *parser, Lexer *lexer);


void advance(Parser *parser);


void expect(Parser *parser, TokenType type);

string parseLetStatement(Parser *parser);

string parse(Parser *parser);

string gen(Parser *parser);

#endif
