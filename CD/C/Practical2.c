/*
 * Symbol Table Implementation in C
 * This program demonstrates:
 * a) Extraction of declared identifiers and storing them in a symbol table
 * b) Display of identifier information in tabular format
 * c) Checking if identifiers are declared before use
 * d) Detection of multiple declarations of same identifiers
 * e) Reporting misuse of keywords as identifiers
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Maximum limits for symbol table and string lengths
#define MAX_IDENTIFIERS 100     // Maximum number of identifiers in symbol table
#define MAX_NAME_LENGTH 50      // Maximum length of identifier names
#define MAX_TYPE_LENGTH 20      // Maximum length of data type names
#define MAX_SCOPE_LENGTH 20     // Maximum length of scope names

// Structure to represent a single entry in the symbol table
typedef struct {
    char name[MAX_NAME_LENGTH];         // Identifier name
    char datatype[MAX_TYPE_LENGTH];     // Data type (int, float, etc.)
    char scope[MAX_SCOPE_LENGTH];       // Scope (global, function name, etc.)
    int line_number;                    // Line number where declared
    int declared;                       // Flag to indicate if declared (always 1 for valid entries)
} SymbolEntry;

// Structure to represent the entire symbol table
typedef struct {
    SymbolEntry entries[MAX_IDENTIFIERS];  // Array of symbol table entries
    int count;                              // Current number of entries
} SymbolTable;

// Global symbol table instance
SymbolTable symbolTable = {.count = 0};

// Array of C keywords - identifiers cannot have these names
const char* keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};
const int keyword_count = sizeof(keywords) / sizeof(keywords[0]);

// Array of C data types - used to identify declaration statements
const char* datatypes[] = {
    "int", "float", "double", "char", "void", "long", "short", "unsigned", "signed"
};
const int datatype_count = sizeof(datatypes) / sizeof(datatypes[0]);

// Function prototypes
int isKeyword(const char* word);
int isDatatype(const char* word);
int isValidIdentifier(const char* word);
int findIdentifier(const char* name, const char* scope);
void addIdentifier(const char* name, const char* datatype, const char* scope, int line_number);
void checkUndeclaredUsage(const char* name, const char* scope, int line_number);
void displaySymbolTable();
void processDeclaration(const char* line, const char* scope, int line_number);
void processStatement(const char* line, const char* scope, int line_number);

/*
 * Function: isKeyword
 * Purpose: Check if a given word is a C keyword
 * Parameters: word - string to check
 * Returns: 1 if keyword, 0 otherwise
 */
int isKeyword(const char* word) {
    // Linear search through keywords array
    for (int i = 0; i < keyword_count; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1;  // Found keyword
        }
    }
    return 0;  // Not a keyword
}

/*
 * Function: isDatatype
 * Purpose: Check if a given word is a C data type
 * Parameters: word - string to check
 * Returns: 1 if data type, 0 otherwise
 */
int isDatatype(const char* word) {
    // Linear search through datatypes array
    for (int i = 0; i < datatype_count; i++) {
        if (strcmp(word, datatypes[i]) == 0) {
            return 1;  // Found data type
        }
    }
    return 0;  // Not a data type
}

/*
 * Function: isValidIdentifier
 * Purpose: Check if a word is a valid C identifier
 * Rules: - Must start with letter or underscore
 *        - Rest can be letters, digits, or underscores
 *        - Cannot be a keyword
 * Parameters: word - string to validate
 * Returns: 1 if valid identifier, 0 otherwise
 */
int isValidIdentifier(const char* word) {
    if (!word || strlen(word) == 0) return 0;  // Null or empty string
    
    // First character must be letter or underscore
    if (!isalpha(word[0]) && word[0] != '_') return 0;
    
    // Rest must be alphanumeric or underscore
    for (int i = 1; i < strlen(word); i++) {
        if (!isalnum(word[i]) && word[i] != '_') return 0;
    }
    
    // Must not be a keyword - report error if it is
    if (isKeyword(word)) {
        printf("Error: '%s' is a keyword and cannot be used as an identifier\n", word);
        return 0;
    }
    
    return 1;  // Valid identifier
}

