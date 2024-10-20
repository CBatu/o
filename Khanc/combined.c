
// Content of include/error.h
#ifndef ERROR_H
#define ERROR_H

#define MAX_ERRORS 100*2
#define MAX_MESSAGE_LENGTH 256

typedef enum {
    LEXICAL_ERROR,
    SYNTAX_ERROR,
    SEMANTIC_ERROR,
    RUNTIME_ERROR
} ErrorType;

typedef struct {
    ErrorType type;
    char message[MAX_MESSAGE_LENGTH];
    int line;
    int column;
} Error;

void error(ErrorType type, int line, int column,const  char *format, ...);
void checkError();

#endif


// Content of include/Token.h
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
    TDOT
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


// Content of include/Lex/lexer.h
#ifndef LEXER_H
#define LEXER_H


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


// Content of include/util/color.h
#ifndef COLOR_H
#define COLOR_H

#include <stdarg.h> 

// Renk ve stil kodları
#define RESET   "\033[0m"        // Reset
#define BOLD    "\033[1m"        // Kalın
#define UNDERLINE "\033[4m"      // Altı çizili

// Renk kodları
#define RED     "\033[31m"       // Kırmızı
#define GREEN   "\033[32m"       // Yeşil
#define YELLOW  "\033[33m"       // Sarı
#define BLUE    "\033[34m"       // Mavi
#define MAGENTA "\033[35m"       // Magenta
#define CYAN    "\033[36m"       // Cyan
#define WHITE   "\033[37m"       // Beyaz

// Fonksiyonlar
void printColored(const char *color, const char *style, const char *format, ...);

#endif // COLOR_H


// Content of include/Parser/parser.h
#ifndef PARSER_H
#define PARSER_H

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


// Content of src/main.c
#include <stdio.h>
#include <stdlib.h>

int main() {
    const char *input = "'";

    Lexer lexer;
    initLexer(&lexer, input);


    Parser parser;
    initParser(&parser, &lexer);

    char *code = gen(&parser);

    
    printf("%s", code);
    free(code);
    return 0;
}


// Content of src/error.c
#include <stdio.h>
#include <stdlib.h>

static Error errors[MAX_ERRORS];
static int totalMessages = 0;

#include <stdarg.h> 

void error(ErrorType type, int line, int column, const char *format, ...) {
    if (totalMessages < MAX_ERRORS) {
        va_list args;
        va_start(args, format);

        vsnprintf(errors[totalMessages].message, MAX_MESSAGE_LENGTH, format, args);
        errors[totalMessages].type = type;
        errors[totalMessages].line = line;
        errors[totalMessages].column = column;

        va_end(args);

        totalMessages++;
    } else if (totalMessages == MAX_ERRORS) {
        printColored(RED, "", "Error limit exceeded! Too many errors at line %d, column %d.\n", line, column);
        exit(1);
    }
}

const char* getErrorMessage(ErrorType type, const char *message) {
    static char detailedMessage[1024]; 
    switch (type) {
        case LEXICAL_ERROR:
            snprintf(detailedMessage, sizeof(detailedMessage), 
                     "Lexical Error: %s", message);
            break;
        case SYNTAX_ERROR:
            snprintf(detailedMessage, sizeof(detailedMessage), 
                     "Syntax Error: %s", message);
            break;
        case SEMANTIC_ERROR:
            snprintf(detailedMessage, sizeof(detailedMessage), 
                     "Semantic Error: %s", message);
            break;
        case RUNTIME_ERROR:
            snprintf(detailedMessage, sizeof(detailedMessage), 
                     "Runtime Error: %s", message);
            break;
        default:
            snprintf(detailedMessage, sizeof(detailedMessage), "Unknown Error: %s", message);
            break;
    }
    return detailedMessage;
}

void checkError() {
    if (totalMessages > 0) {
        for (int i = 0; i < totalMessages; i++) {
            printColored(RED, "", "error: ");
            
            // Hata detayları kalın yazı ve normal renk olacak
            printColored("", BOLD, "%s at line %d, column %d\n", 
                         getErrorMessage(errors[i].type, errors[i].message), 
                         errors[i].line, 
                         errors[i].column);
        }
        exit(1);
    }
}


// Content of src/Lex/lexer.c
#include <ctype.h>  
#include <stdlib.h> 
#include <string.h> 

void initLexer(Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->cur = 0;
    lexer->line = 1;
    lexer->column = 1;
}

Token nextToken(Lexer *lexer) {
    char c;

    while ((c = lexer->input[lexer->cur]) != '\0') {
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
            case '\n':
                lexer->cur++;
                lexer->line++;
                lexer->column = 1;
                break;
            case ' ':
            case '\t':
                lexer->cur++;
                lexer->column++;
                break;
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
                } else if( isalnum(c) ) {
                    int start = lexer->cur;
                    while (isalnum(lexer->input[lexer->cur])) {
                        lexer->cur++;
                        lexer->column++;
                    }

                    int length = lexer->cur - start;
                    char *value = (char *)malloc(length + 1);
                    strncpy(value, &lexer->input[start], length);
                    value[length] = '\0';
                    if (strcmp(value, "let") == 0) {
                        Token token = createToken(LET, value);
                        free(value);
                        return token;
                    }else {
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


// Content of src/util/color.c
#include <stdio.h>

void printColored(const char *color, const char *style, const char *format, ...) {
    va_list args;
    va_start(args, format);

    // Stil ve renk ile başla
    printf("%s%s", style, color);

    // Formatlı mesajı yazdır
    vprintf(format, args);

    // Renk sıfırlama kodunu yazdır
    printf("%s", RESET);

    va_end(args);
}


// Content of src/Parser/parser.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



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


string parse(Parser *parser) {
    string result = NULL;
    switch (parser->currentToken.type) {
        case LET:

            result = parseLetStatement(parser);
            break;
        case IDENTIFIER:
            result = parseIdentifier(parser);
            break;
        case END_OF_FILE:
            result = strdup("// EOF\n");
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
        advance(parser);
    }
    
    
    return code;
}

