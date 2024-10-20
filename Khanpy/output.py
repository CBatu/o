def format_c_code(code):
    formatted_code = ""
    indent_level = 0
    indent_space = "    "  
    i = 0
    while i < len(code):
        char = code[i]
        if char == '{':
            indent_level += 1
            formatted_code += " {\n" + indent_space * indent_level
        elif char == '}':
            indent_level -= 1
            formatted_code += "\n" + indent_space * indent_level + "}"
        elif char == ';':
            formatted_code += ";\n" + indent_space * indent_level
        else:
            formatted_code += char
        i += 1
    return formatted_code.strip()


def main():
    text = open("input.kh", "r").read()
    lexer = Lexer(text)

    parser = Parser(lexer)

    gen =parser.parse()
    code = ""
    while gen is not None:
        code += gen
        print(gen)
        gen = parser.parse()

    c = open("output.c", "w").write(format_c_code(code))
if __name__ == "__main__":
    main()


# File: ./Token/Token.py
from enum import Enum, auto

class TokenType(Enum):
    INT=auto()
    PLUS=auto()
    MINUS=auto()
    MUL=auto()
    DIV=auto()
    IDENTIFIER=auto()
    SEMICOLON=auto()
    COMMA=auto()
    LPAREN=auto()
    RPAREN=auto()
    LBRACE=auto()
    LET=auto()
    RBRACE=auto()
    DOT=auto()
    EQUALS=auto()
    EQUALS_EQUALS=auto()  # ==
    NOT_EQUALS=auto()     # !=
    LT=auto()             # <
    GT=auto()             # >
    LTE=auto()            # <=
    GTE=auto()            # >=
    TTDOT=auto()
    AUTO=auto()
    IF=auto()
    ELSE=auto()
    ELIF=auto()
    IMPORT=auto()
    HASHTAG=auto()
    INCLUDE=auto()
    FUNC=auto()
    TO=auto()
    STRING=auto()
    RETURN=auto()
    EOF=auto()


    def __str__(self):
        return self.name
    
    def __repr__(self):
        return self.name
    
    def __eq__(self, other):
        return self.value == other
    
    def __ne__(self, other):
        return self.value != other
    
    def __lt__(self, other):
        return self.value < other

    def __gt__(self, other):
        return self.value > other

class Token:
    def __init__(self, token_type, token_value):
        self.type = token_type
        self.value = token_value

    def __str__(self):
        return f'Token({self.type}, {self.value})'
    
    def __repr__(self):
        return self.__str__()


