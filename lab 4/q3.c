#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FILE_SIZE 100000
#define MAX_SYMBOLS 500  // Maximum entries in symbol table

typedef enum {
    KEYWORD,
    IDENTIFIER,
    NUMERIC_CONSTANT,
    SPECIAL_SYMBOL,
    OPERATOR,
    VAR_REF
} TokenType;

// Symbol Table Entry Structure
typedef struct {
    char name[50];  // Name of identifier
    TokenType type; // Type of token
    int position;   // Position in the file
} SymbolTableEntry;

SymbolTableEntry symbolTable[MAX_SYMBOLS];  // Symbol Table
int symbolCount = 0; // Counter for entries in the symbol table

// Function to check if a token is a keyword
int isKeyword(const char *str) {
    return (strcmp(str, "scalar") == 0 || strcmp(str, "foreach") == 0);
}

// Function to add an entry to the Symbol Table
void addToSymbolTable(char *token, TokenType type, int position) {
    // Check if token already exists
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, token) == 0) return; // Avoid duplicates
    }
    
    // Add new entry
    if (symbolCount < MAX_SYMBOLS) {
        strncpy(symbolTable[symbolCount].name, token, sizeof(symbolTable[symbolCount].name) - 1);
        symbolTable[symbolCount].name[sizeof(symbolTable[symbolCount].name) - 1] = '\0';
        symbolTable[symbolCount].type = type;
        symbolTable[symbolCount].position = position;
        symbolCount++;
    }
}

// Function to process tokens
void getNextToken(FILE *fp) {
    char fileBuffer[MAX_FILE_SIZE];
    size_t bytesRead = fread(fileBuffer, 1, MAX_FILE_SIZE - 1, fp);
    fileBuffer[bytesRead] = '\0';  // Ensure null-terminated string

    char *token = strtok(fileBuffer, " \t\n{};(),"); // Tokenizing based on delimiters

    while (token) {
        int position = token - fileBuffer; // Calculate token position in the file
        
        // Ignore shebang line (`#!`)
        if (strncmp(token, "#!", 2) == 0) {
            token = strtok(NULL, "\n"); 
        }
        // Ignore comments (`#`)
        else if (token[0] == '#') {
            token = strtok(NULL, "\n");
        }
        // `$n` followed by an identifier is treated as a variable reference
        else if (strncmp(token, "$n", 2) == 0 && isalpha(token[2])) {
            printf("<VAR_REF, %s>\n", token);
            addToSymbolTable(token, VAR_REF, position);
        }
        // Keywords: `scalar`, `foreach`
        else if (isKeyword(token)) {
            printf("<KEYWORD, %s>\n", token);
            addToSymbolTable(token, KEYWORD, position);
        }
        // Special token `@_`
        else if (strcmp(token, "@_") == 0) {
            printf("<SPECIAL_SYMBOL, @_>\n");
        }
        // Operator `+=`
        else if (strcmp(token, "+=") == 0) {
            printf("<OPERATOR, +=>\n");
        }
        // Identifiers (e.g., `$sum`, `$item`)
        else if (token[0] == '$' && isalpha(token[1])) {
            printf("<IDENTIFIER, %s>\n", token);
            addToSymbolTable(token, IDENTIFIER, position);
        }
        // Numeric Constants
        else if (isdigit(token[0])) {
            printf("<NUMERIC_CONSTANT, %s>\n", token);
            addToSymbolTable(token, NUMERIC_CONSTANT, position);
        }
        // Special symbols (`+`, `=`, `{`, `}`, `;`, `(`, `)`)
        else if (strchr("=+{}();", token[0])) {
            printf("<SPECIAL_SYMBOL, %s>\n", token);
        }

        token = strtok(NULL, " \t\n{};(),");  // Get next token
    }
}

// Function to print the Symbol Table
void printSymbolTable() {
    printf("\n===== SYMBOL TABLE =====\n");
    printf("Index\tToken\t\tType\t\tPosition\n");
    printf("-------------------------------------------------\n");
    for (int i = 0; i < symbolCount; i++) {
        printf("%d\t%s\t\t%d\t\t%d\n", i, symbolTable[i].name, symbolTable[i].type, symbolTable[i].position);
    }
}

int main() {
    FILE *fp = fopen("script.pl", "r");
    if (!fp) {
        printf("Error opening file.\n");
        return 1;
    }

    getNextToken(fp);
    fclose(fp);

    printSymbolTable(); // Print Symbol Table after processing

    return 0;
}
