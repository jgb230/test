#include "leetCodeEasy.h"
#include <string.h>

#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))
#define MYEXCEPTION(x) {char des[1024];sprintf(des, "%s %d %s [%s]", __FILE__, __LINE__, __func__, x); throw myException(des);}

int guessGame(int a[], int b[]){
    int ret = (a[0] == b[0]) + (a[1] == b[1]) + (a[2] == b[2]);
    std::cout << "ret:" << ret << std::endl;
    return 0;
}

bool isChar(char *s){
    bool ret = true;
    for(int i = 0; i < (int)strlen(s); i++){
        if(!((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z'))){
            ret = false;
            break;
        }
    }
    return ret;
}

#define LOCATION(s) ((s >= 'a')?(int)(s - 'a' + 26):(int)s - 'A')

int numJewelsInStones(char *j, char *s){
    int arr[52] = {0};
    int count = 0;
    for(int i = 0; i < (int)strlen(s); i++){
        arr[LOCATION(s[i])]++;
    }
    for(int i = 0; i < (int)strlen(j); i++){
        count += arr[LOCATION(j[i])];
    }
    std::cout << "count:" << count << std::endl;
    return 0;
}

void testEasy(){
    try{
        {
            int a[3] = {1};
            int b[3] = {1, 2, 3};
            if(ARRAYSIZE(a) != 3){
                MYEXCEPTION("数组a长度不为3~");
            }
            if(ARRAYSIZE(b) != 3){
                MYEXCEPTION("数组b长度不为3~");
            }
            guessGame(a, b);
        }
        {
            char *j = "Aa1";
            char *s = "absdcsAA";
            if(!isChar(j)){
                MYEXCEPTION("数组j不全为字母~");
            }
            if(!isChar(s)){
                MYEXCEPTION("数组s不全为字母~");
            }
            numJewelsInStones(j, s);
        }
    }catch (std::exception &e){
        std::cout << e.what() << std::endl;
    }
}