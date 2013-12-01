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
        if (input[j]=='\\') 
        {
            j++;
        }
        else
        {
            name[j]=input[j];
            j++;
        }
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
    char* temp;

    while (input[i]!='\0')
        {
        temp=(char*) malloc(MAX_ARG_LENGTH*sizeof(char));
        j=0;                                    
        if (temp==NULL)
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
                        debug("What was written: %s", temp);
                        exit(-1);
                        break;
                        }
                    if (j==MAX_STRING_LENGTH) 
                        {
                        log_err("1 of the arguments is exceeding allowed argument amount");
                        log_err("Terminatiing...");
                        exit(-1);
                        } else temp[j]=input[i];
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
                    } else temp[j]=input[i];
            i++;
            j++;
            n++;
                
            }

        if ((temp[j-1]=='"') && (!fq))
            {
            temp[j-1]='\0';
            } else temp[j]='\0';
                
        i++;
        debug("Arguement %d is (%s)", cnt, temp);
        arr[cnt]=temp;
        debug("Written in arg array: %s", arr[cnt]);
        free(temp);
        cnt++;
        /* pushToList(outputList, singleArguement); */
          /* And then I clean it for a new arguement */
        }

    debug("Sucessfully exited the loop, written a total of is %d", cnt);
    *length=cnt;
    arr[cnt]=NULL;
    debug("Finished filling arrguements array");

    }

void splitString()
    {
        int fslash=0, fquote=0, fend=0;
        int charcnt=0, arrcharcnt=0;
        char word[STRING_INIT_LENGTH];

        while (!fend)
        {
            switch(source[charcnt])
            {
                case '\\':
                {
                    if (fslash)
                    {
                        word[arrcharcnt]=source[charcnt];
                        arrcharcnt++;
                        charcnt++;
                        fslash=0;
                    }
                    else
                    {
                        fslash=1;
                        charcnt++;
                    }
                    break;
                }

                case '\"':
                {
                    if (fslash)
                    {
                        word[arrcharcnt]=source[charcnt];
                        arrcharcnt++;
                        charcnt++;
                        fslash=0;

                    } 
                    else 
                    {
                        if (fquote)
                        {
                            fquote=0;
                            charcnt++;
                        }
                        else
                        {
                            fquote=1;
                            charcnt++;
                        }
                    }
                    break;
                }

                case '#':
                {
                    if ((fslash) || (fquote))
                    {
                        word[arrcharcnt]=source[charcnt];
                        arrcharcnt++;
                        charcnt++;
                        if (fslash) fslash=0;
                    }
                    else
                    {
                    fend=1;
                    }
                    break;
                }

                case EOF:
                {
                    endflag=1;
                    fend=1;
                    break;
                }

                case '\0':
                {
                    debug("I am writing last arguement which is number %d", arguementCnt);
                    word[arrcharcnt]='\0';
                    debug("Writing >%s<", word);
                    arrcharcnt=0;
                    arguements[arguementCnt]=word;
                    printf("Written >%s< in array of args\n", arguements[arguementCnt]);
                    arguementCnt++;
                    fend=1;
                    break;
                }

                case ' ':
                {
                    if (fquote)
                    {
                        word[arrcharcnt]=source[charcnt];
                        arrcharcnt++;
                        charcnt++;
                    }
                    else
                    {
                        charcnt++;
                        debug("I have arguement number %d", arguementCnt);
                        word[arrcharcnt]='\0';
                        debug("Writing >%s<", word);
                        arrcharcnt=0;
                        arguements[arguementCnt]=word;
                        printf("Written >%s< in array of args\n", arguements[arguementCnt]);
                        arguementCnt++;
                    }
                    break;
                }

                default:
                {
                    word[arrcharcnt]=source[charcnt];
                    arrcharcnt++;
                    charcnt++;
                    break;
                }
            }
        }
        if (fquote)
        {
            errflag=1;
            log_err("The quote was not closed - will not execute the command");
        }
    arguements[arguementCnt]=NULL;
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

void cleanCommandArray()
    {
        while (arguementCnt!=0) 
        {
            arguements[arguementCnt]=NULL;
            arguementCnt--;                                  
        }                                                    
    }



#endif
