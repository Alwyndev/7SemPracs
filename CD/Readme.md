# Compiler Design Practicals

<!-- Added Table of Contents -->
## 📑 Table of Contents
1. Overview
2. Project Structure
3. File Renaming Guide
4. Practical 1
5. Practical 2
6. Build & Run
7. Sample Usage
8. Learning Objectives
9. Key Concepts
10. Assignment Details
11. Recent Changes
12. Prerequisites
13. Advanced Usage
14. Future Enhancements
15. Contributing
16. License

This repository contains practical implementations for Compiler Design course covering lexical analysis, tokenization, and symbol table management using both C and Python.

## 📁 Project Structure

```
CD/
├── C/                                  # C implementations
│   ├── string_validator.c              # (rename from practical1_1.c) String validation
│   ├── keyword_counter.c               # (rename from practical1_2.c) Keyword counter
│   ├── delimiter_detector.c            # (rename from Practical1_3.c) Delimiter/punctuation identifier
│   ├── literal_extractor.c             # (rename from Practical1_4.c) String/character literal recognizer
│   ├── c_lexer.l                       # (rename from ex.l) Lex specification file
│   ├── lex.yy.c                        # Generated lexer from Flex
│   ├── symbol_table_manager.c          # (rename from Practical2.c) Symbol table implementation
│   ├── test_input.txt                  # (rename from input.txt) Test input for symbol table
│   └── *.exe                           # Compiled executables
├── python/                             # Python implementations
│   ├── character_validator.py          # (rename from practical1_1.py) Character validation
│   └── advanced_tokenizer.py           # (rename from practical1_2.py) Advanced tokenizer
├── assignment_descriptions.txt         # (rename from Practicals.txt) Assignment descriptions
└── README.md                           # This file
```

## 🗂️ File Renaming Guide

**Suggested Renames** (based on functionality):

| Current Name | Suggested Name | Reason |
|--------------|----------------|--------|
| `practical1_1.c` | `string_validator.c` | Validates string content |
| `practical1_2.c` | `keyword_counter.c` | Counts C keywords |
| `Practical1_3.c` | `delimiter_detector.c` | Detects delimiters/punctuation |
| `Practical1_4.c` | `literal_extractor.c` | Extracts string/char literals |
| `Practical2.c` | `symbol_table_manager.c` | Manages symbol table |
| `ex.l` | `c_lexer.l` | Lex rules for C tokens |
| `practical1_1.py` | `character_validator.py` | Character validation |
| `practical1_2.py` | `advanced_tokenizer.py` | Advanced tokenization |
| `input.txt` | `test_input.txt` | Test input data |
| `Practicals.txt` | `assignment_descriptions.txt` | Assignment descriptions |

## 🎯 Practical 1: Lexical Analysis Fundamentals

### Part A: String Validation

#### C Implementation ([`string_validator.c`](C/string_validator.c))
- **Purpose**: Validates if input string contains only alphanumeric characters, whitespaces, and allowed operators
- **Functions**:
  - `isAlpha()`, `isDigit()`, `isAlNum()` - Character type checking
  - `isSpace()`, `isOperator()` - Special character validation
  - `isValidString()` - Main validation logic
- **Usage**: Compile and run to check string validity

#### Python Implementation ([`character_validator.py`](python/character_validator.py))
- **Purpose**: Similar validation using custom character checking functions
- **Features**: Manual implementation of character type checking without using built-in methods
- **Usage**: `python character_validator.py`

### Part B: Keyword Counter

#### [`keyword_counter.c`](C/keyword_counter.c)
- **Purpose**: Counts and identifies C keywords in source code
- **Features**:
  - Comprehensive C keyword list (34 keywords)
  - Word-by-word parsing with delimiter handling
  - Case-sensitive keyword matching
- **Algorithm**: Tokenizes input and matches against predefined keyword array
- **Usage**: Enter C source code to get keyword count

### Part C: Delimiter and Punctuation Identifier

#### [`delimiter_detector.c`](C/delimiter_detector.c)
- **Purpose**: Identifies and reports positions of delimiters and punctuation marks
- **Supported Delimiters**: `;`, `,`, `(`, `)`, `{`, `}`, `[`, `]`, `:`, `.`
- **Supported Punctuation**: `!`, `?`, `'`, `"`, `-`, `#`, `@`, `/`
- **Features**:
  - Multi-line input support
  - Position tracking
  - Real-time character analysis
