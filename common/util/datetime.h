#ifndef H_DATETIME_H_
#define H_DATETIME_H_

#ifdef _cplusplus
    extern "C"{
#endif
    #include <time.h>
    #include <sys/time.h>
#ifdef _cplusplus
    }
#endif

#include <iostream>

using namespace std;

namespace util {
    class Datetime {
    public:
        /**
        * 获取当前的时间戳(秒级)
        *
        * @return long
        */
        static long   getTimestamp(void);

        /**
        * 获取当前的时间戳（微秒级）
        *
        * @return long
        */
        static long    getTimeustamp(void);

        /**
        * 获取当前的日期和时间
        * 格式：1900-01-01 00:00:01
        *
        * @return string
        */
        static string getCurrentDate(void);

        /**
        * 获取当前的日期和时间 
        * 格式：2017年 07月 16日 星期日 00:13:28 CST
        *
        * @return string
        */
        static string getCurrentDateForCST(void);

    private:

    };
}

#endif
