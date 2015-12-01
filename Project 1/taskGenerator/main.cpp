#include <iostream>
#include "taskGenerator.h"
#include <string>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int utilisation = -1;
    int nbTasks = -1;
    int lowP = -1;
    int highP = -1;
    int lowO = -1;
    int highO = -1;
    bool implicit = false;
    bool synchronous = false;
    char* path = NULL;
    int c;
    while ((c = getopt (argc, argv, "u:n:o:p:P:f:F:is")) != -1)
    switch (c)
    {
        case 'u':
            utilisation = atoi(optarg);
            break;
        case 'n':
            nbTasks = atoi(optarg);
            break;
        case 'o':
            path = optarg;
            break;
        case 'p':
            lowP = atoi(optarg);
            break;
        case 'P':
            highP = atoi(optarg);
            break;
        case 'f':
            lowO = atoi(optarg);
            break;
        case 'F':
            highO = atoi(optarg);
            break;
        case 'i':
            implicit = true;
            break;
        case 's':
            synchronous = true;
            break;
        case '?':
            std::cout<<"error"<<std::endl;
            return 1;
        default:
            abort ();
    }
    if (nbTasks<1 || utilisation <0 || utilisation>100 || path == NULL){
        std::cout<<"bad or missing argument"<<std::endl;
        return 2;
    }

    taskGenerator tg(nbTasks, utilisation, path);

    if (lowO > 0 && highO >= lowO){
        tg.setHO(highO);
        tg.setLO(lowO);
    }
    else if (!(lowO == -1 && highO == -1)){
        std::cout<<"Invalid offset option, floor must be >= 0 and roof must be >= floor"<<std::endl;
        std::cout<<"If you want a synchronous tasks generation use argument -s"<<std::endl;
        return 2;
    }

    if (lowP >= 0 && highP >= lowP){
        tg.setHP(highP);
        tg.setLP(lowP);
    }
    else if (!(lowP == -1 && highP == -1)){
        std::cout<<"Invalid period option, floor must be >= 0 and roof must be >= floor"<<std::endl;
        return 2;
    }

    if (synchronous){
        tg.setHO(1);
        tg.setLO(0);
    }

    if (implicit){
        tg.setI(true);
    }

    tg.generateTasks();

    return 0;
}
