#include "headers.h"
#include "zadm.h"
#include "strutils.h"
#include "utils.h"
#include "parser.h"
#include "executor.h"
/* #include "shared.h" */



int main()
    {
        init();
        sayHello();
        while (TRUE)
            {
                cleanCommandArray();
                inviteUser();
                getString();
                /* printf("%s\n", arguements[0]); */
                startWorking();
            }    
        

        return 0;
    }
