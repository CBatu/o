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
