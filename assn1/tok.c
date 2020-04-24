/*

convert a program written in simple and 
restricted  C language into a stream of tokens.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXLEN 1024

/* Declare Global Variables & Tables */

int x=0,y=1,c=0,token_id=0;
char resv;
FILE *fp ;
int flag=0, numFlag=9,m;
char buffer[MAXLEN];
int location[MAXLEN];

/* SymTab DataStructure */
struct table{
    int token_id;
    int tokenType;
    char *lexeme;
    int xpos;
    int ypos;
};

struct table SYMTAB[MAXLEN]; 

/* Check New Lines Tabs White Spaces and 
Accordingly  Manage the Position Counter */

void checkForWhitespace(char ch){
	if (ch=='\n'){
		x=0;y++;
	}else if (ch==' '){
		x++;
	}else if (ch=='\t'){
		x=x+4;
	}
	else x++;

}


/* Check for Special Punctuation RELOP
ArithOP Relational Assignment Operator*/

int checkPunct(char ch){


	char keyword[21];
    	keyword[0]='+';//arith_op
    	keyword[1]='-';
    	keyword[2]='*';
    	keyword[3]='/';
    	keyword[4]='<';//relop
    	keyword[5]='>';
    	keyword[6]='=';//assn
    	keyword[7]='(';//brackets
    	keyword[8]=')';
    	keyword[9]='{';
    	keyword[10]='}';
    	keyword[11]='[';
    	keyword[12]=']';
    	keyword[13]=',';//punct
    	keyword[14]=';';
    	keyword[15]='?';
    	keyword[16]='!';
    	keyword[17]='"';
    	keyword[18]='\n';//delimeters
    	keyword[19]='\t';
    	keyword[20]=' ';


    for (int i = 0; i < 21; ++i){
    	if (ch==keyword[i]){
    		return i;
    	}
    }
    return -1;
}

void commentHandler(){
	char temp;
	temp=fgetc(fp);
	while(temp!='\n'){
		temp=fgetc(fp);
	}
	x=1;y++;

}

void freeBuffer(){
	for (int i = 0; i < MAXLEN; ++i){
		buffer[i]='\0';
		location[i]='\0';
		c=0;
	}
	flag=0;
	numFlag=0;

}

void pretty_print(int h){
	
	printf("-----------------------------------------------------------------\n");
	if (strcmp(buffer,"if")==0||strcmp(buffer,"else")==0||strcmp(buffer,"for")==0||strcmp(buffer,"int")==0||strcmp(buffer,"float")==0||strcmp(buffer,"void")==0||strcmp(buffer,"return")==0)
	printf("%4d  |\t Reserved Keyword     | %20s | %3d %3d  |\n",++token_id,buffer,location[0],y);
	else if(numFlag==1)
	printf("%4d  |\tNumerical Expres      | %20s | %3d %3d  |\n",++token_id,buffer,location[0],y);
	else printf("%4d  |\t   Identifiers        | %20s | %3d %3d  |\n",++token_id,buffer,location[0],y);
	SYMTAB[token_id].token_id = token_id;
    SYMTAB[token_id].tokenType = 99;
    SYMTAB[token_id].lexeme= buffer;
    SYMTAB[token_id].xpos=location[0];
    SYMTAB[token_id].ypos=y;
	freeBuffer();
	

} 

void doEntry(char ch){
	int j = checkPunct(ch);
	if (j==-1){
		buffer[c]=ch;
		location[c]=x; //column number
		
		flag=1;
		m=isdigit(buffer[c]); // if most recent buffer element is numeral then isdigit return non zero -- 2048 in this case
		if (m!=0) numFlag=1;
		else numFlag=0;
		c++;

	}else if (j==3)
	{
		resv=fgetc(fp);
		if (resv=='/')
		{
			commentHandler();
		}else {
		printf("----------------------------------------------------------------\n");
		if (j>=0 && j<4)   printf("%4d  |\t Arithemtic  Exp     | %20c | %3d %3d  |\n",++token_id,ch,x,y);
			checkForWhitespace(resv);
    		doEntry(resv);
		};
	}else if (j>-1 && j<18)
	{
		if(flag==1){pretty_print(j);}
		printf("----------------------------------------------------------------\n");
		if (j>=0 && j<4)   printf("%4d  |\t Arithemtic  Exp.     | %20c | %3d %3d  |\n",++token_id,ch,x,y);
		if (j>=4 && j<=5)  printf("%4d  |\t Relational  Exp.     | %20c | %3d %3d  |\n",++token_id,ch,x,y);
		if (j==6)          printf("%4d  |\t Assignment  Operand  | %20c | %3d %3d  |\n",++token_id,ch,x,y);
		if (j>=7 && j<=12) printf("%4d  |\t Brackets             | %20c | %3d %3d  |\n",++token_id,ch,x,y);
		if (j>=13 && j<=17)printf("%4d  |\t Punctuation          | %20c | %3d %3d  |\n",++token_id,ch,x,y);


	//
	}if(j>=18) {
		
		if(flag==1){pretty_print(j);
		return;}

	}
    /*
    SYMTAB[token_id].token_id = token_id;
    SYMTAB[token_id].tokenType = j;
    SYMTAB[token_id].lexeme[0]= ch;
    SYMTAB[token_id].xpos=x;
    SYMTAB[token_id].ypos=y;
    */
}

int main(int argc, char const *argv[]){
	
	char ch;
    fp= fopen(argv[1], "r");
    ch=fgetc(fp);
    printf("----------------------------------------------------------------+\n");
    printf("  TID |\t   Token Type  \t      |\t    Lexeme \t     | Position |\n");
    while(ch!= EOF){
    	checkForWhitespace(ch);
    	doEntry(ch);
    	ch=fgetc(fp);

    }
    printf("----------------------------------------------------------------+\n");
    
	return 0;
}