- **Usage**: Enter code/text, terminate with `$` on new line

### Part D: String and Character Literal Recognition

#### [`literal_extractor.c`](C/literal_extractor.c)
- **Purpose**: Extracts and displays string literals (`"..."`) and character literals (`'...'`)
- **Features**:
  - Handles both single and double quotes
  - Extracts content between quotes
  - Simple quote-based parsing
- **Limitations**: Doesn't handle escaped quotes

### Part E: Lex Program for Token Recognition

#### [`c_lexer.l`](C/c_lexer.l) - Flex Specification
- **Purpose**: Defines lexical rules for tokenizing C-like code
- **Token Types**:
  - **Keywords**: `int`, `float`, `return`, `if`, `else`, `while`, `for`, `void`, `char`, `double`
  - **Identifiers**: Variable/function names
  - **Numbers**: Integers and floating-point
  - **Others**: Unrecognized characters
- **Generated Output**: [`lex.yy.c`](C/lex.yy.c) (auto-generated by Flex)

#### Advanced Python Tokenizer ([`advanced_tokenizer.py`](python/advanced_tokenizer.py))
- **Purpose**: Comprehensive tokenizer with statistics
- **Token Types**:
  - **KEYWORD**: Reserved words
  - **IDENTIFIER**: Variable/function names
  - **OPERATOR**: Arithmetic, logical, comparison operators
  - **DELIMITER**: Punctuation marks
  - **NUMBER**: Integer and floating-point numbers
  - **STRING**: String literals
  - **UNKNOWN**: Unrecognized characters

**Features**:
- Multi-character operator support (`==`, `!=`, `<=`, `>=`, `&&`, `||`)
- String literal handling with quote matching
- Comprehensive token statistics
- Detailed token classification

## 🗃️ Practical 2: Symbol Table Implementation

### [`symbol_table_manager.c`](C/symbol_table_manager.c) - Complete Symbol Table System

**Purpose**: Implements a full-featured symbol table for lexical analysis with error detection.

#### Core Features:

1. **Symbol Table Management**
   - Stores identifier name, data type, scope, and line number
   - Supports up to 100 identifiers
   - Handles multiple scopes (global, function-specific)

2. **Declaration Processing**
   - Extracts identifiers from declaration statements
   - Supports multiple declarations: `int x, y, z;`
   - Validates identifier format and uniqueness

3. **Usage Validation**
   - Checks if identifiers are declared before use
   - Scope-aware lookup (current scope → global scope)
   - Reports undeclared identifier usage

4. **Error Detection**
   - **Multiple declarations**: Same identifier in same scope
   - **Keyword misuse**: Using reserved words as identifiers
   - **Undeclared usage**: Using variables before declaration
   - **Invalid identifiers**: Malformed variable names

5. **Supported Data Types**
   - `int`, `float`, `double`, `char`, `void`
   - `long`, `short`, `unsigned`, `signed`

#### Test Input ([`test_input.txt`](C/test_input.txt))
Contains sample C code with various scenarios:
- Valid declarations
- Multiple declarations (error case)
- Keyword misuse (error case)
- Undeclared variable usage (error case)

#### Key Functions:
- `addIdentifier()` - Adds new symbols to table
- `findIdentifier()` - Searches for existing symbols
- `checkUndeclaredUsage()` - Validates identifier usage
- `processDeclaration()` - Parses declaration statements
- `displaySymbolTable()` - Shows formatted symbol table

## 🔧 How to Build and Run

### C Programs
```bash
# Navigate to C directory
cd C/

# Compile individual programs
gcc string_validator.c -o string_validator.exe
gcc keyword_counter.c -o keyword_counter.exe
gcc delimiter_detector.c -o delimiter_detector.exe
gcc literal_extractor.c -o literal_extractor.exe
gcc symbol_table_manager.c -o symbol_table_manager.exe

# For Lex program (requires Flex)
flex c_lexer.l
gcc lex.yy.c -o lexer.exe

# Run programs
./string_validator.exe
./keyword_counter.exe
./delimiter_detector.exe
./literal_extractor.exe
./symbol_table_manager.exe
./lexer.exe
```

