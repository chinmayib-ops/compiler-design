
/* AFTER LEFT FACTORING IDENTIFIER_LIST
Program -> main(){ declarations statement-list } 
declarations -> data-type identifier-list; declarations | E
data-type -> int | char
identifier-list -> id identifier-list1
identifier-list1 -> E | , identifier-list | [number]identifier-list2
identifier-list2 -> , identifier-list | E
statement_list -> statement statement_list | E
statement -> assign-stat; | decision_stat
assign_stat -> id = expn
expn -> simple-expn eprime 
eprime -> relop simple-expn | E 
simple-expn -> term seprime 
seprime -> addop term seprime | E
term -> factor tprime
tprime -> mulop factor tprime | E
factor -> id | num
decision-stat -> if (expn) {statement_list} dprime 
dprime -> else {statement_list} | E
relop -> ==|!=|<=|>=|>|< 
addop -> +|- 
mulop -> *|/|%
*/

#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


FILE * fp;
int lookahead = 0; //lookahead = 0 means previous token was matched with input. lookahead = 1 means epsilon prodn was used and hence old token is yet to be matched
Token tk;
void NextToken()
{
    if(!lookahead)
    {
        tk = getNextToken(fp);
        printf("token is %s\n",tk.name);
    }
    lookahead=0; 
}

void invalid(char* str)
{
    printf("\n-----------------ERROR!----------------\n");
    printf("Expected Token %s at line %d and col %d\n",str,tk.row,tk.col);
    exit(0);
}
void valid()
{
    printf("\n----------------SUCCESS!---------------\n");
    exit(0);
}

void statement_list();
void expn();
void id_list();
void statement();
void declarations();

void mulop()
{
    if(strcmp(tk.name,"*")==0 || strcmp(tk.name,"/")==0 || strcmp(tk.name,"%%")==0)
    {
        return;
    }

    else
        invalid( "* or / or %%");
}

void addop()
{
    if(strcmp(tk.name,"+")==0 || strcmp(tk.name,"-")==0)
    {
        return;
    }

    else
        invalid( "+ or -");
}

void relop()
{
    if(strcmp(tk.name,"==")==0 || strcmp(tk.name,"!=")==0 || strcmp(tk.name,"<=")==0 || strcmp(tk.name,">=")==0 || strcmp(tk.name,">")==0 || strcmp(tk.name,"<")==0)
    {
        return;
    }

    else
        invalid("== or != or <= or >= or > or <");
}

void dprime()
{
    if(strcmp(tk.name,"}")==0||strcmp(tk.name,"id")==0||strcmp(tk.name,"if")==0)
    {
        lookahead=1;
        return;
    }
        
    else if(strcmp(tk.name,"else")==0)
    {
        NextToken();
        if(strcmp(tk.name,"{")==0)
        {
            NextToken();
            statement_list();
            NextToken();
            if(strcmp(tk.name,"}")==0)
                return;
            else
                invalid( "}");
        }
        else
            invalid( "{");
    }

    else
        invalid("} or id or if or else");
}

void decision_stat()
{
    if(strcmp(tk.name,"if")==0)
    {
        NextToken();
        if(strcmp(tk.name,"(")==0)
        {
            NextToken();
            expn();
            NextToken();
            if(strcmp(tk.name,")")==0)
            {
                NextToken();
                if(strcmp(tk.name,"{")==0)
                {
                    NextToken();
                    statement_list();
                    NextToken();
                    if(strcmp(tk.name,"}")==0)
                    {
                        NextToken();
                        dprime();
                        return;
                    }
                    else
                        invalid( "}");
                }
                else
                    invalid( "{");
            }
            else
                invalid( ")");
        }
        else
            invalid( "(");
    }
    else
        invalid( "if");
}

void factor()
{
    if(strcmp(tk.name,"id")==0 || strcmp(tk.name,"num")==0)
        return;

    else
        invalid( "id or num");
}

void tprime()
{
    if(strcmp(tk.name,"==")==0 || strcmp(tk.name,"!=")==0 || strcmp(tk.name,"<=")==0 || strcmp(tk.name,">=")==0 || strcmp(tk.name,">")==0 || strcmp(tk.name,"<")==0 || strcmp(tk.name,"+")==0 || strcmp(tk.name,"-")==0 || strcmp(tk.name,")")==0 || strcmp(tk.name,";")==0)
    {
        lookahead=1;
        return;
    }

    mulop();
    NextToken();
    factor();
    NextToken();
    tprime();
    return;

}

