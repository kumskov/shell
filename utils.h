#ifndef __utils_h__
#define __utils_h__

/* Old junk to work with lists. No longer needed since I don't use lists anymore. */

void pushToList(arg** target, char* text) /* Will push "text" string to "trg"-list */
    {
    /* int j=0, n=1; */
    arg* safeCopy=*target;
    arg* newNode;
    newNode=(arg*) malloc(sizeof(arg));

    while (safeCopy->next!=NULL)
        {
        safeCopy=safeCopy->next;
        }

    newNode->elem=text;

    /*
    while (text[j]!='\0')
            {
            newNode->elem[j]=text[j];
            j++;
            n++;
            printf("done\n");
            newNode->elem=(char*) realloc(newNode->elem, n*sizeof(char));
            }
    */

    debug("Arguement in push %s (temporal output)\n", newNode->elem);
    /* newNode->elem[j]='\0';  */

    safeCopy->next=newNode;
    newNode->prev=safeCopy;
    newNode->next=NULL;
    }

void initList(arg** target) /* Create the first node of arguement list. The first node will always be NULL */
    {
    (*target)=(arg*) malloc(sizeof(arg));
    /* (*target)->elem=(char*) malloc(sizeof(char)); */
    (*target)->elem='\0';
    (*target)->next=NULL;
    (*target)->prev=NULL;
    }   

void cleanList(arg** list)
    {
    arg* tail=*list;
    arg* head=*list;
    while (head->next!=NULL)
        {
        head=head->next;
        free(tail);
        tail=head;
        }
    free(head);
    free(tail);
    }

/* Now this is more interesting... */

void init()     /* Make shell ignore signals */
    {
    TERMINAL=STDIN_FILENO;
    processCounter=0;
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGINT, SIG_IGN);
    }

int checkProcessLimit()     /* Check if we have too many processes running */
    {
    if (processCounter>=MAX_PROCESS_AMOUNT) 
        {
        log_err("Maximum amount of parallel processes has been launched. Could not create another one.");
        return 0;
        } else
            {
            debug("There are %d processes at the moment. Creating a new one...", processCounter);
            return 1;
            }
    }

#endif