### Python Programs
```bash
# Navigate to python directory
cd python/

# Run programs
python character_validator.py
python advanced_tokenizer.py
```

## 📝 Sample Usage

### Tokenizer Example (Python)
```python
# Input
input_string = 'int x = 10; if (x > 5) print("Hello");'

# Output
('KEYWORD', 'int')
('IDENTIFIER', 'x')
('OPERATOR', '=')
('NUMBER', '10')
('DELIMITER', ';')
# ... and so on
```

### Symbol Table Example
```c
// Input (input.txt)
int x, y, z;
float average;
x = 10;
int x;  // Error: Multiple declaration

// Output
Added identifier: x (int) in scope global at line 1
Added identifier: y (int) in scope global at line 1  
Added identifier: z (int) in scope global at line 1
Added identifier: average (float) in scope global at line 2
Error: Multiple declaration of identifier 'x' in scope 'global' at line 4
```

## 🎓 Learning Objectives

1. **Lexical Analysis**: Understanding tokenization and pattern recognition
2. **Symbol Table Management**: Identifier storage and scope handling
3. **Error Detection**: Compile-time error identification
4. **Regular Expressions**: Pattern matching for tokens
5. **Data Structures**: Efficient symbol storage and retrieval
6. **Language Design**: Understanding compiler front-end components

## 🔍 Key Concepts Demonstrated

- **Finite State Automata**: Token recognition patterns
- **Hash Tables**: Efficient symbol lookup
- **Scope Resolution**: Variable visibility rules
- **Error Handling**: Graceful error reporting
- **Code Parsing**: Statement analysis and token extraction
- **Language Grammar**: Syntax rule implementation

## 📋 Assignment Details

Detailed assignment descriptions are available in [`assignment_descriptions.txt`](assignment_descriptions.txt), covering:
- **Practical 1**: Complete lexical analysis implementation
- **Practical 2**: Symbol table construction with error detection

This implementation serves as a foundation for understanding compiler design principles and lexical analysis techniques.

## 🔄 Recent Changes (2025-08)
- Added descriptive file naming suggestions
- Updated all file references to use meaningful names
- Added File Renaming Guide table
- Clarified prerequisites and environment setup
- Added input redirection examples
- Added rebuild / clean instructions for Flex-generated files
- Added Future Enhancements & Contributing sections
- Noted filename casing (Readme.md vs README.md)

## ✅ Prerequisites
- GCC (e.g., MinGW / GNU GCC 9+)
- Flex (for ex.l) and (optionally) Bison/Yacc for future grammar work
- Python 3.8+ (for tokenizer scripts)
- A terminal supporting standard I/O redirection

Optional (recommended):
- make (You can create a simple Makefile later)
- Git

## 🧪 Advanced / Recommended Usage

### Provide multiline input from a file
Instead of typing manually:
```
./symbol_table_manager.exe < test_input.txt
./keyword_counter.exe < some_source.c
```

### Regenerate lexer cleanly
```
cd C
flex c_lexer.l
gcc lex.yy.c -o lexer.exe
```

### Clean generated artifacts (manual)
```
del C\lex.yy.c
del C\*.exe
```
(Use rm on UNIX-like systems.)

### Run Python tokenizer on inline code
```
echo "int a=5; float b=2.5;" | python advanced_tokenizer.py
```

## ⚠️ Known Limitations
- String/char literal handling (literal_extractor.c) does not process escaped quotes
- Symbol table has fixed capacity (100) and linear lookup
- Lex spec (c_lexer.l) covers a limited subset of C tokens

## 🚀 Future Enhancements (Suggested)
- Add escaped sequence handling in string/char literal recognizer
- Extend lex rules: comments, multi-line strings, operators (++, --, +=, etc.)
- Introduce a hash-based symbol table
- Add scope push/pop (block-level) using a stack
- Integrate a simple parser (expression / declaration grammar)
- Add unit tests (e.g., using Python's unittest or a shell test harness)
- Provide a Makefile for cross-platform builds
- Add CI workflow (GitHub Actions) for automatic build/test

## 🤝 Contributing
1. Fork repository
2. Create a feature branch
3. Keep changes modular (one practical / fix per commit)
4. Submit PR with a short rationale
5. Ensure examples still compile

## 📄 License
Educational use. Add a LICENSE file (e.g., MIT) if redistribution is intended.