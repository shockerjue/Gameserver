#include "./../include/environment.h"

namespace engine {
    map<int,Value> Environment::s_args;

    void Environment::parseArgs(int argc,const char* argv[]){
        if(1 >= argc){
            return ;
        }

        int i = 0;
        for(i = 1; i < argc; ++i){
            Environment::setArgs(i,string(argv[i]));
        }
    }

    void Environment::setArgs(int key,string value) {
        if(0 == key || 0 == value.size()){
            return ;
        }

        Value _value = value;
        map<int,Value>::iterator it = Environment::s_args.find(key);
        if(it != Environment::s_args.end()){
            return ;
        }

        Environment::s_args[key] = _value;
    }

    bool Environment::hasArgs(string arg){
        if(0 == arg.size()){
            return false;
        }

        map<int,Value>::iterator it = Environment::s_args.begin();
        while(it != Environment::s_args.end()){
            if(it->second == arg){
                return true;
            }

            it++;
        }

        return false;
    }

    Value Environment::getArgsByKey(int key){
        Value _v;
        if(!key){
            return _v;
        }

        map<int,Value>::iterator it = Environment::s_args.find(key);
        if(it == Environment::s_args.end()){
            return _v;
        }

        return it->second;
    }
}