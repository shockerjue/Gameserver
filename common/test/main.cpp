#include "./../engine/engine.h"
#include "./../include/type.h"
#include "./../include/value.h"
#include "./../include/thread.h"
#include "./../util/log.h"
#include "./../include/configmodule.h"
#include "./../db/db.h"
#include "ncallback.h"

#include <typeinfo>

using namespace util;
using namespace config;
using namespace engine;
using namespace engine::db;
using namespace engine::thread;

class NCallback;

void mysql_test(ConfigModule* config){
        MysqlUtil* pMysqlUtil = new MysqlUtil();
        pMysqlUtil->SetHost(config->getStringByKey("dbhost"));
        pMysqlUtil->SetPort(config->getIntByKey("dbport"));
        pMysqlUtil->SetDbName(config->getStringByKey("dbname"));
        pMysqlUtil->SetUser(config->getStringByKey("dbuser"));
        pMysqlUtil->SetPass(config->getStringByKey("dbpass"));

        Log::Print(__func__, pMysqlUtil->toString());
        bool ret = pMysqlUtil->InitConnection();
        //
        // int rows = pMysqlUtil->Exec("INSERT INTO ems(ems_filename,ems_date) value('11111111111111175','2017-06-16')");
        // int rows = pMysqlUtil->Exec("UPDATE ems SET ems_filename='0000000000000000' WHERE ems_id=222");
        // Log::Print(__func__,rows);
        vector<map<string,Value> > data = pMysqlUtil->Selection("SELECT * FROM ems LIMIT 0,10");
        vector<map<string,Value> >::iterator it = data.begin();
        while(it != data.end()){
                map<string,Value>::iterator temp = it->begin();
                while(temp != it->end()){
                        cout<<temp->first<<":"<<temp->second.toString()<<"  ";
                        temp++;
                }
                cout<<endl;

                it++;
        }

        delete pMysqlUtil;
        pMysqlUtil = nullptr;
}

void redis_test(ConfigModule* config){
        RedisUtil* pRedisUtil = new RedisUtil();
        pRedisUtil->SetHost(config->getStringByKey("rshost"));
        pRedisUtil->SetPort(config->getIntByKey("rsport"));

        Log::Print(__func__, pRedisUtil->toString());

        bool ret = pRedisUtil->InitConnection();
        if(ret){
                Log::Print(__func__, "Connect redis success!");
        }

        pRedisUtil->Set("name","shockerjue");
        Log::Print(__func__,pRedisUtil->Get("name").toString());

        delete pRedisUtil;
        pRedisUtil = nullptr;
}

Thread_Func(thread1){
        Thread* th = (Thread*)_thread;
        sleep(10);
        Log::Print(__func__,"Thread::sendMessage  over!");

        return nullptr;
}

void thread_test(void){
        Thread thread("thread1");
        thread.run(Thread_Call(thread1));

        Log::Print(__func__, "Wait thread .....");
        thread.wait();  
}

struct Test {
        int a;
        int b;
};

void struct_test(void){
        struct Test test;
        test.a = 90;
        test.b = 100;
        Log::Print(__func__, typeid(test).name());
        Log::Print(__func__, typeid(test.a).name());
        Log::Print(__func__, typeid(test.a).name());
}

int main(int argc,const char* argv[]){
        // Engine engine;
        // engine.onInit(5623);
        // engine.onStart();

        // NCallback ncall;
        // engine.SetMsgCallback(&ncall);
        // engine.Run();

        ConfigModule* config = ConfigModule::getConfigModule();
        if(nullptr == config){
                return 1;
        }

        config->onLoad("./readme");
        Log::s_proc_name = config->getStringByKey("name");
        Log::Print(__func__,config->getStringByKey("address"));
        
        // mysql_test(config);
        // redis_test(config);
        // thread_test();
        struct_test();

        delete config;
        config = nullptr;
        
        return 0;
}