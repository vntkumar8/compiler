
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char grammar[50][50];
char first[50][50];
char follow[50][50];
FILE *inputFile,*outputFile;



int readGrammar(){
	int count=0,i=0,j=0;
	char option, ch;
	inputFile=fopen("input","r");
	for(ch=getc(inputFile);ch!=EOF;ch=getc(inputFile))                  //Storing the characters of grammar in grammar
    {
        
        grammar[i][j]=ch;
        if(ch=='\n')
        {
            count+=1;
            grammar[i][j+1]=0;
            i+=1;
            j=0;
            continue;
        }
        j+=1;
    }
    return count;
}


void readFirst(){
	int count=0,i=0,j=0;
	char option, ch;
	inputFile=fopen("first","r");
	for(ch=getc(inputFile);ch!=EOF;ch=getc(inputFile))                  //Storing the characters in first
    {
        
        first[i][j]=ch;
        if(ch=='\n')
        {
            count+=1;
            first[i][j+1]=0;
            i+=1;
            j=0;
            continue;
        }
        j+=1;
    }
    
}
void readFollow(){
    int count=0,i=0,j=0;
    char option, ch;
    inputFile=fopen("follow","r");
    for(ch=getc(inputFile);ch!=EOF;ch=getc(inputFile))                  //Storing the characters in follow
    {
        
        follow[i][j]=ch;
        if(ch=='\n')
        {
            count+=1;
            follow[i][j+1]=0;
            i+=1;
            j=0;
            continue;
        }
        j+=1;
    }
    
}
char nonTerm[40];
int no_nonTerm=0;
void buildNonTerminal(int count){
    int c=0;
    for (int i = 0; i < count; ++i)
    {
        nonTerm[c]=grammar[i][0];
        c++;
    }
    no_nonTerm=c;   
}

int checkNonTerm(int ch){
    for (int i = 0; i < no_nonTerm; ++i)
    {
        if (ch==nonTerm[i])
        {
            return i;
        }
        
    }
    return -1;
}
char term[40];
int mflag=-1;
void buildTerminal(int count){
    int c=0;
    
    for (int i = 0; i < count; ++i)
    {
        int lim=strlen(grammar[i]);
        for (int j = 0; j < lim; ++j)
        {
           
            if (grammar[i][j]=='|'||grammar[i][j]=='-'||grammar[i][j]=='>'||grammar[i][j]=='\n'||grammar[i][j]=='^')
            {
                continue;
            }
            char ch=grammar[i][j];
            int key=checkNonTerm(ch);

            //printf("%c returned %d\n",ch,key );
            if (key==-1)
            {
                term[c]=grammar[i][j];
                c++;
            }
            
        }
    }

}

char *substring(int i,int j,char *ch)
{
    int n,k=0;
    char *ch1;
    ch1=(char*)malloc((j-i+1)*1);
    n=j-i+1;

    while(k<n)
    {
        ch1[k]=ch[i];
        i++;k++;
    }

    return (char *)ch1;
}
void modPrint(int p,int mc){
    char* temp;
    for (int i = 0; i < strlen(grammar[p]); ++i)
    {
        if (grammar[p][i]!='|')
        {
            printf("%c",grammar[p][i] );
            //temp[i]=grammar[p][i];
            //table[mc].pro[i]=grammar[p][i];
        }else if (grammar[p][i]=='|')
        {
            return;
        }
    }
}
int main(int argc, char const *argv[])
{
    int noOfProductions=readGrammar();
    int majorCount=0;
    outputFile=fopen("output","w");
    //buildTerminal(noOfProductions);
    buildNonTerminal(noOfProductions);
    readFollow();
    readFirst();
    buildTerminal(noOfProductions);
    printf("Non Terminals : %s\n",nonTerm );
    printf("Terminals : %s\n",term );

    printf("------------------------------------------------------\n");
    printf("NonTerm  \tTerminal\tProductions\n");
    for (int i = 0; i < noOfProductions; ++i)
    {   printf("------------------------------------------------------\n");
        //grammar[i][0]
        int l=strlen(first[i]) ;
        //printf("%d\n", l);
        for (int j = 1; j < l; ++j)
        {   if (first[i][j]=='\n') continue;
            /**IF EPSILON OCCURS IN RIGHT PRO**/

            if (first[i][j]=='^')
            {
                //printf("follow should occur\n");
                for (int m = 1; m < strlen(follow[i]); ++m)
                {   if (follow[i][m]=='\n')
                {
                    continue;
                }
                    printf("%c\t\t%c\t\t%c->^\n\n",nonTerm[i],follow[i][m],nonTerm[i] );
                    //write to a file
                    fprintf(outputFile,"%c\t\t%c\t\t%c->^\n\n",nonTerm[i],follow[i][m],nonTerm[i]);
                   
                }
                
            }



            /**EPSILON Handled**/       
            /////TODO: handle printing ^ in parseTable
            else  {printf("%c\t\t%c\t\t",nonTerm[i], first[i][j]);
            modPrint(i,majorCount);
            printf("\n\n");
// gr[i]   

            }
        }
        //printf("%s\n",first[i][] );
       
    }
        

    return 0;
}
