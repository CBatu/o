from Token.Token import TokenType, Token

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
