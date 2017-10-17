#ifndef H_LOG_H_
#define H_LOG_H_

#ifdef _cplusplus
    extern "C"{
#endif
    #include <stdarg.h>
    #include <stdio.h>
#ifdef _cplusplus
    }
#endif

#include <string>
#include <iostream>

#include "datetime.h"

using namespace std;

namespace util{
    class Log{
    public:
        template<typename T,typename... Args>
        static void Print(string __func, T msg, Args... args){
            string dt = Datetime::getCurrentDate();
            cout<<s_proc_name<<"["<<dt<<"][I]   "<<__func<<" : "<<msg<<endl;
        }

        static void Print(string proc ,string func, string date, string msg){
            cout<<proc<<"["<<date<<"][I]    "<<func<<" : "<<msg<<endl; 
        }

        template<typename T,class... Args>
        static void Perror(string __func,T msg,Args... args){
            string dt = Datetime::getCurrentDate();
            cout<<s_proc_name<<"["<<dt<<"][E]   "<<__func<<" : "<<msg<<endl;
        }

        static void Perror(string proc ,string func, string  date, string msg){
            cout<<proc<<"["<<date<<"][E]    "<<func<<" : "<<msg<<endl; 
        }

        template<typename T,class... Args>
        static void Warning(string __func,T msg,Args... args){
            string dt = Datetime::getCurrentDate();
            cout<<s_proc_name<<"["<<dt<<"][W]       "<<__func<<" : "<<msg<<endl;
        }

        static void Warning(string proc ,string func, string date, string msg){
            cout<<proc<<"["<<date<<"][W]    "<<func<<" : "<<msg<<endl; 
        }

        static string   s_proc_name;
    };
}

#endif
