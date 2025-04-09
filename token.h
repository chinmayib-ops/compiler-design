#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

typedef enum{
    IDENTIFIER,
    KEYWORD,
    RELOP,
    SYMBOL,
    INVALID,
    OPERATOR,
    NUMBER,
    END
}TokenClass;

 typedef struct{
  char name[20];
  TokenClass classs;
 int row;
 int col;
 }Token;

 const char *keywords[]={"int","char","main","if","else"};
 const char symbols[] ="()[]{};=,";
 const char *relops[] ={"==","!=",">=","<=",">","<"};

 Token getNextToken(FILE *fp){

    static int row=1, col=1;
    Token tk ={.row=row, .col=col};
    char c, buffer[20];
    int i=0;
    while((c = fgetc(fp))!=EOF){
    if(c =='\n'){
        row++;
        col=1;
        continue;
    }if(isspace(c)){
        col++;
        continue;
    }
    if(c=='/'){
        char next= fgetc(fp);
        if(next == '/'){
            while((c=fgetc(fp))!='\n' && c!=EOF);
            ungetc(c,fp);
            col=1;
            continue;
        }else if(next == '*'){
            col+=2;
            while(1){
                c= fgetc(fp);
                col++;
                if(c=='\n'){
                    row++;
                    col=1;
                }
                if(c=='*' &&(c= fgetc(fp))=='/'){
                    col+=2;
                    break;
                }
                if(c==EOF){
                    strcpy(tk.name,"EOF");
                    tk.classs=END;
                    return tk;
                }
            }
            continue;
        }
        else {ungetc(next,fp);}
    }
    tk.row=row;
    tk.col=col;

    if(strchr(symbols,c)){
        tk.name[0]=c;
        tk.name[1]='\0';
        tk.classs=SYMBOL;
        col++;
        return tk;
    }
    if(c=='=' ||c=='!' || c=='>'||c=='<'){
        buffer[i++]=c;
        c= fgetc(fp);
        if(c=='='){
            buffer[i++]=c;
            buffer[i]='\0';
            col+=2;
            strcpy(tk.name, buffer);
            tk.classs= RELOP;
            return tk;
        }
        ungetc(c,fp);
        if(i==1){
            buffer[i]='\0';
            strcpy(tk.name, buffer);
            tk.classs=(buffer[0]=='=')?OPERATOR:RELOP;
            col++;
            return tk;
        }
    }
    if(isdigit(c)){
        while(isdigit(c)){
            buffer[i++]=c;
            c=fgetc(fp);
            col++;
        }
        ungetc(c,fp);
        buffer[i]='\0';
        strcpy(tk.name, "num");
        tk.classs=NUMBER;
        col--; //reading that extra character so you have to backtrack
        return tk;
    }
    if(isalpha(c)||c=='_'){
        while(isalnum(c) ||c=='_'){
            buffer[i++]=c;
            c=fgetc(fp);
            col++;
        }
        ungetc(c,fp);
        buffer[i]='\0';

        for(int j=0; j<5;j++){
            if(strcmp(buffer,keywords[j])==0){
                strcpy(tk.name, buffer);
                tk.classs=KEYWORD;
                return tk;
            }
        }
        strcpy(tk.name,"id");
        tk.classs=IDENTIFIER;
        return tk;
    }

    if(strchr("+-*/%%",c)){
        tk.name[0]=c;
        tk.name[1]='\0';
        tk.classs=OPERATOR;
        col++;
        return tk;
    }

    tk.name[0]=c;
    tk.name[1]='\0';
    tk.classs= INVALID;
    col++;
    return tk;


    }
    strcpy(tk.name, "EOF");
    tk.classs= END;
    return tk;
 }
