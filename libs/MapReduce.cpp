#include"MapReduce.h"

MapReduce::MapReduce(std::string& filename, size_t& _mapNum, size_t& _redNum) : m_mapNum{_mapNum}, m_redNum{_redNum}
{
    m_borders.reserve(_mapNum);
    m_MapperResults.reserve(_mapNum);
    m_inputFile.open(filename);
    m_ReducerResults.reserve(_redNum);
}

bool MapReduce::setMapper(void (*_mapper)(std::ifstream&, pos_t&, pos_t&,  vString_t&))
{
    return setFunction( m_mapperThread , _mapper);
}

size_t someFunction1(){ return 0;}

void MapReduce::mapperThread(std::ifstream& fileIn, pos_t begin , pos_t end , vString_t& vStr)
{
    (*m_mapperThread) (fileIn, begin, end, vStr);
    std::sort(vStr.begin(), vStr.end());
}

bool MapReduce::setReducer(void (*_reducer)(vString_t&))
{
    return setFunction( m_reducerThread , _reducer);
}

void MapReduce::start()
{
    if (m_mapperThread && m_reducerThread){
        split();
        map();
        shuffle();
        reduce();
    }
    else
        std::cerr << "maper or reducer function not set";
}

void MapReduce::split()
{
    m_inputFile.seekg(0, std::ios_base::end);
    uint32_t endPos = m_inputFile.tellg();
    uint32_t part = endPos / m_mapNum;
    
    char temp{0};
    for (uint32_t pos = part; pos<endPos; pos += part)
    {
        m_inputFile.seekg(pos, std::ios_base::beg);
        while (m_inputFile.read(&temp,1) && temp!='\n' && !m_inputFile.fail());
        // {}

        pos = m_inputFile.tellg();
        m_borders.push_back(pos);
    }
    m_borders.push_back(endPos);
}

void MapReduce::map()
{
    std::vector<std::thread> tasks{m_borders.size()};
    
    // run mapping function
    size_t counter{0}; 
    std::iostream::pos_type begin{0};
    for (auto& end : m_borders ){
        tasks.emplace_back(&MapReduce::mapperThread, std::ref(*this), std::ref(m_inputFile), std::ref(begin), std::ref(end), std::ref(m_MapperResults[counter++]));
        begin = end;
    }

    for (auto& task : tasks  )
        if (task.joinable())
            task.join();
}

void MapReduce::shuffle()
{
    auto sorter = [&] (auto maperResult){
        std::copy(maperResult.begin(), maperResult.end(), std::back_inserter(m_sorterResults));};

    std::for_each(m_MapperResults.begin(), m_MapperResults.end(), sorter);
    std::sort(m_sorterResults.begin(), m_sorterResults.end());
}

void MapReduce::reduce(){

    std::vector<std::thread> tasks{m_redNum};
    
    for (size_t reducerIter = 0; reducerIter<m_redNum; ++reducerIter)
        tasks.emplace_back(&MapReduce::reducerThread, std::ref(*this), std::ref(m_ReducerResults[reducerIter++].getVector()) );  
    
    for (auto& task : tasks  )
        if (task.joinable())
            task.join();

}