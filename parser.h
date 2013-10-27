#ifndef __parser_h__
#define __parser_h__

int backSlashCheck(char letter, int *counter)   /* Such a sweet construction I decided it deserved its very own function */
    {
    if (letter=='\\')     /* Simple and nice construction to check if there was a backslash before the charachter we are reading now */
            {
            counter++;
            return 1;
            } else return 0;
    }

int getExecName(char* input, char* name, char* args) /* Will get the name of the program to execute */
    {
    int j=0, i=0; /* j is global, i is local */
    
    while((input[j]!=' ') && (input[j]!='\0'))
        {
        name[j]=input[j];
        j++;
        }


    name[j]='\0';
    j++;
    while(input[j]!='\0')
        {
        args[i]=input[j];
        j++;
        i++;
        }
        
    args[i]='\0';
    if (args[0]=='\0') return 0;
        else return 1;
    }   

void getHashComment(const char* input, char* output) /* Get comments indicated by # and ignore them */
    {
    /* int fhash=0; */  /* Indicate if we found hash comment */
    int i=0,bs=0;
    
    while (input[i]!='\0')
        {
        bs=backSlashCheck(input[i],&i);
        if ((input[i]!='#') && (!bs)) output[i]=input[i];
            else break;
        
        /*
        else 
            {
            fhash=1;    //If we'll need to know if there was a comment, we can return fhash
            break;
            }
        */
        
        i++;
        }
        
    output[i]='\0';
    }

void getArguements(char* input, char** arr, int *length)   /* Get arguements from string and fill the array with them. */
    {
    int i=0, j, n=1, bs=0, fq=0, cnt=0;

    while (input[i]!='\0')
        {
        arr[cnt]=(char*) malloc(MAX_STRING_LENGTH*sizeof(char));
        j=0;                                    
        if (errno)
            {
            log_err("Could not allocate memory for argument array, exiting...");
            exit(-1);
            }  
        
        while ((input[i]!=' ') && (input[i]!='\0')) 
            {
            fq=0;
            bs=backSlashCheck(input[i], &i);
            
            if ((input[i]=='"') && (!bs))       /* This huge and ugly construction will think of everything in the quotation as of 1 plain text */
                                            /* So if you enter aaa aa"ab ab bb"bb ab, it will decide that there are 3 arguements - (aaa), (aaab ab bbbb) and (ab) */
                {
                i++; /* The starting quotation itself will not be written as text */
                fq=1;
                while ((input[i]!='"') || (bs)) /* Not sure if this will work, but it should. This will make a \" construction into simple " in plain text */
                    {

                    bs=backSlashCheck(input[i], &i); /* Words can't express how proud I am with this little beauty */
                        
                    if (input[i]=='\0')
                        {
                        log_err("One of the arguements could not be finished since the quotation wasn't closed");
                        debug("What was written: %s", arr[cnt]);
                        exit(-1);
                        break;
                        }
                    if (j==MAX_STRING_LENGTH) 
                        {
                        log_err("1 of the arguments is exceeding allowed argument amount");
                        log_err("Terminatiing...");
                        exit(-1);
                        } else arr[cnt][j]=input[i];
                    i++;
                    j++;
                    n++; 
                    bs=0;
                    debug("I am working on quotes now");
                    }
                    
                /* i++; //And the finishing quotation won't be written either */
                }
            
            if (j==MAX_STRING_LENGTH) 
                    {
                    log_err("1 of the arguments is exceeding allowed argument amount");
                    log_err("Terminatiing...");
                    exit(-1);
                    } else arr[cnt][j]=input[i];
            i++;
            j++;
            n++;
                
            }

        if ((arr[cnt][j-1]=='"') && (!fq))
            {
            arr[cnt][j-1]='\0';
            } else arr[cnt][j]='\0';
                
        i++;
        debug("Arguement %d is (%s)", cnt, arr[cnt]);
        debug("Written in arg array: %s", arr[cnt]);
        cnt++;
        /* pushToList(outputList, singleArguement); */
          /* And then I clean it for a new arguement */
        }

    debug("Sucessfully exited the loop, written a total of is %d", cnt);
    *length=cnt;
    arr[cnt]=NULL;
    debug("Finished filling arrguements array");

    }

void printArguements(arg** inputList) /* This is a placeholder since we don't do anything with arguements yet */
    {   
    arg* safeCopy=*inputList;
    safeCopy=safeCopy->next; /* Just to ignore the first NULL */
    
    /* if(safeCopy->elem[0]=='\0')
        printf("No arguments");
    else
    { */
    
        while (1)
            {
            printf("Arguement: %s\n", safeCopy->elem);
            if (safeCopy->next!=NULL)
                {
                safeCopy=safeCopy->next;
                } else break;
            }
            
    }

#endif
