#ifndef TEST_LD_H
#define TEST_LD_H

#include <boost/dll/shared_library.hpp>

template<typename type>
class LoadLD{
    public:
        typedef type *(*funcp1)();
        typedef type *(func1)();
        LoadLD(std::string path);
        ~LoadLD();
        funcp1 getSO();
    private:
        boost::dll::shared_library *m_lib;
};

template<typename type>
LoadLD<type>::LoadLD(std::string path){
    m_lib = new boost::dll::shared_library(path);
}

template<typename type>
LoadLD<type>::~LoadLD(){
    if(m_lib->is_loaded())
    {
        m_lib->unload();
    }
}

template<typename type>
typename LoadLD<type>::funcp1 LoadLD<type>::getSO(){
    if (m_lib->has("createService")) {
        return m_lib->get<func1>("createService");
    }
    return NULL;
}

#endif