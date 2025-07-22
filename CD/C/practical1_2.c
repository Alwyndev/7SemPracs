#include <stdio.h>
#include <stdlib.h>

struct Token{
    char *type;
    char* value;
};

struct Token* createToken(char *type, char *value) {
    struct Token* token = (struct Token*)malloc(sizeof(struct Token));
    token->type = type;
    token->value = value;
    return token;
}

void freeToken(struct Token* token) {
    if (token) {
        free(token->value);
        free(token);
    }
}

void printToken(struct Token* token) {
    if (token) {
        printf("Token Type: %s, Value: %s\n", token->type, token->value);
    }
}

struct keyword{
    char* name;
};
int main() {
    // Example usage
    struct Token* token = createToken("IDENtIFIER", "myVariable");
    printToken(token);
    freeToken(token);
    return 0;


}