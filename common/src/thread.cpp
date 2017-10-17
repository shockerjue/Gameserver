#include "./../include/thread.h"

namespace engine {

	namespace thread {
		
		Message::Message(void):
		what(-1),
		size(0),
		obj(nullptr){

		}

		Message::~Message(void){
			if(nullptr != this->obj){
				delete[] this->obj;

				this->obj = nullptr;
			}

			this->what = -1;
			this->size = 0;
		}

		void Message::setsize(const int size){
			this->size = size;
		}

		void Message::setwhat(const int what){
			this->what = what;
		}

		int Message::getwhat(void){
			return this->what;
		}

		int Message::getsize(void){
			return this->size;
		}

		Message::Message(const Message& message){
			this->what = message.what;
			this->size = message.size;
			
			if(0 < this->size){
				this->obj = new char[message.size + 1];
				
				if(nullptr != this->obj){
					memcpy(this->obj,message.obj,message.size);

					return ;
				}
			}

			this->size = 0;

			return ;
		}

		Message& Message::operator=(const Message& message){
			if(this == &message){
				return *this;
			}

			if(nullptr != this->obj){
				delete[] this->obj;

				this->obj = nullptr;
				this->size = 0;
			}

			this->what = message.what;
			this->size = message.size;

			if(0 < message.size){
				this->obj = new char[message.size + 1];
				if(nullptr != this->obj){
					this->size = message.size;
					memcpy(this->obj,message.obj,message.size);

					return *this;
				}
			}

			this->size = 0;

			return *this;
		}

		void Message::setobject(const void* object,int size){
			if(nullptr == object || !size){
				return ;
			}

			if(nullptr != this->obj){
				delete[] this->obj;

				this->obj = nullptr;
			}

			this->obj = new char[size + 1];
			if(nullptr == this->obj){
				Log::Warning(__func__, "Allocate memory fail!");

				this->obj = nullptr;
				this->size = 0;

				return ;
			}

			memcpy(this->obj,object,size);
			this->size = size;

			return ;
		}

		void* Message::getobject(void* object,int* size) {
			if(nullptr == object || 0 == this->size ||
				nullptr == size){
				object = nullptr;

				return object;
			}

			if(nullptr == this->obj){
				object = nullptr;

				return object;
			}

			memcpy(object,this->obj,this->size);
			*size = this->size;

			return object;
		}

		void* Message::getobject(void* object,int size){
			if(nullptr == object || !size){
				object = nullptr;

				return object;
			}

			if(nullptr == this->obj || this->size < size){
				object = nullptr;

				return object;
			}

			memcpy(object,this->obj,size);

			return object;
		}

		string Message::toString(void){
			char buf[64] = {0};

			sprintf(buf, "{what: %d, size: %d}",
				this->what,
				this->size);

			return string(buf);
		}

		map<string,Thread> 					Thread::s_threads;
		map<ThreadId,struct CounterItem > 	Thread::s_thread_counter;

		Thread Thread::sget_thread_by_name(string name){
			Thread temp("");

			if(0 == name.size()){
				return temp;
			}

			map<string,Thread>::iterator it = Thread::s_threads.find(name);
			if(it != Thread::s_threads.end()){
				return it->second;
			}

			return temp;
		}

		Thread::Thread():
		m_send(-1),
		m_recv(-1),
		m_thread_id(0),
		m_runnable(false){
			this->m_thread_name = "";
		}

		Thread::Thread(const string name):
		m_send(-1),
		m_recv(-1),
		m_thread_id(0),
		m_runnable(false){
			this->m_thread_name = name;
		}

		Thread::~Thread(void){
			this->sub_counter();
		}

		Thread::Thread(const Thread& thread){
			this->m_send = thread.m_send;
			this->m_recv = thread.m_recv;
			this->m_runnable = thread.m_runnable;
			this->m_thread_id = thread.m_thread_id;
			this->m_thread_name = thread.m_thread_name;

			this->add_counter();

			return ;
		}

		Thread& Thread::operator=(const Thread& thread){
			if(this == &thread){
				return *this;
			}

			this->m_send = thread.m_send;
			this->m_recv = thread.m_recv;
			this->m_runnable = thread.m_runnable;
			this->m_thread_id = thread.m_thread_id;
			this->m_thread_name = thread.m_thread_name;

			this->add_counter();

			return *this;
		}

		void Thread::set_name(const string name){
			this->m_thread_name = name;
		}

		string Thread::get_name(void){
			return this->m_thread_name;
		}


		void Thread::run(const string name,ThreadFunc runnable){
			this->set_name(name);
			this->run(runnable);
		}

