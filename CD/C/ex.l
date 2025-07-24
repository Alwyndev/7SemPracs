%option noyywrap
%{
    #include <stdio.h>
    #include <string.h>

    // A list of C keywords to match against
    char *keywords[] = {
        "int", "float", "return", "if", "else", "while", "for", "void", "char", "double"
    };

    int isKeyword(const char* word) {
        for (int i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++) {
            if (strcmp(word, keywords[i]) == 0)
                return 1;
        }
        return 0;
    }
%}

%%

[ \t\n]+             ; // Ignore whitespace

[0-9]+\.[0-9]+       { printf("Float: %s\n", yytext); }
[0-9]+               { printf("Integer: %s\n", yytext); }

[a-zA-Z_][a-zA-Z0-9_]* {
                        if (isKeyword(yytext))
                            printf("Keyword: %s\n", yytext);
                        else
                            printf("Identifier: %s\n", yytext);
                     }

.                    { printf("Other: %s\n", yytext); }

%%

int main() {
    yylex();
    return 0;
}