/*
 * Function: findIdentifier
 * Purpose: Search for an identifier in the symbol table within a specific scope
 * Parameters: name - identifier name to search for
 *            scope - scope to search in
 * Returns: index of entry if found, -1 otherwise
 */
int findIdentifier(const char* name, const char* scope) {
    // Linear search through symbol table entries
    for (int i = 0; i < symbolTable.count; i++) {
        // Match both name and scope
        if (strcmp(symbolTable.entries[i].name, name) == 0 && 
            strcmp(symbolTable.entries[i].scope, scope) == 0) {
            return i;  // Found at index i
        }
    }
    return -1;  // Not found
}

/*
 * Function: addIdentifier
 * Purpose: Add a new identifier to the symbol table
 * Checks: - Valid identifier format
 *         - No duplicate declarations in same scope
 *         - Symbol table not full
 * Parameters: name - identifier name
 *            datatype - data type of identifier
 *            scope - scope where declared
 *            line_number - line number of declaration
 */
void addIdentifier(const char* name, const char* datatype, const char* scope, int line_number) {
    // Validate identifier format
    if (!isValidIdentifier(name)) return;
    
    // Check for multiple declarations in same scope
    if (findIdentifier(name, scope) != -1) {
        printf("Error: Multiple declaration of identifier '%s' in scope '%s' at line %d\n", 
               name, scope, line_number);
        return;
    }
    
    // Check if symbol table is full
    if (symbolTable.count >= MAX_IDENTIFIERS) {
        printf("Error: Symbol table overflow\n");
        return;
    }
    
    // Add new entry to symbol table
    strcpy(symbolTable.entries[symbolTable.count].name, name);
    strcpy(symbolTable.entries[symbolTable.count].datatype, datatype);
    strcpy(symbolTable.entries[symbolTable.count].scope, scope);
    symbolTable.entries[symbolTable.count].line_number = line_number;
    symbolTable.entries[symbolTable.count].declared = 1;
    symbolTable.count++;
    
    printf("Added identifier: %s (%s) in scope %s at line %d\n", name, datatype, scope, line_number);
}

/*
 * Function: checkUndeclaredUsage
 * Purpose: Check if an identifier is being used without declaration
 * Checks current scope first, then global scope for the identifier
 * Parameters: name - identifier name being used
 *            scope - current scope
 *            line_number - line where identifier is used
 */
void checkUndeclaredUsage(const char* name, const char* scope, int line_number) {
    // Skip invalid identifiers
    if (!isValidIdentifier(name)) return;
    
    // Check current scope first, then global scope
    if (findIdentifier(name, scope) == -1 && findIdentifier(name, "global") == -1) {
        printf("Error: Undeclared identifier '%s' used at line %d\n", name, line_number);
    }
}

/*
 * Function: displaySymbolTable
 * Purpose: Display the symbol table in a formatted tabular layout
 * Shows: Identifier name, data type, scope, and line number
 */
void displaySymbolTable() {
    printf("\n=== SYMBOL TABLE ===\n");
    // Print table headers
    printf("%-15s %-10s %-10s %-10s\n", "Identifier", "Datatype", "Scope", "Line No.");
    printf("%-15s %-10s %-10s %-10s\n", "----------", "--------", "-----", "-------");
    
    // Print each symbol table entry
    for (int i = 0; i < symbolTable.count; i++) {
        printf("%-15s %-10s %-10s %-10d\n",
               symbolTable.entries[i].name,
               symbolTable.entries[i].datatype,
               symbolTable.entries[i].scope,
               symbolTable.entries[i].line_number);
    }
    printf("\n");
}

