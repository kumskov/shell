#ifndef __strutils_h__ 
#define __strutils_h__

/* Reading user input */

char* getString() /* Read infinite string from the input, say something if we go over MEM_LENGTH. */
    { 
    int a=0, n=1;
    char* input;
    input=(char*) malloc(STRING_INIT_LENGTH*sizeof(char));

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
        a++;
        input[a]=getchar();
        }
    if (input[a]==EOF) 
    {
        printf("\nTerminating...\n");
        free(input);
        exit(0);
    }
    input[a]='\0';
    
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


#endif
