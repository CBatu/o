#include "error.h"
#include "util/color.h"
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
