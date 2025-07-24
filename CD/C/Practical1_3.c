#include <stdio.h>
#include <ctype.h>
#include <string.h>

int isDelimiter(char ch) {
    // Common delimiters
    return (ch == ';' || ch == ',' || ch == '(' || ch == ')' || ch == '{' || ch == '}' ||
            ch == '[' || ch == ']' || ch == ':' || ch == '.');
}

int isPunctuation(char ch) {
    // Common punctuation marks
    return (ch == '!' || ch == '?' || ch == '\'' || ch == '"' || ch == '-' || ch == '#' || ch == '@' || ch == '/');
}

int main() {
    char input[1000];

    printf("Enter the text/code (end input with a single $ on a new line):\n");

    // Read multiline input until "$"
    char temp[200];
    input[0] = '\0';
    while (1) {
        fgets(temp, sizeof(temp), stdin);
        if (temp[0] == '$' && temp[1] == '\n') break;
        strcat(input, temp);
    }

    printf("\nIdentified Delimiters and Punctuation:\n");

    for (int i = 0; input[i] != '\0'; i++) {
        if (isDelimiter(input[i])) {
            printf("Delimiter: '%c' at position %d\n", input[i], i);
        } else if (isPunctuation(input[i])) {
            printf("Punctuation: '%c' at position %d\n", input[i], i);
        }
    }

    return 0;
}
