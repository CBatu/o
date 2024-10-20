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
            case _:
                return None
            
    def parseLetStatement(self):
        self.eat(TokenType.LET)  
        name = self.parseExpression(TokenType.EQUALS)
        checkvalue = name.split(":")
        if len(checkvalue) >= 2:
            name = checkvalue[0]
            typevalue = checkvalue[1]
            print(self.current_token)
        else:
            typevalue = "auto"
            
        self.eat(TokenType.EQUALS)  
        value = self.parseExpression(TokenType.SEMICOLON)  
        self.eat(TokenType.SEMICOLON)  
        return f"{typevalue} {name} = {value}"
    
    def parseIdentifier(self):
        name = ""
        while self.current_token.type != TokenType.EQUALS and self.current_token.type != TokenType.LPAREN and self.current_token.type != TokenType.AUTO:
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
                return f"auto {name} = {value};"


    def parseExpression(self, stop_token_type):
        value = ""
        
        while self.current_token.type != stop_token_type and self.current_token.type != TokenType.EOF:
            value += self.current_token.value  
            self.nextToken() 

        return value.strip() 

    def eat(self, token_type):
        if self.current_token.type == token_type:
            value = self.current_token
            self.nextToken()
            return value
        else:
            self.error(f"Expected token {token_type}, got {self.current_token.type}")

    def error(self, message):
        raise Exception(f"Parser Error: {message}")
