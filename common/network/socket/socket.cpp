#include "socket.h"

namespace engine {

    namespace network{
        
        map<int,int>    Socket::s_counter;

        Socket::Socket(void){
            this->m_socket = 0;
            this->m_type = 0;
            this->m_port = 0;
        }

        Socket::Socket(const network::Protocol type){
            this->m_socket = 0;
            this->m_type = 0;
            this->m_port = 0;

            switch (type) {
                case network::Protocol::TCP:{
                    this->m_socket = socket(AF_INET , SOCK_STREAM , 0);
                }break;
                case network::Protocol::UDP:{
                    this->m_socket = socket(AF_INET , SOCK_DGRAM  , 0);
                }break;
                default:{
                    this->m_socket = -1;
                }break;
            }

            if (this->m_socket){
               this->addcounter();
            }
        }

        Socket::Socket(const int socket){
            if(!socket){
                return ;
            }

            this->m_socket = socket;
            Socket::s_counter[this->m_socket] = 1;
        }

        Socket::Socket(const Socket& socket){
            this->m_type = socket.m_type;
            this->m_port = socket.m_port;
            this->m_socket = socket.m_socket;
            this->m_socketAddress = socket.m_socketAddress;

            this->addcounter();

            return ;
        }

        Socket& Socket::operator=(const Socket& socket){
            if(this == &socket){
                return *this;
            }

            this->m_type = socket.m_type;
            this->m_port = socket.m_port;
            this->m_socket = socket.m_socket;
            this->m_socketAddress = socket.m_socketAddress;

            this->addcounter();

            return *this;
        }

        int Socket::isConnect(void){
            if(0 >= this->m_socket){
                return 0;
            }

            return 1;
        }

        bool Socket::isopen(void){
            if(0 >= this->m_socket){
                perror("Didn't create socket!");

                return false;
            }

            return true;
        }

        void Socket::addcounter(void){
             if (this->m_socket){
                map<int,int>::iterator it = Socket::s_counter.find(this->m_socket);
                if(it == Socket::s_counter.end()){
                    Socket::s_counter[this->m_socket] = 1;
                }else {
                    int counter = it->second + 1;
                    Socket::s_counter[this->m_socket] = counter;
                }
            }
        }

        bool Socket::Create(const network::Protocol type){
            if(0 < this->m_socket){
                perror("Already call socket function!");

                return true;
            }

            this->m_socket = 0;
            switch (type) {
                case network::Protocol::TCP:{
                    this->m_socket = socket(AF_INET , SOCK_STREAM , 0);
                }break;
                case network::Protocol::UDP:{
                    this->m_socket = socket(AF_INET , SOCK_DGRAM  , 0);
                }break;
                default:{
                    this->m_socket = -1;
                }break;
            }

            if (this->m_socket){
                this->addcounter();
            }else{
                return false;
            }

            return true;
        }

        bool  Socket::SetSocket(const int socket){
            if(!socket){
                return false;
            }

            if(this->m_socket){
                perror("Already connected!");

                return false;
            }

            this->m_socket = socket;

            return true;
        }

        bool Socket::Bind(SocketAddress socketAddress,unsigned int port){
            if(!this->isopen()){
                return false;
            }

            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);

            string ip = socketAddress.getHostAddress();
            inet_pton(AF_INET,ip.c_str(),&addr.sin_addr);

            socklen_t alen = sizeof(struct sockaddr);

            int ret = bind(this->m_socket,(const struct sockaddr*)&addr,alen);
            if(-1 == ret ){
                perror(strerror(errno));

                return false;
            }

            return true;
        }

        bool Socket::Bind(const char* ip,unsigned short port){
            if(nullptr == ip || !port){
                perror("ip is nullptr!");

                return false;
            }

            if(!this->isopen()){
                return false;
            }

            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            inet_pton(AF_INET,ip,&addr.sin_addr);

            socklen_t alen = sizeof(struct sockaddr);

            int ret = bind(this->m_socket,(const struct sockaddr*)&addr,alen);
            if(-1 == ret ){
                perror(strerror(errno));

                return false;
            }

            return true;
        }

