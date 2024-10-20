#include "Lex/lexer.h"
#include "error.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "Token.h"
#include <stdio.h>

void initLexer(Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->cur = 0;
    lexer->line = 1;
    lexer->column = 1;
}

Token nextToken(Lexer *lexer) {
    char c;

    while ((c = lexer->input[lexer->cur]) != '\0') {
        // Beyaz boşlukları atla
        if (isspace(c)) {
            if (c == '\n') {
                lexer->line++;
                lexer->column = 1;
            } else {
                lexer->column++;
            }
            lexer->cur++;
            continue;
        }

        switch (c) {
            case '+':
                lexer->cur++;
                lexer->column++;
                return createToken(PLUS, "+");
            case '-':
                lexer->cur++;
                lexer->column++;
                return createToken(MINUS, "-");
            case '*':
                lexer->cur++;
                lexer->column++;
                return createToken(MULTIPLY, "*");
            case '/':
                lexer->cur++;
                lexer->column++;
                return createToken(DIVIDE, "/");
            case ';':
                lexer->cur++;
                lexer->column++;
                return createToken(SEMICOLON, ";");
            case '=':
                lexer->cur++;
                lexer->column++;
                return createToken(EQUALS, "=");
            case '(':
                lexer->cur++;
                lexer->column++;
                return createToken(LPAREN, "(");
            case ')':
                lexer->cur++;
                lexer->column++;
                return createToken(RPAREN, ")");
            case ',':
                lexer->cur++;
                lexer->column++;
                return createToken(COMMA, ",");
            case ':':
                if (lexer->input[lexer->cur + 1] == '=') {
                    lexer->cur += 2;
                    lexer->column += 2;
                    return createToken(TTEQUALS, ":=");
                } else {
                    lexer->cur++;
                    lexer->column++;
                    return createToken(TDOT, ":");
                }
            case '{':
                lexer->cur++;
                lexer->column++;
                return createToken(LBRACE, "{");
            case '}':
                lexer->cur++;
                lexer->column++;
                return createToken(RBRACE, "}");
            default:
                if (isdigit(c)) {
                    int start = lexer->cur;
                    while (isdigit(lexer->input[lexer->cur])) {
                        lexer->cur++;
                        lexer->column++;
                    }

                    int length = lexer->cur - start;
                    char *value = (char *)malloc(length + 1);
                    strncpy(value, &lexer->input[start], length);
                    value[length] = '\0';

                    Token token = createToken(INT, value);
                    free(value);

                    return token;
                }
                else if (isalnum(c) || c == '_') {
                    int start = lexer->cur;
                    while (isalnum(lexer->input[lexer->cur]) || lexer->input[lexer->cur] == '_') {
                        lexer->cur++;
                        lexer->column++;
                    }

                    int length = lexer->cur - start;
                    char *value = (char *)malloc(length + 1);
                    strncpy(value, &lexer->input[start], length);
                    value[length] = '\0';

                    // Anahtar kelimeleri kontrol et
                    if (strcmp(value, "let") == 0) {
                        Token token = createToken(LET, value);
                        free(value);
                        return token;
                    }
                    else if (strcmp(value, "elif") == 0) {
                        Token token = createToken(ELSE_IF, value);
                        free(value);
                        return token;
                    }
                    else if (strcmp(value, "if") == 0) {
                        Token token = createToken(IF, value);
                        free(value);
                        return token;
                    }
                    else if (strcmp(value, "else") == 0) {
                        Token token = createToken(ELSE, value);
                        free(value);
                        return token;
                    }
                    else if (strcmp(value, "while") == 0) {
                        Token token = createToken(WHILE, value);
                        free(value);
                        return token;
                    }
                    else {
                        Token token = createToken(IDENTIFIER, value);
                        free(value);
                        return token;
                    }
                }
                else {
                    error(LEXICAL_ERROR, lexer->line, lexer->column, "Unexpected character '%c'", lexer->input[lexer->cur]);
                    lexer->cur++;
                    lexer->column++;
                }
        }
    }
    checkError(lexer->input);
    return createToken(END_OF_FILE, "EOF");
}
