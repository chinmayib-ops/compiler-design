#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKEN_LEN 50

// Function to check if a character is an operator
int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%');
}

// Function to tokenize input arithmetic expressions
void getNextToken(const char *expr) {
    int i = 0, length = strlen(expr);
    char token[MAX_TOKEN_LEN];

    while (i < length) {
        // Ignore whitespace
        if (isspace(expr[i])) {
            i++;
            continue;
        }

        // Detect numbers (integer or decimal)
        if (isdigit(expr[i])) {
            int j = 0;
            while (isdigit(expr[i]) || expr[i] == '.') {
                token[j++] = expr[i++];
            }
            token[j] = '\0';
            printf("<NUMERIC_CONSTANT, %s>\n", token);
        }

        // Detect operators
        else if (isOperator(expr[i])) {
            printf("<OPERATOR, %c>\n", expr[i]);
            i++;
        }

        // Detect parentheses
        else if (expr[i] == '(' || expr[i] == ')') {
            printf("<PARENTHESIS, %c>\n", expr[i]);
            i++;
        }

        // Detect assignment
        else if (expr[i] == '=') {
            printf("<ASSIGNMENT, %c>\n", expr[i]);
            i++;
        }

        // Handle unknown characters
        else {
            printf("<UNKNOWN, %c>\n", expr[i]);
            i++;
        }
    }
}

int main() {
    char expression[] = "x = 10 + (20.5 * 3) / 2 - 5";
    printf("Tokenizing: %s\n", expression);
    getNextToken(expression);
    return 0;
}
