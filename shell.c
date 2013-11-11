#include "headers.h"
#include "zadm.h"
#include "strutils.h"
#include "utils.h"
#include "parser.h"
#include "executor.h"



int main()
    {
    int arglength;
    int i;
    init();

    while ((TRUE) && (!endflag))
        {
        arglength=0;
        printf("------------- starting new command -------------\n");
        debug("Source is >%s<, execName is >%s<, arguementstring is >%s<", source, execName, arguementString);
        printf(">");
        getString(source);
        debug("Input is >%s<", source);
        argflag=getExecName(source, execName, arguementString);
        printf("Program name is >%s<\n", execName);
        free(source);
        
        arglength=3;
        if (argflag)
            {
            debug("Getting arguements...");
            debug("I have a string of >>%s<<", arguementString);
            splitArrayFromString(arguementString, arguements, &arglenght);
            debug("Global counter of array elements is %d", arglenght);
            }   
        if (errflag)
        {
            log_err("An error occured - command ignored");
        } 
        else defflag=checkDefaultCommands();
        /* if (!defflag)
            ironsInTheFire(execName, arguements, FG); */

         /*Uncomment this to proceed to Shell Part 2 task */
        cleanUp(arguements);
        execName[0]='\0';
        arguementString[0]='\0';
        }    
    

    return 0;
    }
