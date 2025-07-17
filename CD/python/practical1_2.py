def tokenize(input_string):
# Defines the main tokenizing function that takes a string of code as input

    tokens = []
    # Initializes an empty list to store the identified tokens
    
    keywords = {'if', 'else', 'while', 'for', 'def', 'return', 'import', 'from', 'as', 'class', 'try', 'except', 'finally', 'int', 'float', 'str', 'bool', 'print'}
    # Defines a set of reserved keywords that have special meaning in the language
    
    operators = {'+', '-', '*', '/', '=', '==', '!=', '<', '>', '<=', '>=', '&&', '||', '!', '++', '--'}
    # Defines a set of operators for arithmetic, comparison, logical, and assignment operations
    
    delimiters = {';', ',', '(', ')', '{', '}', '[', ']', ':'}
    # Defines punctuation marks that separate or group code elements
    
    i = 0
    # Initializes index pointer to traverse the input string character by character
    
    while i < len(input_string):
    # Main loop that processes each character until end of string
    
        char = input_string[i]
        # Gets the current character being examined
        
        # Skip whitespace
        if char.isspace():
        # Checks if current character is whitespace (space, tab, newline, etc.)
            i += 1
            # Moves to next character without creating a token
            continue
            # Skips to next iteration of the loop
            
        # Handle string literals
        if char == '"':
        # Detects the start of a string literal
            string_literal = '"'
            # Begins building the string token with opening quote
            i += 1
            # Moves past the opening quote
            while i < len(input_string) and input_string[i] != '"':
            # Continues until closing quote is found or end of string
                string_literal += input_string[i]
                # Adds each character to the string literal
                i += 1
                # Moves to next character
            if i < len(input_string):
            # Checks if we found a closing quote (not end of string)
                string_literal += input_string[i]
                # Adds the closing quote to complete the string
                i += 1
                # Moves past the closing quote
            tokens.append(('STRING', string_literal))
            # Adds the complete string as a STRING token
            continue
            # Skips to next iteration
            
        # Handle single character delimiters
        if char in delimiters:
        # Checks if current character is a delimiter
            tokens.append(('DELIMITER', char))
            # Creates a DELIMITER token with the character
            i += 1
            # Moves to next character
            continue
            # Skips to next iteration
            
        # Handle operators (including multi-character ones)
        if char in '+-*/<>=!&|':
        # Checks if character could be start of an operator
            operator = char
            # Starts building the operator with current character
            if i + 1 < len(input_string):
            # Checks if there's a next character to examine
                two_char = char + input_string[i + 1]
                # Forms potential two-character operator
                if two_char in operators:
                # Checks if the two-character combination is a valid operator
                    operator = two_char
                    # Uses the two-character operator
                    i += 1
                    # Advances extra position for two-character operator
            tokens.append(('OPERATOR', operator))
            # Adds the operator token (either one or two characters)
            i += 1
            # Moves to next character
            continue
            # Skips to next iteration
            
        # Handle numbers
        if char.isdigit():
        # Checks if current character is a digit
            number = ''
            # Initializes empty string to build the number
            while i < len(input_string) and (input_string[i].isdigit() or input_string[i] == '.'):
            # Continues while we have digits or decimal points
                number += input_string[i]
                # Adds digit or decimal point to number
                i += 1
                # Moves to next character
            tokens.append(('NUMBER', number))
            # Adds the complete number as a NUMBER token
            continue
            # Skips to next iteration
            
        # Handle identifiers and keywords
        if char.isalpha() or char == '_':
        # Checks if character can start an identifier (letter or underscore)
            identifier = ''
            # Initializes empty string to build identifier
            while i < len(input_string) and (input_string[i].isalnum() or input_string[i] == '_'):
            # Continues while we have letters, digits, or underscores
                identifier += input_string[i]
                # Adds character to identifier
                i += 1
                # Moves to next character
            if identifier in keywords:
            # Checks if the identifier is actually a reserved keyword
                tokens.append(('KEYWORD', identifier))
                # Creates a KEYWORD token
            else:
                tokens.append(('IDENTIFIER', identifier))
                # Creates an IDENTIFIER token for variable/function names
            continue
            # Skips to next iteration
            
        # Handle unknown characters
        tokens.append(('UNKNOWN', char))
        # Creates an UNKNOWN token for unrecognized characters
        i += 1
        # Moves to next character
    
    return tokens
    # Returns the complete list of tokens

def print_statistics(tokens):
# Function to count and display token type statistics
    stats = {}
    # Initializes empty dictionary to store counts
    for token_type, _ in tokens:
    # Iterates through tokens, only using the token type (ignoring value with _)
        stats[token_type] = stats.get(token_type, 0) + 1
        # Increments count for this token type (starts at 0 if new)
    
    print("\n=== Token Statistics ===")
    # Prints header for statistics section
    for token_type, count in stats.items():
    # Iterates through each token type and its count
        print(f"{token_type}: {count}")
        # Displays token type and count

if __name__ == '__main__':
# Checks if this script is being run directly (not imported)
    input_string = input("Enter the code to check:")
    # Prompts user to enter code for tokenization
    tokens = tokenize(input_string)
    # Calls tokenize function on user input
    
    print("\n=== Tokens ===")
    # Prints header for token list
    for token in tokens:
    # Iterates through each token
        print(token)
        # Displays each token as (TYPE, VALUE) tuple
    
    print_statistics(tokens)
    # Calls function to display token statistics