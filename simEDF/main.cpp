#include <iostream>
#include "simEDF.h"
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc < 3){
        std::cout<<"not enough argument"<<std::endl;
        return 0;
    }
    simEDF s(atoi(argv[2]), argv[1]);
    if (argc > 3){
        s.setS(true);
    }
    s.simulate();

    return 0;
}
