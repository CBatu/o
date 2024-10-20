from Lexer.Lexer import Lexer
from Parser.Parser import Parser


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
