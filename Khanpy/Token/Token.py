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
