#ifndef __MY_EXCEPTION__
#define __MY_EXCEPTION__

#include <exception>
#include <string>

#define TIMEBEGIN(num) clock_t  start##num = clock();

#define TIMEEND(num)  clock_t  end##num = clock(); { double seconds  =(double)(end##num - start##num)/CLOCKS_PER_SEC;\
                      fprintf(stderr, "Use time is: %.6f\n", seconds);}

class myException :public std::exception{
    private:
        std::string des_;
    public:
        myException(const std::string &des){
            des_ = des;
        }
        const char * what() const throw(){
            return des_.c_str();
        }
};

#endif