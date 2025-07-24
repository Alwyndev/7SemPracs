#include <stdio.h>
#include <string.h>
#include <ctype.h>

const char *keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default",
    "do", "double", "else", "enum", "extern", "float", "for", "goto",
    "if", "int", "long", "register", "return", "short", "signed",
    "sizeof", "static", "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while"
};

#define TOTAL_KEYWORDS (sizeof(keywords)/sizeof(keywords[0]))

int isKeyword(const char *word) {
    for (int i = 0; i < TOTAL_KEYWORDS; i++) {
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

int main() {
    char program[1000], word[100];
    int keywordCount = 0;
    int i = 0, j = 0;

    printf("Enter C source code: ");
    fgets(program, sizeof(program), stdin);

    // Parse the input string word by word
    while (i < strlen(program)) {
        // Skip whitespace and special characters
        while (i < strlen(program) && (!isalnum(program[i]) && program[i] != '_')) {
            i++;
        }
        
        // Extract word (identifier/keyword)
        j = 0;
        while (i < strlen(program) && (isalnum(program[i]) || program[i] == '_')) {
            word[j++] = program[i++];
        }
        word[j] = '\0';
        
        // Check if it's a keyword
        if (j > 0 && isKeyword(word)) {
            keywordCount++;
        }
    }

    printf("Total number of keywords: %d\n", keywordCount);
    return 0;
}