/*
 * Function: processDeclaration
 * Purpose: Parse a declaration statement and extract identifiers
 * Example: "int x, y, z;" extracts x, y, z as int identifiers
 * Parameters: line - the declaration statement
 *            scope - current scope
 *            line_number - line number in source
 */
void processDeclaration(const char* line, const char* scope, int line_number) {
    char temp[256];
    strcpy(temp, line);  // Make a copy since strtok modifies the string
    
    char* token = strtok(temp, " \t\n;,");  // Tokenize by spaces, tabs, newlines, semicolons, commas
    char datatype[MAX_TYPE_LENGTH] = "";
    
    // First token should be the data type
    if (token && isDatatype(token)) {
        strcpy(datatype, token);
        
        // Process remaining tokens as potential identifiers
        token = strtok(NULL, " \t\n;,=");
        while (token) {
            // Skip assignment operators, numeric values, and string literals
            if (strcmp(token, "=") != 0 && !isdigit(token[0]) && token[0] != '"') {
                addIdentifier(token, datatype, scope, line_number);
            }
            token = strtok(NULL, " \t\n;,=");
        }
    }
}

/*
 * Function: processStatement
 * Purpose: Parse a general statement and check for identifier usage
 * Example: "x = y + z;" checks if x, y, z are declared
 * Parameters: line - the statement to process
 *            scope - current scope
 *            line_number - line number in source
 */
void processStatement(const char* line, const char* scope, int line_number) {
    char temp[256];
    strcpy(temp, line);  // Make a copy since strtok modifies the string
    
    // Tokenize by various delimiters including operators and punctuation
    char* token = strtok(temp, " \t\n;,()=+-*/");
    while (token) {
        // Check if token is a valid identifier (not keyword or datatype)
        if (isValidIdentifier(token) && !isKeyword(token) && !isDatatype(token)) {
            checkUndeclaredUsage(token, scope, line_number);
        }
        token = strtok(NULL, " \t\n;,()=+-*/");
    }
}

/*
 * Main function: Orchestrates the entire symbol table analysis process
 * 1. Opens and reads input.txt file
 * 2. Processes each line to identify declarations and usage
 * 3. Displays final symbol table
 * 4. Demonstrates scope functionality
 */
int main() {
    printf("Symbol Table Implementation\n");
    printf("===========================\n\n");
    
    // Attempt to open the input file
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open input.txt file\n");
        printf("Please create an input.txt file with C code statements\n");
        return 1;
    }
    
    char line[256];      // Buffer to store each line
    int line_number = 1; // Track current line number
    
    printf("Processing code from input.txt:\n");
    printf("-------------------------------\n");
    
    // Read and process file line by line
    while (fgets(line, sizeof(line), file)) {
        // Remove trailing newline character
        line[strcspn(line, "\n")] = 0;
        
        // Skip empty lines but still increment line counter
        if (strlen(line) == 0) {
            line_number++;
            continue;
        }
        
        printf("Line %d: %s\n", line_number, line);
        
        // Determine if line is a declaration by checking if it starts with a datatype
        int is_declaration = 0;
        for (int j = 0; j < datatype_count; j++) {
            if (strstr(line, datatypes[j]) == line) {  // Line starts with datatype
                is_declaration = 1;
                break;
            }
        }
        
        // Process accordingly
        if (is_declaration) {
            processDeclaration(line, "global", line_number);
        } else {
            processStatement(line, "global", line_number);
        }
        
        line_number++;
    }
    
    fclose(file);  // Close input file
    
    // Display the final symbol table
    displaySymbolTable();
    
    // Demonstrate scope functionality with examples
    printf("=== SCOPE DEMONSTRATION ===\n");
    addIdentifier("local_var", "int", "function1", line_number);        // Valid
    addIdentifier("local_var", "float", "function2", line_number + 1);  // Same name, different scope - OK
    addIdentifier("local_var", "double", "function1", line_number + 2); // Same name, same scope - Error
    
    // Display final symbol table including scope demonstration
    displaySymbolTable();
    
    return 0;
}
