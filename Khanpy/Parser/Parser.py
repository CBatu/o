from Lexer.Lexer import Lexer
from Token.Token import TokenType, Token

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
