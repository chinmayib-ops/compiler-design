/*
Write a getNextToken() function to generate tokens for the following Python script.

#!/usr/bin/python3
# This script calculates the sum of a list

def calculate_sum(numbers):
    total = 0
    for num in numbers:
        total += num
    return total

numbers = [10, 20, 30, 40]
result = calculate_sum(numbers)
print(f"Sum: {result}")
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#define MAX_FILE_SIZE 10000
#define MAX_SYMBOLS 500

typedef enum{
    KEYWORD,
    IDENTIFIER,
    NUMERIC_CONSTANT,
    SPECIAL_SYMBOL,
    OPERATOR,
    STRING_LITERAL
}tokenTypes;

typedef struct{
    char name[50];
    tokenTypes type; //whatever category my token falls under
    int position;
}SymbolTableEntry;

SymbolTableEntry symbolTable[MAX_SYMBOLS];
int symbolCount = 0;


int isKeyword(const char *str){
    return(strcmp(str,"def")==0 || strcmp(str,"for")==0 || strcmp(str, "in")==0 || strcmp(str,"return")==0);
}

void addToSymbolTable(const char *token, tokenTypes type, int position){
    for(int i=0; i<symbolCount;i++){
        if(strcmp(symbolTable[i].name,token)==0)return;
    }
    if(symbolCount < MAX_SYMBOLS){
        strncpy(symbolTable[symbolCount].name,token,sizeof(symbolTable[symbolCount].name)-1);
        symbolTable[symbolCount].name[sizeof(symbolTable[symbolCount].name)-1] ='\0';
        symbolTable[symbolCount].type= type;
        symbolTable[symbolCount].position= position;
        symbolCount++;
    }
}

void getNextToken(FILE *fp){
    char bufferFile[MAX_FILE_SIZE];
    size_t bytesRead = fread(bufferFile, 1, MAX_FILE_SIZE-1, fp);
    bufferFile[bytesRead];

char *token = strtok(bufferFile, " \t\n{};(),[]");

while(token){
    int position = token - bufferFile;
    if(strncmp(token, "!#",2) == 0){
        token = strtok(NULL, "\n");
    }else if(token[0] == '#'){
        token = strtok(NULL, "\n");
    }else if(isKeyword(token)){
        printf("<KEYWORD : %s>\n", token);
        addToSymbolTable(token, KEYWORD, position);
    }else if(isalpha(token[0]) || token[0]=='_'){
        printf("<IDENTIFIER : %s>\n", token);
        addToSymbolTable(token,IDENTIFIER,position);
    }else if(strchr(":()[],{}", token[0])){
        printf("<SPECIAL_SYMBOL: %s>\n", token);  
        addToSymbolTable(token, SPECIAL_SYMBOL,position ); 
    }else if(strcmp(token, "=") == 0 || strcmp(token,"+=")==0){
        printf("<OPERATOR : %s>\n", token);
        addToSymbolTable(token,OPERATOR,position);
    }else if(isdigit(token[0])){
        printf("<NUMERIC_CONSTANT : %s>\n", token);
        addToSymbolTable(token,NUMERIC_CONSTANT,position);
    }else if(token[0] =='f' && token[1] == '"'){
        printf("<STRING_LITERAL : %s>\n", token);
        addToSymbolTable(token,STRING_LITERAL,position);
    }
    token = strtok(NULL, " \t\n():,[]{}");

}
}

void printSymbolTable(){
    printf("\n===== SYMBOL TABLE =====\n");
    printf("Index\tToken\t\tType\t\tPosition\n");
    printf("-------------------------------------------------\n");
    for(int i=0;i<symbolCount;i++){
        const char *typeName;
        switch(symbolTable[i].type){
            case KEYWORD: typeName = "KEYWORD"; break;
            case IDENTIFIER: typeName = "IDENTIFIER"; break;
            case NUMERIC_CONSTANT: typeName = "NUMERIC_CONSTANT"; break;
            case SPECIAL_SYMBOL: typeName = "SPECIAL_SYMBOL"; break;
            case OPERATOR: typeName = "OPERATOR"; break;
            case STRING_LITERAL: typeName = "STRING_LITERAL"; break;
            default: typeName = "UNKNOWN"; break;
        }
        printf("%d\t%s\t\t%s\t\t%d\n", i,symbolTable[i].name, typeName,symbolTable[i].position);

    }
}

int main(){
    FILE *fp = fopen("script.py" , "r");
    if(!fp){
        printf("error opening file. \n");
        return 1;
    }

    getNextToken(fp);
    fclose(fp);
    printSymbolTable();
    return 0;
}
