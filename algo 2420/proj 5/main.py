from stack import Stack

def eval_postfix(expr):
    if not isinstance(expr, str):
        raise ValueError
    
    if len(expr) <= 2:
        return int(expr)

    stack = Stack()
    index = 0

    for i in range(0, len(expr), 2):
        if expr[i].isnumeric(): # next input is a number
            stack.push(int(expr[i]))
        else: # next input is an operation
            try:
                num1 = stack.pop()
                num2 = stack.pop()
            except IndexError:
                raise SyntaxError
            
            if expr[i] == "+": ans = num2 + num1
            elif expr[i] == "-": ans = num2 - num1
            elif expr[i] == "*": ans = num2 * num1
            elif expr[i] == "/": ans = num2 / num1
            else: raise SyntaxError
            stack.push(ans)
    
    if stack.size() > 1: raise SyntaxError
    return stack.pop()


def in2post(expr):
    if not isinstance(expr, str):
        raise ValueError

    stack = Stack()
    out = ""
    expr.strip()
    expr.replace(" ", "")

    for i in range(len(expr)):
        c = expr[i]
        if c == "\n": continue

        if c == "(":
            stack.push(c)
        elif c.isnumeric():
            out += c + " "
        elif c in "+-*/":
            while stack.size() != 0 and stack.top() != "(" and not (stack.top() in "+-" and c in "*/"):
                out += stack.pop() + " "
            stack.push(c)
        elif c == ")":
            out += stack.pop() + " "
            while stack.top() != "(":
                try:
                    out += stack.pop() + " "
                except IndexError:
                    raise SyntaxError
            stack.pop() # left parenthesis
        else: pass

    while stack.size() != 0:
        if stack.top() == "(": raise SyntaxError
        out += stack.pop() + " "

    out.strip()
    return out
        


def main():
    txt = open("algo\proj 5\data.txt", "r")
    for infix in txt:
        print(f"infix: {infix}")

        postfix = in2post(infix)
        print(f"postfix: {postfix}")

        answer = eval_postfix(postfix)
        print(f"answer: {answer:.1f}")
        print()
    
if __name__=="__main__":
    main()