#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
   char str[1024];
   FILE *fp,*fout;
   char lch;
   int flag=9; // recursion detection flag
   int cnt=0;
   char alpha[10][100];
   char beta[10][100];
   // SUPPORT FOR READING FROM FILE
   int lines_allocated = 128;
   int max_line_len = 100;

    /* Allocate lines of text */
    char **words = (char **)malloc(sizeof(char*)*lines_allocated);
    if (words==NULL)
        {
        fprintf(stderr,"Out of memory (1).\n");
        exit(1);
        }

    fp = fopen("input.txt", "r");
    fout=fopen("output.txt","w");
    int i;
    for (i=0;1;i++){
        int j;
        words[i] = malloc(max_line_len);
        if (fgets(words[i],max_line_len-1,fp)==NULL)
            break;

        
    }
    fclose(fp);


//ACTUAL READING OF GRAMMAR AND REMOVAL OF  LEFT RECURSSION
    int j;
    for(j = 0; j < i; j++)
        printf("%s\n", words[j]);
     
     int top=j;

    for (int z = 0; z < top; ++z){
        int l=strlen(words[z]);
         for (int i = 3; i < l; ++i){
         lch=words[z][0];
         str[cnt]=words[z][i];
         cnt++;
         }
         str[cnt]='\0';


          char s[2];
          s[0]='|';
          int len,ay=0,ax=0;
          int bx=0,by=0;
         char *token;
   
   /* get the first token */
         token = strtok(str, s);
         
   /* walk through other tokens */
   while( token != NULL ){
     // printf( " %s\n", token );

      // ALPHA TABLE ENTRY
            if (token[0]==lch){
               len =strlen(token);
               for (int i = 0; i < len; ++i)
               {
                  alpha[ay][i]=token[i+1];
               }
               ay++;
               flag=1;

            }
        //BETA TABLE ENTRY    
            else if (token[0]!=lch)
            {
               len =strlen(token);
               for (int i = 0; i < len; ++i)
               {
                  beta[by][i]=token[i];
               }
               by++;
            }
            token = strtok(NULL, s);
   }
   
   if(flag!=1){printf("NO Left recursion in Grammar\n"); return -1;}
// alpha beta table printing
/*   for (int i = 0; i < ay; ++i)
   {
      printf("%s\n",alpha[i] );
   }

   for (int i = 0; i < by; ++i)
   {
      printf("%s\n",beta[i] );
   }
  
*/
            fprintf(fout,"%c->",lch );
            for (int i = 0; i < by; ++i)
            {
               fprintf(fout,"%s%c\'",beta[i],lch );
               if(i=(by-1))fprintf(fout,"|");
            }  
            fprintf(fout,"\n");
            fprintf(fout,"%c\'->",lch );
            for (int i = 0; i < ay; ++i)
            {
               fprintf(fout,"%s%c\'",alpha[i],lch );
               fprintf(fout,"|");
            }
               fprintf(fout,"^\n");


}

    
   return(0);
}
