#include <array>
#include <algorithm>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <stdlib.h>
#include <thread>
#include <vector>

#include "queue.h"

class MapReduce
{
    typedef std::iostream::pos_type  pos_t;
    typedef std::vector<std::string> vString_t;
    typedef std::vector<vString_t> v2String_t;
    typedef std::vector<tsVector<std::string>> vTsvString;

    public:
        MapReduce() = delete;
        MapReduce(std::string&, size_t&, size_t&);
        
        bool setMapper(void (*_mapper)(std::ifstream&, pos_t&, pos_t&, vString_t&));
        bool setReducer(void (*_reducer)(vString_t&));
        void start();

    private:
        
        void map();
        void reduce();

        void (*m_mapperThread)(std::ifstream& fileIn, pos_t&, pos_t&, vString_t&); 
        void mapperThread(pos_t, pos_t, size_t);

        void (*m_reducerThread)(vString_t&);
        void reducerThread(vString_t& StrIn);
            
        size_t getHash(const std::string&);

        template <typename T1, typename T2>
        bool setFunction(T1*, T2*);

        static const std::string hasher;

        v2String_t                              m_MapperResults;
        vTsvString                              m_ReducerResults;
        std::ifstream                           m_inputFile;
        std::mutex                              m_mutex; 
        std::condition_variable                 m_cv;
        size_t                                  m_mapNum, m_redNum;
};



template <typename T1, typename T2>
        bool MapReduce::setFunction(T1* f1, T2* f2)
{
    (void) f1;
    try
    {
        if (f2){
            f1 = f2;
            return true;
        }
        else
            std::cerr << "Error: nullptr pointer" << '\n';

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return false;
}