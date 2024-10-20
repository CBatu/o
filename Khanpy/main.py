from Lexer.Lexer import Lexer
from Token.Token import TokenType
from Parser.Parser import Parser

def main():
    text = ""
    lexer = Lexer(text)

    parser = Parser(lexer)

    gen =parser.parse()
    while gen is not None:
        print(gen)
        gen = parser.parse()

if __name__ == "__main__":
    main()
