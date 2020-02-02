# recursive_descent.py
#
# Example of recursive parsing, written in a functional style without
# modifying the parsed string or using global variables.
#
# B. Bird - 01/23/2020

class ParsingException(Exception):
    pass

class ExpressionNode:
    def evaluate(self,x):
        raise NotImplementedError

class AddNode(ExpressionNode):
    def __init__(self, left, right):
        self.left = left
        self.right = right
    def evaluate(self,x):
        return self.left.evaluate(x) + self.right.evaluate(x)


class SubtractNode(ExpressionNode):
    def __init__(self, left, right):
        self.left = left
        self.right = right
    def evaluate(self,x):
        return self.left.evaluate(x) - self.right.evaluate(x)


class MultiplyNode(ExpressionNode):
    def __init__(self, left, right):
        self.left = left
        self.right = right
    def evaluate(self,x):
        return self.left.evaluate(x) * self.right.evaluate(x)

class DivideNode(ExpressionNode):
    def __init__(self, left, right):
        self.left = left
        self.right = right
    def evaluate(self,x):
        return self.left.evaluate(x) / self.right.evaluate(x)

class XNode(ExpressionNode):
    def evaluate(self,x):
        return x

class ConstantNode(ExpressionNode):
    def __init__(self,c):
        self.c = c
    def evaluate(self,x):
        return self.c



def skip_whitespace(str, pos):
    while pos < len(str) and str[pos].isspace():
        pos += 1
    return pos

def parse_brackets_and_terminals(expression,pos):
    pos = skip_whitespace(expression, pos)
    if pos == len(expression):
        raise ParsingException("Missing symbol")
    if expression[pos] == '(':
        pos += 1
        node, pos = parse_add_subtract(expression, pos)
        pos = skip_whitespace(expression, pos)
        if pos == len(expression):
            raise ParsingException("Unmatched parenthesis")
        if expression[pos] != ')':
            raise ParsingException("Expected )")
        pos += 1
        return node, pos
    else:
        #Look for either the symbol 'x' or a sequence of digits
        #(we really should be searching for digits using a regular expression)
        if expression[pos] == 'x':
            node = XNode()
            pos += 1
            return node, pos
        digit_sequence = ''
        while pos < len(expression) and (expression[pos].isdigit() or expression[pos] == '.'):
            digit_sequence += expression[pos]
            pos += 1
        if len(digit_sequence) == 0:
            raise ParsingException("Expected constant")
        node = ConstantNode(float(digit_sequence))
        return node, pos

def parse_multiply_divide(expression, pos):
    #First pull in the left operand
    pos = skip_whitespace(expression, pos)
    left, pos = parse_brackets_and_terminals(expression, pos)
    while True:
        pos = skip_whitespace(expression, pos)
        if pos == len(expression) or expression[pos] not in ('*','/'):
            return left, pos
        op = expression[pos]
        pos += 1
        right, pos = parse_brackets_and_terminals(expression, pos)
        if op == '*':
            left = MultiplyNode(left, right)
        else:
            left = DivideNode(left,right)

def parse_add_subtract(expression, pos):
    #First pull in the left operand
    pos = skip_whitespace(expression, pos)
    left, pos = parse_multiply_divide(expression, pos)
    #Now, pull in additional operands as long as we find + or - signs connecting them
    #(we may find no + or - operators, in which case we return the left operand)
    while True:
        pos = skip_whitespace(expression, pos)
        if pos == len(expression) or expression[pos] not in ('+','-'):
            return left, pos
        op = expression[pos]
        pos += 1
        right, pos = parse_multiply_divide(expression, pos)
        #Now reassign left to be an expression node to prepare for the next iteration
        if op == '+':
            left = AddNode(left, right)
        else:
            left = SubtractNode(left,right)
    


def parse(expression):
    result,pos = parse_add_subtract(expression, 0)
    pos = skip_whitespace(expression, pos)
    if pos != len(expression):
        raise ParsingException("Erroneous trailing tokens")
    return result








if __name__ == '__main__':
    print("Enter an expression: ")
    expression = input()
    print("Enter a value for x: ")
    x = float(input())
    expression_tree = parse(expression)
    print("Result: %g"%expression_tree.evaluate(x))
