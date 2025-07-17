#include <stdio.h>
#include <string.h>

int isAlpha(char c){
    if ((c >='A' && c <= 'Z') || (c >= 'a' && c <= 'z')){
        return 1;
    }
    return 0;
}

int isDigit(char c){
    if (c >='0' && c <= '9'){
        return 1;
    }
    return 0; 
}

int isAlNum(char c){
    return isAlpha(c) || isDigit(c);
}

int isSpace(char c){
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r'){
        return 1;
    }
    return 0;
}

int isOperator(char c){
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || 
        c == '='){
        return 1;
    }
    return 0;
}

int isValidString(char* str){
    int len = strlen(str);
    for (int i = 0; i < len; i++){
        if (!isAlNum(str[i]) && !isSpace(str[i]) && !isOperator(str[i])){
            return 0; // Invalid character found
        }
    }
    return 1;
}



int main(){
    char str[100];
    printf("Enter a string: ");
    fgets(str, sizeof(str), stdin);

    if (isValidString(str)){
        printf("Valid string.\n");
    } else {
        printf("Invalid string.\n");
    }

    return 0;
}