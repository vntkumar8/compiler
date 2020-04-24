#include<stdio.h> 
#include<ctype.h>
#include<string.h>

int limit, x = 0;
char inputSET[10][10], storeArray[10];

void computeFirstSet(char ch);
void computeFollowSet(char ch);
void ModBufArray(char ch);

FILE *inputFile,*outfile;

int readFromFile(){
	int count=0,i=0,j=0;
	char option, ch;
	inputFile=fopen("input","r");
	printf("-------------------------------------------\n\t\tInput Grammar\n-------------------------------------------\n");
	for(ch=getc(inputFile);ch!=EOF;ch=getc(inputFile))                  //Storing the characters of grammar in inputSET
    {
        printf("%c",ch);
        inputSET[i][j]=ch;
        if(ch=='\n')
        {
            count+=1;
            inputSET[i][j+1]=0;
            i+=1;
            j=0;
            continue;
        }
        j+=1;
    }
    return count;
}

void printFirst(){
	outfile=fopen("output","a");
	int count=0,i=0,j=0;
	char option, ch;
	printf("-------------------------------------------\n\t   First Computation\n-------------------------------------------\n");
 	for(int i=0;i<limit;i++){
  		x = 0;
  		ch=inputSET[i][0];
  		computeFirstSet(ch);
  		printf("First[%c]:   { ", ch);
  		fprintf(outfile,"First[%c]:   { ",ch);
  		for(count = 0; count < x; count++)
   		{
			printf("%c ", storeArray[count]);
			fprintf(outfile,"%c ", storeArray[count]);
			if (count!=x-1)
			{
				printf(" , ");
				fprintf(outfile," , ");
			}
		}
		printf("}\n");
		fprintf(outfile,"}\n");
         	
	}	
	fclose(outfile);
}

void printFollow(){
	outfile=fopen("output","a");
	int count=0,i=0,j=0;
	char option, ch;
	printf("-------------------------------------------\n\t   Follow Computation\n-------------------------------------------\n");
 	for(int i=0;i<limit;i++){
  		x = 0;
  		ch=inputSET[i][0];
  		computeFollowSet(ch);
  		printf("Follow[%c]:   { ", ch);
  		fprintf(outfile,"Follow[%c]:   { ",ch);
  		for(count = 0; count < x; count++)
   		{
			printf("%c ", storeArray[count]);
			fprintf(outfile,"%c ", storeArray[count]);
			if (count!=x-1)
			{
				printf(" , ");
				fprintf(outfile," , ");
			}
		}
		printf("}\n");
		fprintf(outfile,"}\n");
         	
	}
	fclose(outfile);	
}
void ModBufArray(char ch)
{
	int i;
    	for(i = 0; i <= x; i++)
	{
     		if(storeArray[i] == ch)
            	{
			return;
		}
	}
   	storeArray[x++] = ch;
}

int main()
{
	int count=0,i=0,j=0;
	char option, ch;
	outfile=fopen("output","w");
	fclose(outfile);
	limit=readFromFile();
	printFirst();
	printFollow();
	return 0;
}

void computeFollowSet(char ch)
{
	int i, j;
	int length = strlen(inputSET[i]);
	if(inputSET[0][0] == ch)
	{
		ModBufArray('$');
	}
 	for(i = 0; i < limit; i++)
 	{
  		for(j = 2; j < length; j++)
  		{
   			if(inputSET[i][j] == ch)
   			{
    				if(inputSET[i][j + 1] != '\0')
				{
					computeFirstSet(inputSET[i][j + 1]);
				}
    				if(inputSET[i][j + 1] == '\0' && ch != inputSET[i][0])
				{     				
					computeFollowSet(inputSET[i][0]);
				}
   			}
  		}
 	}
}

void computeFirstSet(char ch)
{
      	int i=0, k=0;
	if(!(isupper(ch)))
        {
		ModBufArray(ch);
	}
	for(k = 0; k < limit; k++)
	{
        	if(inputSET[k][0] == ch)
        	{
			if(inputSET[k][2] == '$') 
			{
				computeFollowSet(inputSET[i][0]);
			}
                 	else if(islower(inputSET[k][2]))
                     	{
                     		ModBufArray(inputSET[k][2]);
			}
                 	else 
			{
				computeFirstSet(inputSET[k][2]);
			}
                 }
	}
}