        bool Socket::Listen(int blog){
            if(!this->isopen() || !blog){
                return false;
            }

            int ret = listen(this->m_socket,blog);
            if(-1 == ret){
                perror(strerror(errno));

                return false;
            }

            return true;
        }

        bool Socket::SetMode(network::fdmode mode){
            switch (mode) {
                case network::fdmode::BLK:{

                }break;
                case network::fdmode::NBLK:{
                    int old_option = fcntl(this->m_socket,F_GETFL);
                    int new_option = old_option | O_NONBLOCK;
                    fcntl(this->m_socket, F_SETFL, new_option);
                }break;
            }

            return true;
        }

        bool Socket::SetOpt(int level, int optname,const void *optval,int len){
            if(nullptr == optval || !len){
                return false;
            }

            if((setsockopt(this->m_socket,level,optname,optval,len)) < 0)
            {
                perror("setsockopt failed");

                return false;
            }

            return true;
        }

        bool Socket::Connect(const char* ip,unsigned short port){
            if(nullptr == ip || !port){
                perror("id is nullptr");

                return false;
            }

            if(!this->isopen()){
                return false;
            }

            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            inet_pton(AF_INET,ip,&addr.sin_addr);

            socklen_t alen = sizeof(struct sockaddr);

            int ret = connect(this->m_socket,(struct sockaddr*)&addr,alen);
            if(-1 == ret){
                perror(strerror(errno));

                return false;
            }

            this->m_port = port;
            this->m_socketAddress = SocketAddress(ip,4);


            return true;
        }

        bool Socket::Connect(SocketAddress socketAddress, unsigned short port){
            if(!this->isopen() || !port){
                return false;
            }

            struct sockaddr_in addr;

            bzero(&addr,sizeof(struct sockaddr_in));
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);

            string ip = socketAddress.getHostAddress();
            inet_pton(AF_INET,ip.c_str(),&addr.sin_addr);

            socklen_t alen = sizeof(struct sockaddr);

            int ret = connect(this->m_socket,(struct sockaddr*)&addr, alen);
            if(-1 == ret){
                perror(strerror(errno));

                return false;
            }

            this->m_port = port;
            this->m_socketAddress = socketAddress;

            return true;
        }

        int Socket::GetNetId(void){
            return this->m_socket;
        }

        int Socket::Send(const char* buf,int size){
            if(nullptr == buf || !size){
                return -1;
            }

            if(!this->isopen()){
                return -1;
            }

            int ret = send(this->m_socket,buf,size,0);//MSG_NOSIGNAL);

            return ret;
        }

        int Socket::Recv(char* buf, int size){
            if(!this->isopen() || nullptr == buf ||
                !size){
                return -1;
            }

            int ret = recv(this->m_socket,buf,size,0);

            return ret;
        }

        bool Socket::Accept(Socket* socket){
            if(nullptr == socket){
                return false;
            }

            if(!this->isopen()){
                return false;
            }

            struct sockaddr_in addr;
            socklen_t alen = sizeof(struct sockaddr);

            int ret = accept(this->m_socket,(struct sockaddr*)&addr,&alen);
            if(-1 == ret){
                return false;
            }

            socket->m_socket = ret;
            socket->m_port = addr.sin_port;
            socket->m_socketAddress = SocketAddress(inet_ntoa(addr.sin_addr),4);

            socket->addcounter();

            return true;
        }

        SocketAddress Socket::getAddress(void){
            return this->m_socketAddress;
        }

        void Socket::Disconnect(){
             if (this->m_socket){
                map<int,int>::iterator it = Socket::s_counter.find(this->m_socket);
                if(it != Socket::s_counter.end()){
                    int counter = it->second - 1;
                    Socket::s_counter[this->m_socket] = counter;

                     if(0 >= counter){
                        close(this->m_socket);

                        Socket::s_counter.erase(this->m_socket);

                        this->m_socket = -1;

                        return ;
                    }
                }
            }
           
        }

        Socket::~Socket(){
            this->Disconnect();
        }
    }

}