# File: ./Lexer/Lexer.py
class Lexer:
    def __init__(self, text):
        self.text = text
        self.pos = 0
        self.cur = self.text[self.pos] if self.text else None

    def advance(self):
        self.pos += 1
        if self.pos < len(self.text):
            self.cur = self.text[self.pos]
        else:
            self.cur = None  # EOF durumu

    def nextToken(self):
        while self.cur is not None:  # Boşlukları atla
            if self.cur.isspace():
                self.advance()
                continue

            # İki karakterli operatörleri kontrol et
            if self.cur == '=':
                self.advance()
                if self.cur == '=':
                    self.advance()
                    return Token(TokenType.EQUALS_EQUALS, '==')
                return Token(TokenType.EQUALS, '=')
            if self.cur == '!':
                self.advance()
                if self.cur == '=':
                    self.advance()
                    return Token(TokenType.NOT_EQUALS, '!=')
                raise Exception(f'Invalid token: {self.cur}')
            if self.cur == '>':
                self.advance()
                if self.cur == '=':
                    self.advance()
                    return Token(TokenType.GTE, '>=')
                return Token(TokenType.GT, '>')
            if self.cur == '<':
                self.advance()
                if self.cur == '=':
                    self.advance()
                    return Token(TokenType.LTE, '<=')
                return Token(TokenType.LT, '<')
            match self.cur:
                case '+':
                    self.advance()
                    return Token(TokenType.PLUS, '+')
                case '-':
                    self.advance()
                    if self.cur == '>':
                        self.advance()
                        return Token(TokenType.TO, '->')
                    return Token(TokenType.MINUS, '-')
                case '*':
                    self.advance()
                    return Token(TokenType.MUL, '*')
                case '/':
                    self.advance()
                    return Token(TokenType.DIV, '/')
                case ';':
                    self.advance()
                    return Token(TokenType.SEMICOLON, ';')
                case ',':
                    self.advance()
                    return Token(TokenType.COMMA, ',')
                case '(':
                    self.advance()
                    return Token(TokenType.LPAREN, '(')
                case ')':
                    self.advance()
                    return Token(TokenType.RPAREN, ')')
                case '{':
                    self.advance()
                    return Token(TokenType.LBRACE, '{')
                case '}':
                    self.advance()
                    return Token(TokenType.RBRACE, '}')
                case '.':
                    self.advance()
                    return Token(TokenType.DOT, '.')
                case '#':
                    self.advance()
                    return Token(TokenType.HASHTAG, '#')
                case '\"':
                    self.advance()
                    value = ''
                    while self.cur is not None and self.cur != '\"':
                        value += self.cur
                        self.advance()
                    self.advance()
                    return Token(TokenType.IDENTIFIER, f"\"{value}\"")
                case ':':
                    self.advance()
                    if self.cur == '=':
                        self.advance()
                        return Token(TokenType.AUTO, ':=')
                    else:
                        return Token(TokenType.TTDOT, ':')
                case _:
                    if self.cur.isdigit():
                        value = ''
                        while self.cur is not None and self.cur.isdigit():
                            value += self.cur
                            self.advance()
                        return Token(TokenType.INT, value)
                    elif self.cur.isalpha():
                        value = ''
                        while self.cur is not None and self.cur.isalpha():
                            value += self.cur
                            self.advance()
                        match value:
                            case 'if':
                                return Token(TokenType.IF, value)
                            case 'else':
                                return Token(TokenType.ELSE, value)
                            case 'elif':
                                return Token(TokenType.ELIF, value)
                            case 'let':
                                return Token(TokenType.LET, value)
                            case 'import':
                                return Token(TokenType.IMPORT, value)
                            case 'include':
                                return Token(TokenType.INCLUDE, value)
                            case 'func':
                                return Token(TokenType.FUNC, value)
                            case 'return':
                                return Token(TokenType.RETURN, value)
                            case _:
                                return Token(TokenType.IDENTIFIER, value)
                    else:
                        raise Exception(f'Invalid token: {self.cur}')
        
        return Token(TokenType.EOF, None)  # EOF token'ını döndür


