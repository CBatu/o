
#include "Lex/lexer.h"
#include "Token.h"
#include "error.h"
#include "Parser/parser.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    const char *input = "if 1 { a := 12 + 212 *2; } elif 1 { a := 1234; }";
    Lexer lexer;
    initLexer(&lexer, input);


    Parser parser;
    initParser(&parser, &lexer);

    char *code = gen(&parser);

    
    printf("%s", code);
    free(code);
    return 0;
}
