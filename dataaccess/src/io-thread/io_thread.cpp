#include "io_thread.h"

namespace io_thread {
    
    Thread_Func(io_read){
        Thread* thread = (Thread*)_thread;

        int fd = thread->getsendfd();
        if(0 > fd){
            return nullptr;
        }

        Network network(fd);
        network.SetType(ConnectType::THREAD_TYPE);
        network.SetMode(network::fdmode::NBLK);

        fd_set readfds;
        struct timeval timeout;
        FD_ZERO(&readfds);

        MysqlSlave mysqlSlave;
        ShareCache* share_ = ShareCache::getShareCache();

        Read read;
        int i = 0;
        while(1){
            FD_SET(fd,&readfds);
            timeout.tv_sec = 5;
            timeout.tv_usec = 0;

            int ret = select(fd + 1,&readfds,nullptr,nullptr,&timeout);
            if(ret < 0){
                Log::Perror(__func__,"Thread already over!");

                break ;
            }

            /**
            * 接收来自主线程的读消息
            */
            if(0 < ret && FD_ISSET(fd,&readfds)){
                int number = 0;
                vector<Message> msgs = thread->recv_message_in_thread(&number);
                if(0 == number){
                    continue;
                }

                read.new_msg(&mysqlSlave,msgs);
            }
        }

        return nullptr;
    }

    Thread_Func(io_write){
        Thread* thread = (Thread*)_thread;

        int fd = thread->getsendfd();
        if(0 > fd){
            return nullptr;
        }

        Network network(fd);
        network.SetType(ConnectType::THREAD_TYPE);
        network.SetMode(network::fdmode::NBLK);

        fd_set readfds;
        struct timeval timeout;
        FD_ZERO(&readfds);

        MysqlMaster mysqlMaster;
        ShareCache* share_ = ShareCache::getShareCache();

        Write write;
        while(1){
            FD_SET(fd,&readfds);
            timeout.tv_sec = 5;
            timeout.tv_usec = 0;

            int ret = select(fd + 1,&readfds,nullptr,nullptr,&timeout);
            if(ret < 0){
                Log::Perror(__func__,"Thread already over!");

                break ;
            }

            /**
            * 接收来之主线程写数据的线程消息
            */
            if(0 < ret && FD_ISSET(fd,&readfds)){
                int number = 0;
                vector<Message> msgs = thread->recv_message_in_thread(&number);
                if(0 == number){
                    continue;
                }

                write.new_msg(&mysqlMaster,msgs);
            }
        }

        return nullptr;
    }

    Thread_Func(io_sync) {
        Thread* thread = (Thread*)_thread;

        struct timeval timeout;

        Sync sync;
        MysqlMaster mysqlMaster;
        ShareCache* share_ = ShareCache::getShareCache();
        
        while(1){
            timeout.tv_sec = 60;
            timeout.tv_usec = 0;

            int ret = select(0,nullptr,nullptr,nullptr,&timeout);
            if(ret < 0){
                Log::Perror(__func__,"Thread already over!");

                break ;
            }

            sync.sync(share_,&mysqlMaster);
        }

        return nullptr;
    }
}