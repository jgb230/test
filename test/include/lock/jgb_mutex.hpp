#ifndef JGB_MUTEX_HPP
#define JGB_MUTEX_HPP

#include <boost/thread/mutex.hpp>

class mymutex{
    public:
        mymutex();
        ~mymutex();
        int lock();
        int trylock();
        int unlock();
        boost::mutex *getMutex();
    private:
        boost::mutex *m_mutex;
};

#endif