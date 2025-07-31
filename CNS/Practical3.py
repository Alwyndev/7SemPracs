#68, 50

def euclidsGCD(a, b):
    while(b != 0):
        a, b = b, a % b
    return a

def extendedEuclidsGCD(a, b):
    if b == 0:
        return a, 1, 0
    gcd, x1, y1 = extendedEuclidsGCD(b, a % b)
    x = y1
    y = x1 - (a // b) * y1
    return gcd, x, y

a = int(input("Enter the first number : " ))
b = int(input("Enter the second number :  "))
print(extendedEuclidsGCD(a, b))