//
//Design a lexical Analyzer to generate tokens for functions and structures in ‘C’ .


#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FILE_SIZE 10000
#define MAX_SYMBOLS 500

typedef enum {
    KEYWORD,
    IDENTIFIER,
    FUNCTION,
    STRUCTURE,
    SPECIAL_SYMBOL,
    OPERATOR
} TokenType;

typedef struct {
    char name[50];
    TokenType type;
    int position;
} SymbolTableEntry;

SymbolTableEntry symbolTable[MAX_SYMBOLS];
int symbolCount = 0;

// List of C keywords
const char *keywords[] = {"struct", "int", "float", "void", "return"};
int keywordCount = sizeof(keywords) / sizeof(keywords[0]);

// Function to check if a token is a keyword
int isKeyword(const char *token) {
    for (int i = 0; i < keywordCount; i++) {
        if (strcmp(token, keywords[i]) == 0) return 1;
    }
    return 0;
}

// Function to check if a token is already in the symbol table
int isTokenInTable(const char *token) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, token) == 0) return 1;
    }
    return 0;
}

// Function to add a token to the symbol table
void addToSymbolTable(const char *token, TokenType type, int position) {
    if (!isTokenInTable(token)) {
        if (symbolCount < MAX_SYMBOLS) {
            strncpy(symbolTable[symbolCount].name, token, sizeof(symbolTable[symbolCount].name) - 1);
            symbolTable[symbolCount].name[sizeof(symbolTable[symbolCount].name) - 1] = '\0';
            symbolTable[symbolCount].type = type;
            symbolTable[symbolCount].position = position;
            symbolCount++;
        }
    }
}

// Function to process tokens
void getNextToken(FILE *fp) {
    char fileBuffer[MAX_FILE_SIZE];
    size_t bytesRead = fread(fileBuffer, 1, MAX_FILE_SIZE - 1, fp);
    fileBuffer[bytesRead] = '\0';

    char *token = strtok(fileBuffer, " \t\n{};(),");

    while (token) {
        int position = token - fileBuffer; 

        // Detect keywords (struct, int, float, void, return)
        if (isKeyword(token)) {
            printf("<KEYWORD, %s>\n", token);
            addToSymbolTable(token, KEYWORD, position);
        }
        // Detect function names (identifier followed by '(')
        else if (token[strlen(token) - 1] == '(') {
            token[strlen(token) - 1] = '\0'; // Remove '(' to get function name
            printf("<FUNCTION, %s>\n", token);
            addToSymbolTable(token, FUNCTION, position);
        }
        // Detect structures (struct followed by identifier)
        else if (strcmp(token, "struct") == 0) {
            token = strtok(NULL, " \t\n{};(),");  // Get the next token (structure name)
            if (token) {
                printf("<STRUCTURE, %s>\n", token);
                addToSymbolTable(token, STRUCTURE, position);
            }
        }
        // Detect identifiers (variables)
        else if (isalpha(token[0]) || token[0] == '_') {
            printf("<IDENTIFIER, %s>\n", token);
            addToSymbolTable(token, IDENTIFIER, position);
        }
        // Detect operators
        else if (strchr("=+-*/%<>!", token[0])) {
            printf("<OPERATOR, %s>\n", token);
            addToSymbolTable(token, OPERATOR, position);
        }
        // Detect special symbols ({, }, (, ), ;, ,)
        else if (strchr("{}();,", token[0])) {
            printf("<SPECIAL_SYMBOL, %s>\n", token);
        }

        token = strtok(NULL, " \t\n{};(),");
    }
}

// Function to print the symbol table
void printSymbolTable() {
    printf("\n===== SYMBOL TABLE =====\n");
    printf("Index\tToken\t\tType\t\tPosition\n");
    printf("-------------------------------------------------\n");
    for (int i = 0; i < symbolCount; i++) {
        printf("%d\t%s\t\t%d\t\t%d\n", i, symbolTable[i].name, symbolTable[i].type, symbolTable[i].position);
    }
}

int main() {
    FILE *fp = fopen("sample.c", "r");
    if (!fp) {
        printf("Error opening file.\n");
        return 1;
    }

    getNextToken(fp);
    fclose(fp);

    printSymbolTable(); // Print the symbol table

  return 0;
}