#ifndef H_ENVIRONMENT_H_
#define H_ENVIRONMENT_H_

#include "value.h"

#include <map>
#include <iostream>

using namespace std;

namespace engine {

    class Environment {
    public:
        static void parseArgs(int argc,const char* argv[]);
        static bool hasArgs(string arg);
        static Value getArgsByKey(int key);

    private:
        static map<int,Value> s_args;

        static void setArgs(int key,string value);
    };

}
#endif