		void Thread::run(const string name,ThreadFunc runnable,void* obj){
			this->set_name(name);
			this->run(runnable,obj);
		}

		void Thread::run(ThreadFunc runnable){
			this->run(runnable,this);
		}

		void Thread::run(ThreadFunc runnable,void* obj){
			if(nullptr == runnable){
				return ;
			}

			if(this->m_thread_id){
				Log::Warning(__func__, "Thread already running!");

				return ;
			}

			if(this->m_runnable){
				Log::Warning(__func__, "Thread already running!");

				return ;
			}

			if(0 == this->get_name().size()){
				Log::Warning(__func__, "Thread name is empty! reset thread's name.");

				return ;
			}

			if(true == this->isexsits(this->get_name())){
				Log::Warning(__func__, "Name thread already exists!reset name to run.");

				return ;
			}

			if(0 > this->m_recv || 0 > this->m_send){
				int fd[2] = {0};
				if(socketpair(AF_UNIX,SOCK_STREAM,0,fd) == -1){
					return ;
				}

				this->m_recv = fd[0];
				this->m_send = fd[1];
			}

			pthread_create(&this->m_thread_id,nullptr,runnable,obj);

			if(!this->m_thread_id){
				if(0 < this->m_recv){
					close(this->m_recv);
				}

				if(0 < this->m_send){
					close(this->m_send);
				}

				return ;
			}

			this->m_runnable = true;
			this->add_counter();

			Thread::s_threads[this->get_name()] = *this;
		}

		void Thread::kill(void){
			if(0 < this->m_thread_id){
				pthread_kill(this->m_thread_id,SIGKILL);

				this->m_thread_id = 0;
				this->m_runnable = false;

				if(0 < this->m_send){
					close(this->m_send);

					this->m_send = 0;
				}

				if(0 < this->m_recv){
					close(this->m_recv);

					this->m_recv = 0;
				}

				Thread::s_threads.erase(this->get_name());
				Thread::s_thread_counter.erase(this->m_thread_id);
			}
		}

		void Thread::wait(void){
			if(0 < this->m_thread_id){
				pthread_join(this->m_thread_id,NULL);

				this->m_thread_id = 0;
				this->m_runnable = false;

				if(0 < this->m_send){
					close(this->m_send);

					this->m_send = 0;
				}

				if(0 < this->m_recv){
					close(this->m_recv);

					this->m_recv = 0;
				}

				Thread::s_threads.erase(this->get_name());
				Thread::s_thread_counter.erase(this->m_thread_id);
			}
		}

		int Thread::getrecvfd(void){
			return this->m_recv;
		}

		int Thread::getsendfd(void) {
			return this->m_send;
		}

		bool Thread::isexsits(const string name){
			if(0 == name.size()){
				return true;
			}

			map<string,Thread>::iterator it = Thread::s_threads.find(name);
			if(it == Thread::s_threads.end()){
				return false;
			}

			return true;
		}

		void Thread::add_counter(void){
			if(0 > this->m_thread_id){
				return ;
			}

			map<ThreadId,struct CounterItem >::iterator it = 
					Thread::s_thread_counter.find(this->m_thread_id);

			if(it == Thread::s_thread_counter.end()){
				struct CounterItem counter;
				counter._send = 1;
				counter._recv = 1;

				Thread::s_thread_counter[this->m_thread_id] = counter;

				return ;
			}

			it->second._send = it->second._send + 1;
			it->second._recv = it->second._recv + 1;

			return ;
		}

		void Thread::sub_counter(void){
			if(0 > this->m_thread_id){
				return ;
			}

			map<ThreadId,struct CounterItem >::iterator it = 
					Thread::s_thread_counter.find(this->m_thread_id);

			if(it == Thread::s_thread_counter.end()){
				return ;
			}

			it->second._send = it->second._send - 1;
			it->second._recv = it->second._recv - 1;

			if(0 >= it->second._send || 0 >= it->second._recv){
				if(this->alive()){
					this->wait();
				}

				this->kill();
			}

			return ;
		}

		bool Thread::alive(void){
			if(0 > this->m_thread_id){
				return false;
			}

			int pthread_kill_err = pthread_kill(this->m_thread_id,0);

			if(pthread_kill_err == ESRCH){
				return false;
			}else if(pthread_kill_err == EINVAL){
				return false;
			}else{
				return true;
			}

			return false;
		}

