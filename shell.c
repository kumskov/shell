#include "headers.h"
#include "zadm.h"
#include "strutils.h"
#include "utils.h"
#include "parser.h"
#include "executor.h"



int main()
    {
    int f=0;
    int arglength;
    init();

    while(TRUE)
        {
        printf("------------- starting new command -------------\n");
        printf(">");
        source=getString();
        debug("Input is >%s<", source);
        argflag=getExecName(source, execName, arguementString);
        arglength=3;
        if (argflag)
            {
            debug("Getting arguements...");
            debug("I have a string of >>%s<<", arguementString);
            getArguements(arguementString, arguements, &arglenght);
            debug("Global counter of array elements is %d", arglenght);
            }   

        defflag=checkDefaultCommands();
        if (!defflag)
            ironsInTheFire(execName, arguements, FG);

         /*Uncomment this to proceed to Shell Part 2 task */
        cleanUp(arguements, arglength);
        source[0]='\0';
        }    
    

    return 0;
    }