void term()
{
    factor();
    NextToken();
    tprime();
}

void seprime()
{
    if(strcmp(tk.name,"==")==0 || strcmp(tk.name,"!=")==0 || strcmp(tk.name,"<=")==0 || strcmp(tk.name,">=")==0 || strcmp(tk.name,">")==0 || strcmp(tk.name,"<")==0 || strcmp(tk.name,";")==0 || strcmp(tk.name,")")==0)
    {
        lookahead=1;
        return;
    }
    addop();
    NextToken();
    term();
    NextToken();
    seprime();
}

void simple_expn()
{
    term();
    NextToken();
    seprime();
}

void eprime()
{
    if(strcmp(tk.name,";")==0 || strcmp(tk.name,")")==0)
    {
        lookahead=1;
        return;
    }
    relop();
    NextToken();
    simple_expn();
}

void expn()
{
    simple_expn();
    NextToken();
    eprime();
}

void assign_stat()
{
    if(strcmp(tk.name,"id")==0)
    {
        NextToken();
        if(strcmp(tk.name,"=")==0)
        {
            NextToken();
            expn();
        }
        else
            invalid("=");
    }
    else
        invalid( "id");
}
void statement()
{
    if(strcmp(tk.name,"id")==0)
    {
        assign_stat();
        NextToken();
        if(strcmp(tk.name,";")==0)
        {
            return;
        }
            
        else
            invalid(";");
    }
    else
        decision_stat();
}
void statement_list()
{
    if(strcmp(tk.name,"}")==0)
    {
        lookahead=1;
        return;
    }

    statement();
    NextToken();
    statement_list();
}

void id_list2()
{
    if(strcmp(tk.name,";")==0)
    {
        lookahead=1;
        return;
    }
       

    else if(strcmp(tk.name,",")==0)
    {
        NextToken();
        id_list();
        return;
    }

    else
        invalid( "; or ,");
}

void id_list1()
{
    if(strcmp(tk.name,";")==0)
    {
        lookahead=1;
        return;
    }

    else if(strcmp(tk.name,",")==0)
    {
        NextToken();
        id_list();
        return;
    }

    else if(strcmp(tk.name,"[")==0)
    {
        NextToken();
        if(strcmp(tk.name,"num")==0)
        {
            NextToken();
            if(strcmp(tk.name,"]")==0)
            {
                NextToken();
                id_list2();
                return;
            }
            else
                invalid( "]");
        }
        else
            invalid( "num");        
    }

    else
        invalid( "; or , or [");
}

void id_list()
{
    if(strcmp(tk.name,"id")==0)
    {
        NextToken();
        id_list1();
    }
    else
        invalid( "id");
}

void data_type()
{
    if(strcmp(tk.name,"int")==0 || strcmp(tk.name,"char")==0)
        return;

    else
        invalid("int or char");
}

void declarations()
{
    if(strcmp(tk.name,"}")==0 || strcmp(tk.name,"id")==0 || strcmp(tk.name,"if")==0)
        {
            lookahead=1;
            return;
        }

    data_type();
    NextToken();
    id_list();
    NextToken();
    if(strcmp(tk.name,";")==0)
    {
        NextToken();
        declarations();
        return;
    }
    else
        invalid(";");
}

void Program()
{
    if(strcmp(tk.name,"main")==0)
    {
        NextToken();
        if(strcmp(tk.name,"(")==0)
        {
            NextToken();
            if(strcmp(tk.name,")")==0)
            {
                NextToken();
                if(strcmp(tk.name,"{")==0)
                {
                    NextToken();
                    declarations();
                    NextToken();
                    statement_list();
                    NextToken();
                    if(strcmp(tk.name,"}")==0)
                    {
                        NextToken();
                        if(strcmp(tk.name,"EOF")==0)
                            valid();
                        else
                            invalid("EOF");
                    }
                    else
                        invalid( "}");
                }
                else
                    invalid( "{");
                    
            }
            else
                invalid( ")");
        }
        else
            invalid("(");
    }
    else
        invalid( "main");
}

int main() {
    fp = fopen("read.c", "r");
    if (!fp) {
        printf("Error opening file.\n");
        return 1;
    }
    printf("[DEBUG] Token: %s (row: %d, col: %d)\n", tk.name, tk.row, tk.col);

    NextToken();
    Program();
    fclose(fp);
    return 0;
}