#include"MapReduce.h"

MapReduce::MapReduce(std::string& filename, size_t& _mapNum, size_t& _redNum) : m_mapNum{_mapNum}, m_redNum{_redNum}
{
    m_borders.reserve(_mapNum);
    m_maperResults.reserve(_mapNum);
    m_inputFile.open(filename);
}
/*
bool MapReduce::setMapper(void (*_mapper)(std::ifstream&, pos_t, pos_t,  vString_t&))
{
    return setFunction( m_mapperThread , _mapper);
}

bool MapReduce::setSpliter(void (*_reducer)(vString_t&, std::ofstream&))
{
    return setFunction( m_reducerThread , _reducer);
}*/

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

    std::iostream::pos_type begin{0};
    
    // run mapping function
    //size_t counter{0}; 

    std::for_each(m_borders.cbegin(), m_borders.cend(), [&] (std::iostream::pos_type end){
        //tasks.emplace_back(&MapReduce::sample, std::ref(*this), std::ref(m_inputFile), std::ref(begin), std::ref(end), std::ref(m_maperResults[counter++]));
        std::thread t1 {&MapReduce::sample, std::ref(*this), begin,  end};
        //tasks.emplace_back(&MapReduce::sample, std::ref(*this), std::ref(m_inputFile));
        begin = end;
    });

    // join and sort
    /*counter = 0;
    std::for_each(m_borders.cbegin(), m_borders.cend(), [&] (std::thread task){
        if (task.joinable()){
            task.join();
            std::sort(m_maperResults[counter].begin(), m_maperResults[counter].end());
        }
        ++counter;
    });*/

}
