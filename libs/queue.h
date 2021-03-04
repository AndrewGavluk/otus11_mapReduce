#pragma once
#include <atomic>
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <vector>

struct Bulk;

template <typename T>
class tsVector
{
    public:
        tsVector() : m_EOF{false}{};
        
        template <typename T1>
        tsVector(T1 size ): m_vector(size){};

        tsVector(const  tsVector&);
        tsVector(tsVector&&) = default;
        void push(T);
        T get(size_t&);
        std::vector<T>& getVector() { return m_vector;}
        void reserve(size_t num) { m_vector.reserve(num);}
        T& operator [] (size_t);
        void setEOF();
    private:
        std::vector<T> m_vector;
        std::mutex m_mutex; //m_mutexBrackets; 
        std::condition_variable m_cv;
        bool m_EOF; 
};

template <typename T>
void tsVector<T>::setEOF()
{
    m_EOF = true;
    m_cv.notify_one(); // all ( one after one );
}

template <typename T>
void tsVector<T>::push(T line )
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_vector.push_back(line);
    m_cv.notify_one();
}

template <typename T>
T tsVector<T>::get(size_t& number )
{
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_vector[number];
}

template <typename T>
tsVector<T>::tsVector(const tsVector& other){
    (void)other;
}


template <typename T>
T& tsVector<T>::operator [] (size_t index){
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_vector[index];
}