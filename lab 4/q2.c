#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LEN 50
#define MAX_SYMBOLS 100

const char *keywords[] = {"int","float","double","char", "if","else","while","for","struct","return"};
int keyword_count = 10;

const char special_symbols[] = {';', '{', '}', '(', ')', ',', '.', '[', ']', '*','\0'};


struct Symbol{
    char name[MAX_TOKEN_LEN];
    char type[MAX_TOKEN_LEN];
} symbolTable[MAX_SYMBOLS];
int symbolCount = 0;
//input the string to check if its a keyword or not
int isKeyword(char *str){
for(int i=0; i<keyword_count; i++){
    if(strcmp(str,keywords[i]) ==0 ){
        return 1;
    }
}
return 0;
}


int isSpecialSymbol(char c){
    for(int i=0; i<strlen(special_symbols); i++){
           if( c == special_symbols[i])
           return 1;
    }
    return 0;
}

void addToSymbolTable(char *name, char *type){
    strcpy(symbolTable[symbolCount].name, name);
    strcpy(symbolTable[symbolCount].type, type);
    symbolCount++;
}

int isIdentifier(char *str){
    //an identifier should start with either a letter or an underscore
    if(!isalpha(str[0])&& str [0]!= '_')return 0;
    for(int i=1; str[i] !='\0';i++){
        if(!isalnum(str[i]) && str[i] !='_')return 0;
    }
    return 1;
}

int isNumericConstant(char *str){
    for(int i=0; str[i] != '\0' ; i++){
        if(!isdigit(str[i]) && str[i] != '.')return 0;

    }
    return 1;
}
//fp is the input file and ur processing struct definitions in C
void processStruct(FILE *fp){
    char token[MAX_TOKEN_LEN]; //store the struct name
    int i =0; //counter for building the name
    char c = fgetc(fp); //getting the next character from the file

    while(isalnum(c) || c == '_'){
      token[i++] = c; // add it to token
      c = getc(fp);
    }
    token[i] = '\0';
    if(isIdentifier(token)){
        printf("<STRUCT_NAME, %s\n",token); //print it as a struct name token
        addToSymbolTable(token, "struct"); //add to symbol table

}
//skip spaces to reach the first meaningful character
while(isspace(c))
c = fgetc(fp);

if(c == '{'){
    printf("<SPECIAL_SYMBOL, {> \n");
    while(c!='}'){
        i=0;
        c=getc(fp);
        while (isalnum(c) || c == '_')
        {
            /* code */
            token[i++] = c;
            c = fgetc(fp);
        }
        token[i] ='\0';
        if(isKeyword(token)){
            printf("<KEYWORD, %s>\n",token);
            char varName[MAX_TOKEN_LEN];
            //READ VARIABLE NAME
            i=0;
            while(isspace(c))c = fgetc(fp);
            while(isalnum(c) || c == '_'){
                varName[i++] = c;
                c = fgetc(fp);
            }
            varName[i] = '\0';
            printf("<IDENTIFIER , %s>\n>", varName);
            addToSymbolTable(varName,token); 
             }
    
    printf("<SPECIAL_SYMBOL, }> \n");
}
}
}

void tokenize(FILE *fp) {
    char c;
    char buffer[MAX_TOKEN_LEN];
    int i = 0;

    while ((c = fgetc(fp)) != EOF) {
        // Skip spaces and new lines
        if (isspace(c)) {
            continue;
        }

        // Identify keywords, identifiers, and struct
        if (isalpha(c) || c == '_') {
            i = 0;
            while (isalnum(c) || c == '_') {
                buffer[i++] = c;
                c = fgetc(fp);
            }
            buffer[i] = '\0';
            ungetc(c, fp);

            if (isKeyword(buffer)) {
                printf("<KEYWORD, %s>\n", buffer);
                if (strcmp(buffer, "struct") == 0) {
                    processStruct(fp);
                }
            } else {
                printf("<IDENTIFIER, %s>\n", buffer);
            }
        }

        // Identify numeric constants
        else if (isdigit(c)) {
            i = 0;
            while (isdigit(c) || c == '.') {
                buffer[i++] = c;
                c = fgetc(fp);
            }
            buffer[i] = '\0';
            ungetc(c, fp);
            printf("<NUMERIC_CONSTANT, %s>\n", buffer);
        }

        // Identify special symbols
        else if (isSpecialSymbol(c)) {
            printf("<SPECIAL_SYMBOL, %c>\n", c);
        }

        // Identify operators
        else if (c == '=' || c == '<' || c == '>' || c == '!' || c == '+' || c == '-') {
            buffer[0] = c;
            c = fgetc(fp);
            if (c == '=') {
                buffer[1] = c;
                buffer[2] = '\0';
            } else {
                buffer[1] = '\0';
                ungetc(c, fp);
            }
            printf("<OPERATOR, %s>\n", buffer);
        }
    }
}
int main() {
    FILE *fp = fopen("input.c", "r");

    if (!fp) {
        printf("Error opening file.\n");
        return 1;
    }

    tokenize(fp);
    fclose(fp);

    // Display symbol table
    printf("\nSymbol Table:\n");
    for (int i = 0; i < symbolCount; i++) {
        printf("Name: %s, Type: %s\n", symbolTable[i].name, symbolTable[i].type);
    }

    return 0;
}
