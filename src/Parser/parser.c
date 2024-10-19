#include "Lex/lexer.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include "Token.h"
#include <string.h>
#include "Parser/parser.h"



void initParser(Parser *parser, Lexer *lexer) {
    parser->lexer = lexer;
    parser->currentToken = nextToken(lexer);
}

void advance(Parser *parser) {
    parser->currentToken = nextToken(parser->lexer);
}

void expect(Parser *parser, TokenType type) {
    if (parser->currentToken.type != type) {
        char* normal =  getTokenTypeString(parser->currentToken.type);
        char* bold =  getTokenTypeString(type);
        error(SYNTAX_ERROR, parser->lexer->line, parser->lexer->column,
              "Expected token of type '%s' but got '%s' Token value: '%s'",
              bold, normal,
              parser->currentToken.value);
    } else {
        advance(parser);
    }
}

string old_strcat(const char* str1, const char* str2) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    char* result = (char*)malloc(len1 + len2 + 1);

    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    strcpy(result, str1);
    strcat(result, str2);

    return result;
}

string parseExpression(Parser *parser, TokenType type) {
    string value = (char *)malloc(256);  
    value[0] = '\0'; 
    value = (char *)realloc(value, strlen(value) + strlen(parser->currentToken.value) + 1); 
    strcat(value, parser->currentToken.value); 

    advance(parser);
    while (parser->currentToken.type != type && parser->currentToken.type != END_OF_FILE) {
        value = (char *)realloc(value, strlen(value) + strlen(parser->currentToken.value) + 1); 
        strcat(value, parser->currentToken.value); 
        advance(parser);
    }


    return value;
}

string parseLetStatement(Parser *parser) {
    expect(parser, LET);

    string name = parseExpression(parser, EQUALS);
    expect(parser, EQUALS);
    string value = parseExpression(parser, SEMICOLON);

    string type = strtok(name, ":");
    string variableName = strtok(NULL, ":"); 

    string result;
    if (variableName != NULL) {

        variableName = strtok(variableName, " \t"); 
        result = (char *)malloc(strlen(variableName) + strlen(type) + strlen(" = ") + strlen(value) + strlen(";\n") + 1);
        sprintf(result, "%s %s = %s;\n", variableName, type, value);
    } else {
        result = (char *)malloc(strlen("auto ") + strlen(name) + strlen(" = ") + strlen(value) + strlen(";\n") + 1);
        sprintf(result, "auto %s = %s;\n", name, value);
    }

    free(name);
    free(value);

    return result;
}


string parseIdentifier(Parser *parser) {
    string name = (char *)malloc(256);
    name[0] = '\0'; 
    while (parser->currentToken.type != EQUALS && parser->currentToken.type != LPAREN && parser->currentToken.type != SEMICOLON && parser->currentToken.type != TTEQUALS) {
        name = (char *)realloc(name, strlen(name) + strlen(parser->currentToken.value) + 1); 
        strcat(name, parser->currentToken.value); 
        advance(parser);
    }

    string result = NULL;
    switch (parser->currentToken.type) {
        case EQUALS: {
            advance(parser);
            string value = parseExpression(parser, SEMICOLON); 
            result = (char *)malloc(strlen(name) + strlen(" = ") + strlen(value) + strlen(";\n") + 1);
            sprintf(result, "%s = %s;\n", name, value);
            free(value); 
            break; 
        }
        case LPAREN: {
            string args = parseExpression(parser, SEMICOLON); 
            result = (char *)malloc(strlen(name) + strlen(args) + strlen(";\n") + 1);
            sprintf(result, "%s%s;\n", name, args);
            free(args);
            break; 
        }
        case TTEQUALS: {
            advance(parser);
            string value = parseExpression(parser, SEMICOLON); 
            result = (char *)malloc(strlen("auto ") + strlen(name) + strlen(" = ") + strlen(value) + strlen(";\n") + 1);
            sprintf(result, "auto %s = %s;\n", name, value);
            free(value); 
            break; 
        }
        default:
            error(SYNTAX_ERROR, parser->lexer->line, parser->lexer->column,
                  "Unexpected token type %s", getTokenTypeString(parser->currentToken.type));
            advance(parser);
            result = strdup("Error occurred during parsing");
            break;
    }

    free(name); 

    return result;
}
string parseIfStatement(Parser *parser) {
    expect(parser, IF);  
    string condition = parseExpression(parser, LBRACE);  

    expect(parser, LBRACE);  

    string body = strdup(""); 
    while (parser->currentToken.type != RBRACE && parser->currentToken.type != END_OF_FILE) {
        string temp = parse(parser); 
        string new_body = old_strcat(body, temp);  

        free(body);  
        free(temp);  
        body = new_body;  
    }
    
    expect(parser, RBRACE); 


    string result = (char *)malloc(strlen("if (") + strlen(condition) + strlen(") {\n") + strlen(body) + strlen("}\n") + 1);
    sprintf(result, "if (%s) {\n%s}\n", condition, body);

    free(condition);
    free(body);

    return result;
}


string parseELIfStatement(Parser *parser) {
    expect(parser, ELSE_IF);  
    string condition = parseExpression(parser, LBRACE);  

    expect(parser, LBRACE);  

    string body = strdup(""); 
    while (parser->currentToken.type != RBRACE && parser->currentToken.type != END_OF_FILE) {
        string temp = parse(parser); 
        string new_body = old_strcat(body, temp);  

        free(body);  
        free(temp);  
        body = new_body;  
    }
    
    expect(parser, RBRACE); 


    string result = (char *)malloc(strlen("else if (") + strlen(condition) + strlen(") {\n") + strlen(body) + strlen("}\n") + 1);
    sprintf(result, "else if (%s) {\n%s}\n", condition, body);

    free(condition);
    free(body);

    return result;
}


string parseElseStatement(Parser *parser) {
    expect(parser, ELSE);
    expect(parser, LBRACE);
    string result = (char *)malloc(strlen("else {\n") + 1);
    sprintf(result, "else {\n");
    return result;
}

string parse(Parser *parser) {
    string result = NULL;
    switch (parser->currentToken.type) {
        case LET:
            result = parseLetStatement(parser);
            break;
        case IDENTIFIER:
            result = parseIdentifier(parser);
            break;
        case IF:
            result = parseIfStatement(parser);
            break;
        case ELSE:
            result = parseElseStatement(parser);
            break;
        case ELSE_IF:
            result = parseELIfStatement(parser);
            break;
        case RBRACE:  
            result = strdup("}\n");  
            advance(parser);
            break;
        case END_OF_FILE:
            result = strdup("// EOF\n");
            break;
        case SEMICOLON:
            advance(parser);
            break;
        default:
            error(SYNTAX_ERROR, parser->lexer->line, parser->lexer->column,
                  "Unexpected token type %s", getTokenTypeString(parser->currentToken.type));
            advance(parser);
            result = strdup("Error occurred during parsing");
            break;
    }

    return result;
}


string gen(Parser *parser) {
    string code = strdup("");

    while (parser->currentToken.type != END_OF_FILE) {
        string temp = parse(parser);
        string new_code = old_strcat(code, temp);

        free(code); 
        free(temp);  

        code = new_code;
    }

    return code;
}

