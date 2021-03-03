#include <iostream>

#include "../libs/MapReduce.h"

int main(/*int argc, char** argv*/){

    /*if (argc != 4)
    {
        std::cout << "Wrong arguments. Usage: map_reduce <src> <mnum> <rnum>" << std::endl;
        //return -1;
    };

    size_t countMaps = std::stoi(argv[2]);
    if (countMaps <= 0)
    {
        std::cout << "mnum should be > 0" << std::endl;
       // return -2;
    }

    size_t countReducers = std::stoi(argv[3]);
    if (countReducers <= 0)
    {
        std::cout << "rnum should be > 0" << std::endl;
        //return -3;
    }*/

    std::string s1 {"test1"};
    size_t ms = 5;
    size_t rs = 5;
    MapReduce mp1{s1, ms, rs};
    mp1.start();



    return 0;
}
