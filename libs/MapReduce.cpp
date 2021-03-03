#include"MapReduce.h"

const std::string MapReduce::hasher = "ABCDEFGHIJKLMNOPQRSTVWXYZabcdefghijlklmnopqrstvwxyz";

MapReduce::MapReduce(std::string& filename, size_t& _mapNum, size_t& _redNum) : m_mapNum{_mapNum}, m_redNum{_redNum}
{
    m_MapperResults.reserve(_mapNum);
    m_ReducerResults.reserve(_redNum);
    
    m_inputFile.open(filename);
    m_inputFile.seekg(0, std::ios_base::end);
}

bool MapReduce::setMapper(void (*_mapper)(std::ifstream&, pos_t&, pos_t&,  vString_t&))
{return setFunction( m_mapperThread , _mapper);}

bool MapReduce::setReducer(void (*_reducer)(vString_t&))
{return setFunction( m_reducerThread , _reducer);}

// hash sorting by first carackter, good for string with different first char
size_t MapReduce::getHash(const std::string& str ){ 
    const char& c = str[0];
    size_t pos = MapReduce::hasher.find(c);
    
    if (pos != std::string::npos)
        return pos / (MapReduce::hasher.size() / m_redNum);
    return 0;
}

void MapReduce::mapperThread(pos_t begin , pos_t end , size_t counter)
{
    std::unique_lock<std::mutex> lock(m_mutex);
	m_cv.wait(lock);
    auto& thVector = m_MapperResults[counter]; 
    
    // entire function call
    (*m_mapperThread) (m_inputFile, begin, end, thVector);
    std::sort(thVector.begin(), thVector.end());

    // shufle
    for (auto& str : thVector)
        m_ReducerResults[getHash(str)].push(str);
}

void MapReduce::start()
{
    if (m_mapperThread && m_reducerThread){
        map();
        reduce();
    }
    else
        std::cerr << "maper or reducer function not set";
}

void MapReduce::map()
{
    std::vector<std::thread> tasks;

    // loop init params
    pos_t begin{0};
    size_t counter{0}; 
    char temp{0};

    // loop iterator and stop params
    pos_t endPos = m_inputFile.tellg();
    pos_t part = endPos / m_mapNum;

    for (pos_t pos = part; pos<endPos; pos += part)
    {
        m_inputFile.seekg(pos, std::ios_base::beg);
        while (m_inputFile.read(&temp,1) && temp!='\n' && !m_inputFile.fail()); 
        // {}
        
        pos = m_inputFile.tellg();
        tasks.emplace_back(&MapReduce::mapperThread, std::ref(*this), std::ref(begin), std::ref(pos), counter++);
        begin = pos;
    }

    m_cv.notify_all();

    for (auto& task : tasks  )
        if (task.joinable())
            task.join();
}


void MapReduce::reduce(){

    std::vector<std::thread> tasks{m_redNum};
    
    for (size_t reducerIter = 0; reducerIter<m_redNum; ++reducerIter)
        tasks.emplace_back(&MapReduce::reducerThread, std::ref(*this), std::ref(m_ReducerResults[reducerIter++].getVector()) );  
    
    for (auto& task : tasks  )
        if (task.joinable())
            task.join();

}