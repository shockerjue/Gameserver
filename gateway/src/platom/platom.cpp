#include "platom.h"

namespace platom {
    Platom* Platom::s_pPlatom = nullptr;

    Platom::Platom(void){

    }

    Platom::~Platom(void) {

    }

    Platom* Platom::getPlatom(void) {
        if(nullptr == Platom::s_pPlatom) {
            Platom::s_pPlatom = new Platom();
        }

        return Platom::s_pPlatom;
    }

    void Platom::parse(string config) {
        if(0 == config.size()) {
            return ;
        }

        try{
            int fd = open(config.c_str(),O_RDONLY);
            if(-1 == fd){
                Log::Perror(__func__, "Open platom config file error! file: " + config);

                return ;
            }

            char buf[1048576] = {0};
            read(fd,buf,1048576);
            string jstr(buf);
            auto j3 = json::parse(jstr);
            
            int i = 1;
            int total = j3["platom_count"];
            for(i = 1 ; i <= total; ++i) {
                string key = Util::int2str(i);
                struct PlatomItem item;
                item.m_platomid = j3[key]["platom_id"];

                this->m_platoms.push_back(item);

                Log::Print(__func__, item.toString());
            }
        }catch( ... ){
            Log::Perror(__func__, "happened except");
        }
    }

    bool Platom::verification(string platomid) {
        if(0 == platomid.size()) {
            return false;
        }

        vector<struct PlatomItem>::iterator it = this->m_platoms.begin();
        while(it != this->m_platoms.end()) {
            if(0 == platomid.compare(it->m_platomid)) {
                return true;
            }

            it++;
        }

        return false ;
    }
}