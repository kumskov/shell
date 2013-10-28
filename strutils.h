#ifndef __strutils_h__ 
#define __strutils_h__

/* Reading user input */

char* getString() /* Read infinite string from the input, say something if we go over MEM_LENGTH. */
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

        }   
    input[a]='\0';
    
    /* printf("The input is %s\n",input); */ /* Just some debugging junk */
    
    return input;
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

void cleanUp(char** array, int num)
    {
    while (num!=0) 
        {
        array[num] = NULL;                             
        num--;                                                                     
        }
    }


#endif
