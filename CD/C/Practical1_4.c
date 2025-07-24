#include <stdio.h>
#include <string.h>

int main() {
    char program[1000];

    printf("Enter C source code: ");
    fgets(program, sizeof(program), stdin);

    printf("Found literals:\n");

    for (int i = 0; program[i] != '\0'; i++) {
        if (program[i] == '"') {
            printf("String literal: ");
            i++;
            while (program[i] != '"' && program[i] != '\0') {
                putchar(program[i]);
                i++;
            }
            printf("\n");
        } else if (program[i] == '\'') {
            printf("Character literal: ");
            i++;
            while (program[i] != '\'' && program[i] != '\0') {
                putchar(program[i]);
                i++;
            }
            printf("\n");
        }
    }

    return 0;
}
