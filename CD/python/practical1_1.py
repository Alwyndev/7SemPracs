def isalphanum(s):
    if s not in 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789':
        return False
    return True

def isalpha(s):
    if s not in 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ':
        return False
    return True

def isdigit(s):
    
    if s not in '0123456789':
        return False
    return True

def isspace(s):
    if s != ' ':
        return False
    return True

def isoperator(s):
    if s not in '+-/*%^&_|=<>!':
        return False
    return True

if __name__ == '__main__':
    inp = input("Enter the string: ")
    i=0
    while i<len(inp):
        if not(isalphanum(inp[i]) or isalpha(inp[i]) or isdigit(inp[i]) or isspace(inp[i]) or isoperator(inp[i])):
            print(f"Invalid character found: {inp[i]}")
            break
        i += 1  
    else:
        print("All characters are valid.")