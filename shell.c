#include "headers.h"
#include "zadm.h"
#include "strutils.h"
#include "utils.h"
#include "parser.h"
#include "executor.h"



int main()
    {
        init();
        sayHello();
        while ((TRUE) && (!endflag))
            {
                inviteUser();
                getString();
                startWorking();
            }    
        

        return 0;
    }