# File: ./Parser/Parser.py
class Parser:
    def __init__(self, lexer):
        self.lexer: Lexer = lexer
        self.current_token: Token = self.lexer.nextToken()

    def nextToken(self):
        self.current_token = self.lexer.nextToken()

    def parse(self):
        match self.current_token.type:
            case TokenType.LET:
                return self.parseLetStatement()
            case TokenType.IDENTIFIER:
                return self.parseIdentifier()
            case TokenType.IF:
                return self.parseIfStatement()
            case TokenType.IMPORT:
                return self.parseImportStatement()
            case TokenType.HASHTAG:
                self.nextToken()
                if self.current_token.type == TokenType.INCLUDE:
                    return self.parseIncludeStatement()
                else:
                    self.error(f"Unexpected token {self.current_token.type}")
            case TokenType.FUNC:
                return self.parseFuncStatement()
            case TokenType.RETURN:
                return self.parseReturnStatement()
            case TokenType.EOF:
                return None
            case _:
                self.error(f"Unexpected token {self.current_token.type}")

    def parseReturnStatement(self):
        self.eat(TokenType.RETURN)
        value = self.parseExpression(TokenType.SEMICOLON)
        self.eat(TokenType.SEMICOLON)
        return f"return {value};\n"

    def parseFuncStatement(self):
        
        self.eat(TokenType.FUNC)
        name = self.parseExpression(TokenType.LPAREN)
        args = ""
        while self.current_token.type != TokenType.LBRACE:
            if self.current_token.type == TokenType.TO:
                break
            else:
                args += self.current_token.value
                self.nextToken()

        if self.current_token.type == TokenType.TO:
            self.nextToken()
            value = self.parseExpression(TokenType.LBRACE)
            self.eat(TokenType.LBRACE)
            consequence = self.parseBlockStatement()
            self.eat(TokenType.RBRACE)
            return f"{value} {name}{args} {{ {consequence} }}"
        elif self.current_token.type == TokenType.LBRACE:
            self.eat(TokenType.LBRACE)
            consequence = self.parseBlockStatement()
            self.eat(TokenType.RBRACE)
            return f"void {name}{args} {{ {consequence} }}"

    def parseImportStatement(self):
        self.eat(TokenType.IMPORT)
        name = self.parseExpression(TokenType.SEMICOLON)
        self.eat(TokenType.SEMICOLON)
        name += ".c"
        return f"#include \"{name}\"\n"

    def parseIncludeStatement(self):
        self.eat(TokenType.INCLUDE)
        name = self.parseExpression(TokenType.SEMICOLON)
        self.eat(TokenType.SEMICOLON)
        return f"#include \"{name}\"\n"
    
    

    def parseIfStatement(self):
        self.eat(TokenType.IF)
        condition = self.parseExpression(TokenType.LBRACE)
        self.eat(TokenType.LBRACE)
        consequence = self.parseBlockStatement()
        
        if self.current_token.type == TokenType.RBRACE:
            self.eat(TokenType.RBRACE)
        else:
            self.error(f"Missing closing brace for if block")
        
        result = f"if ({condition}) {{\n {consequence} }}\n"
        
        # Parse 'else if' blocks
        while self.current_token.type == TokenType.ELIF:
            self.eat(TokenType.ELIF)
            condition = self.parseExpression(TokenType.LBRACE)
            self.eat(TokenType.LBRACE)
            consequence = self.parseBlockStatement()
            
            if self.current_token.type == TokenType.RBRACE:
                self.eat(TokenType.RBRACE)
            else:
                self.error(f"Missing closing brace for elif block")
            
            result += f" else if ({condition}) {{\n {consequence} }}\n"
        
        # Parse 'else' block
        if self.current_token.type == TokenType.ELSE:
            self.eat(TokenType.ELSE)
            self.eat(TokenType.LBRACE)
            consequence = self.parseBlockStatement()
            
            if self.current_token.type == TokenType.RBRACE:
                self.eat(TokenType.RBRACE)
            else:
                self.error(f"Missing closing brace for else block")
            
            result += f" else {{\n {consequence} }}\n"
        
        return result

    def parseBlockStatement(self):
        block = ""
        while self.current_token.type != TokenType.RBRACE and self.current_token.type != TokenType.EOF:
            statement = self.parse()
            if statement:
                block += statement + " "
        return block.strip()

    def parseLetStatement(self):
        self.eat(TokenType.LET)
        name = self.parseExpression(TokenType.EQUALS)
        checkvalue = name.split(":")
        
        if len(checkvalue) >= 2:
            name = checkvalue[0]
            typevalue = checkvalue[1]
        else:
            typevalue = "auto"
        
        self.eat(TokenType.EQUALS)
        value = self.parseExpression(TokenType.SEMICOLON)
        self.eat(TokenType.SEMICOLON)
        
        return f"{typevalue} {name} = {value};\n"
    
    def parseIdentifier(self):
        name = ""
        while self.current_token.type not in [TokenType.EQUALS, TokenType.LPAREN, TokenType.AUTO]:
            name += self.current_token.value
            self.nextToken()
        
        match self.current_token.type:
            case TokenType.EQUALS:
                self.eat(TokenType.EQUALS)
                value = self.parseExpression(TokenType.SEMICOLON)
                self.eat(TokenType.SEMICOLON)
                return f"{name} = {value};"
            case TokenType.LPAREN:
                value = self.parseExpression(TokenType.SEMICOLON)
                self.eat(TokenType.SEMICOLON)
                return f"{name}{value};"
            case TokenType.AUTO:
                self.eat(TokenType.AUTO)
                value = self.parseExpression(TokenType.SEMICOLON)
                self.eat(TokenType.SEMICOLON)
                return f"auto {name} = {value};\n"

    def parseExpression(self, stop_token_type):
        value = ""
        while self.current_token.type != stop_token_type and self.current_token.type != TokenType.EOF:
            value += self.current_token.value
            self.nextToken()
        return value.strip()

    def eat(self, token_type):
        if self.current_token.type == token_type:
            self.nextToken()
        else:
            self.error(f"Expected token {token_type}, got {self.current_token.type}")

    def error(self, message):
        raise Exception(f"Parser Error: {message}")


