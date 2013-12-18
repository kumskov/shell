#ifndef __strutils_h__ 
#define __strutils_h__

#include "headers.h"

/* Reading user input */


char* getString(int *Break1) 
    { 
    int c;
    int length = 1,n = 1,half_memory = 0; 
    char* str;

    str = (char*)calloc(1,sizeof(char));
    
    while ((c = getchar()) != '\n') 
        {
        if (length+1 > n) 
        {
            if (half_memory) n++;
            else n = 2 * n;
            str = realloc(str, n * sizeof(char));
            if ((str == NULL) && (half_memory == 0))
                {
                printf("\nWarning! You've taken half of memory. Finish entering the string. \n");
                half_memory = 1;
                }
            if ((str == NULL) && (half_memory == 1))
                {printf("\nOut of memory.\n");}
            }
        if (c == EOF) break;
        str[length-1] = (char) c;
        length++;
        }

    if (half_memory == 1) 
        {
        *Break1 = 2; 
        return NULL;
        }
    else if (c == EOF) 
        {
        *Break1 = 1;
        str[length-1] = '\0';
        printf("\n");
        return str;
        }
    else 
        {
        str[length-1] = '\0';
        return str;
        }
    }



char* getOldString(int eoflag) /* Read infinite string from the input, say something if we go over MEM_LENGTH. */
    { 
    int a=0, n=1;
    char* input;
    input=(char*) malloc(sizeof(char));
    /*
    tempinp=(char*) malloc(sizeof(char));
    
        if (tempinp==NULL)
            {
            free(input);
            } else input=tempinp;
    */
    debug("test");

    input[a]=getchar();
    
    while ((input[a]!='\n') && (input[a]!='\0') && (input[a]!=EOF))
        {   
        n++;
        if (n == MEM_LENGHT)
            {
            int memborder = MEM_LENGHT;
            log_err("The string you entered is too large. Stopped reading it at %d symbol", memborder);
                    /* I will probably need some error flag in here. */
            break;  /* Yeah, that's right. You exceed MEMORY LENGHT - everything BREAKs. */
            }
        /*  
        tempinp=(char*) realloc(input, n*sizeof(char));
        if (tempinp==NULL) 
            {
            free(input);
            break;
            } else input=tempinp;
        */
        a++;
        input[a]=getchar();
        if  (input[a]==EOF) 
            {
            debug("Caught EOF\n");
            eoflag=1;
            break;
            }
        else eoflag=0;
        }
    input[a]='\0';
    
    /* printf("The input is %s\n",input); */ /* Just some debugging junk */
    
    return input;
    }

char* strmerge(char* str1, char* str2)
{
    char* result=malloc((strlen(str1)+strlen(str2))*sizeof(char));
    strcpy(result, str1);
    strcat(result, str2);
    return result;
}

/* Alright, getString is awfull dumb useless function that is not optimized in any way. Why the hell would I ever want dynamic string? Screw it, I am doing it like perfectly sane people do */

/* Properly reading user input */

void getUserInput(char* output)
    {
    char userInput[MAX_STRING_LENGTH];
    char input;
    int n=0;
    input = getchar();
    while(TRUE)
        {
        if ((input=='\n') || (input=='\0') || (input==EOF))
            {
            break;
            } else
                {
                if (n==MAX_STRING_LENGTH)
                    {
                    log_err("Input string has reached it's limit");
                    break;
                    }
                userInput[n]=input;
                n++;
                }
        }
    userInput[n]='\0';
    output=userInput;
    }

/* Oh my god why the hell would I want to create a GOD FORSAKEN FUNCTION FOR SIMPLE a=b where both a and b are char*??? */

void putTextToString(char* string, const char* text)
    {
    int n=1;
    int i=0;
    string=(char*) malloc(sizeof(char));
    while (text[i]!='\0')
        {
        string[i]=text[i];
        i++;
        n++;
        string=(char*) realloc(string,n*sizeof(char));
        }
    string[i]='\0';

    }

/* Clearing the array of strings. */

void cleanUp(char** array)
    {
    int i;
    for (i=0; i<128; i++)

        {
        array[i] = NULL;                                                                                                 
        }
    }

void testarray()
    {
        int i;
        for (i=0;i<5;i++)
        {
            debug("test %s", arguements[i]);
        }
        debug("test1 %s", arguements[0]);
        debug("test1 %s", arguements[1]);
        debug("test1 %s", arguements[2]);
    }

void sayHello()
    {
        printf("\n-----------------------------------------------------------\n");
        printf("\tShell started, process ID is <%d>\n", SHELL_ID);
        printf("\tNot much is implemented yet, type \"help\" to see what's up\n");
        printf("-------------------------------------------------------------\n");
        printf("\n");
    }

void inviteUser()
    {
        if (dotflag)
        {
            printf("%s@shell:~%s$ ", getenv("LOGNAME"), getcwd(currentDir, 1024));

        }
        else
        {
            printf("%s@shell:$ ", getenv("LOGNAME"));

        }
    }

void append(char* subject, const char* insert, int pos) 
{
    char buf[100]={};

    strncpy(buf, subject, pos); 
    int len=strlen(buf);
    strcpy(buf+len, insert); 
    len+=strlen(insert);  
    strcpy(buf+len, subject+pos+1); 

    strcpy(subject, buf); 
}

#endif
