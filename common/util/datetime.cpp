#include "datetime.h"

namespace util {
    string Datetime::getCurrentDate(void){
        time_t tm;
        time(&tm);

        struct tm *p = localtime(&tm);

        char dt[64] = {0};

        sprintf(dt,"%d-%d-%d %d:%d:%d",1900 + p->tm_year,
            1 + p->tm_mon,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);

        return string(dt);
    }

    string Datetime::getCurrentDateForCST(void){
        time_t tm;
        time(&tm);

        string dt(ctime(&tm));
        dt[dt.size() - 1] ='0';

        return dt;
    }

    long    Datetime::getTimestamp(void){
        return time(nullptr);
    }

    long    Datetime::getTimeustamp(void){
        struct timeval start;
		gettimeofday(&start,nullptr);

        return start.tv_sec + start.tv_usec;
    }
}
