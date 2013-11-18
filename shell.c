#include "headers.h"
#include "zadm.h"
#include "strutils.h"
#include "utils.h"
#include "parser.h"
#include "executor.h"



int main()
    {
    char* home=getenv("HOME");
    int arglength;
    init();

    arglength=0;
    while ((TRUE) && (!endflag))
        {
        home=getenv("HOME");
        arglength=0;
        printf("------------- starting new command -------------\n");
        debug("Source is >%s<, execName is >%s<, arguementstring is >%s<", source, execName, arguementString);
        printf("%s>", home);
        source=getString();
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
        testarray();
        /* if (errflag)
        {
            log_err("An error occured - command ignored");
        } */
        defflag=checkDefaultCommands();
        /* if (!defflag)
            ironsInTheFire(execName, arguements, FG); */

         /*Uncomment this to proceed to Shell Part 2 task */
        cleanUp(arguements);
        execName[0]='\0';
        arguementString[0]='\0';
        }    
    

    return 0;
    }