		bool Thread::send_message_to_main(Message message){
			if(nullptr == message.obj || !message.size || 
				0 >= this->m_send){
				return false;
			}

			int len = sizeof(message.what) + sizeof(message.size) + message.size;
			char* buf = new char[len + 1];
			if(nullptr == buf){
				return false;
			}

			memcpy(buf,&message.what,sizeof(message.what));
			memcpy(buf + sizeof(message.what),&message.size,sizeof(message.size));
			message.getobject(buf + sizeof(int) + sizeof(int),message.size);

			int ret = send(this->m_send,(char*)buf,len,0);
			
			delete[] buf;
			buf = nullptr;

			return true;
		}

		bool Thread::send_message_to_thread(Message message){
			if(nullptr == message.obj || !message.size || 
				0 >= this->m_recv){
				return false;
			}

			if(false == this->alive()){
				return false;
			}

			int len = sizeof(message.what) + sizeof(message.size) + message.size;
			char* buf = new char[len + 1];
			if(nullptr == buf){
				return false;
			}

			memcpy(buf,&message.what,sizeof(message.what));
			memcpy(buf + sizeof(message.what),&message.size,sizeof(message.size));
			message.getobject(buf + sizeof(int) + sizeof(int),message.size);

			int ret = send(this->m_recv,(char*)buf,len,0);
			
			delete[] buf;
			buf = nullptr;

			Log::Print(__func__,"Send message finished!");

			return true;
		}

		vector<Message>  Thread::recv_message_in_thread(int* s){
			vector<Message> msgs;

			if(0 >= this->m_send){
				*s = 0;

				return msgs;
			}

			char buf[1024] = {0};
			int len = recv(this->m_send,buf,1024,0);
			if(0 >= len){
				*s = 0;
				
				return msgs;
			}

			int _len = 0;
			const char* p = buf;
			while(p != nullptr && _len < len){
				Message message;
				message.setwhat(*((int*)p));
				message.setsize(*((int*)(p + sizeof(int))));
				message.setobject(p + sizeof(int) + sizeof(int),message.getsize());

				_len = _len + sizeof(int) + sizeof(int) + message.getsize();
				p = p + sizeof(int) + sizeof(int) + message.getsize();

				msgs.push_back(message);

				*s = *s + 1;
			}

			return msgs;
		}

		void Thread::srun(Thread thread,ThreadFunc runnable){
			if(nullptr == runnable){
				return ;
			}

			thread.run(runnable);
		}

		void Thread::srun(const string name,ThreadFunc runnable){
			if(0 == name.size() || nullptr == runnable){
				return ;
			}

			map<string,Thread>::iterator it = Thread::s_threads.find(name);
			if(it == Thread::s_threads.end()){
				return ;
			}

			it->second.run(runnable);
		}

		void Thread::srun(Thread thread,ThreadFunc runnable,void* obj){
			if(nullptr == runnable){
				return ;
			}

			thread.run(runnable,obj);
		}

		void Thread::srun(const string name,ThreadFunc runnable,void* obj){
			if(0 == name.size() || nullptr == runnable){
				return ;
			}

			map<string,Thread>::iterator it = Thread::s_threads.find(name);
			if(it == Thread::s_threads.end()){
				return ;
			}

			it->second.run(runnable,obj);
		}

		void Thread::skill(Thread thread){
			thread.kill();
		}

		void Thread::swait(Thread thread){
			thread.wait();
		}

		void	Thread::skill(const string name){
			if(0 == name.size()){
				return ;
			}


			map<string,Thread>::iterator it = Thread::s_threads.find(name);
			if(it == Thread::s_threads.end()){
				return ;
			}

			it->second.kill();
		}

		void	Thread::swait(const string name){
			if(0 == name.size()){
				return ;
			}

			map<string,Thread>::iterator it = Thread::s_threads.find(name);
			if(it == Thread::s_threads.end()){
				return ;
			}

			it->second.wait();
		}

		bool Thread::ssend_message_to_thread(Thread thread,Message message){
			return thread.send_message_to_thread(message);
		}

		bool Thread::ssend_message_to_thread(string name,Message message){
			if(0 == name.size()){
				return false;
			}

			map<string,Thread>::iterator it = Thread::s_threads.find(name);
			if(it == Thread::s_threads.end()){
				return false;
			}

			return it->second.send_message_to_thread(message);
		}

		vector<Message>  Thread::srecv_message_in_thread(Thread thread,int* s){
			return thread.recv_message_in_thread(s);
		}

		vector<Message>  Thread::srecv_message_in_thread(const string name,int* s){
			vector<Message> temp;
			*s = 0;

			if(0 == name.size()){
				return temp;
			}

			map<string,Thread>::iterator it = Thread::s_threads.find(name);
			if(it == Thread::s_threads.end()){
				return temp;
			}

			return it->second.recv_message_in_thread(s);
		}
	}
}