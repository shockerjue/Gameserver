#include "ddos.h"
#include "timeout.h"
#include "threadimplement.h"

namespace engine {

	namespace ddos {
		
		Thread_Func(ddos) {
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
			DDos ddos;

			while(1){
				FD_SET(fd,&readfds);
				
				timeout.tv_sec = 1;
				timeout.tv_usec = 0;

				int ret = select(fd + 1,&readfds,nullptr,nullptr,&timeout);
				if(ret < 0){
					Log::Perror(__func__,"Thread already over!");

					break ;
				}

				/**
				* 添加或者更新消息
				*/
				if(0 < ret && FD_ISSET(fd,&readfds)){
					int number = 0;
					vector<Message> msg = thread->recv_message_in_thread(&number);
					if(0 == number){
						continue;
					}

					ddos.new_msg(msg);
				}

				/**
				* select超时处理，即定时器
				*/
				if(ret == 0){
					// timeout handle

					ddos.check(*thread);

					continue ;
				}
			}

			return nullptr;
		}

		Thread_Func(timeout) {
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
			Timeout ddos;

			while(1){
				FD_SET(fd,&readfds);
				
				timeout.tv_sec = 1;
				timeout.tv_usec = 0;

				int ret = select(fd + 1,&readfds,nullptr,nullptr,&timeout);
				if(ret < 0){
					Log::Perror(__func__,"Thread already over!");

					break ;
				}

				if(0 < ret && FD_ISSET(fd,&readfds)){
					int number = 0;
					vector<Message> msg = thread->recv_message_in_thread(&number);
					if(0 == number){
						continue;
					}

					ddos.new_msg(msg);
				}

				if(ret == 0){
					// timeout handle

					ddos.check(*thread);

					continue ;
				}
			}

			return nullptr;
		}

	}

}