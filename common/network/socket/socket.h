/*************************************************************************
* 网络连接对象
* Dev：shockerjue
**************************************************************************/

#ifndef H_SOCKET_H_
#define H_SOCKET_H_

#include <iostream>
#include <map>

#ifdef _cplusplus
    extern "C"{
#endif
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <signal.h>
    #include <assert.h>
    #include <unistd.h>
    #include <string.h>
    #include <fcntl.h>
    #include <stdlib.h>
    #include <errno.h>
    #include <stdio.h>
    #include <time.h>
#ifdef _cplusplus
    }
#endif

#include "socketaddress.h"

using namespace std;

namespace engine {
    
    namespace network{

        /**
        * 创建网络连接的协议
        * 
        * TCP 
        * UDP
        */
        enum Protocol {
            TCP = 1,
            UDP = 2
        };

        /**
        * 网络连接的模型
        *
        * BLK       阻塞模式
        * NBLK      非阻塞模型
        */
        enum fdmode {
            BLK     = 1,
            NBLK    = 2,
        };

        /**
        * socket 类
        * 使用计数器来管理打开的socket连接，主要是为了
        * 使用复制或者赋值操作
        */
        class Socket {
        public:
            Socket(void);
            ~Socket(void);

            /**
            * 用已经有的socketid来初始化一个Socket对象
            *
            * @param socket
            */
            Socket(const int socket);

            Socket(const Socket& socket);

            /**
            * 使用协议类型创建Socket对象
            *
            * @param type
            */
            Socket(const network::Protocol type);

            Socket &operator=(const Socket& socket);

            /**
            * 将socket放入监听队列
            *
            * @param blog
            * @return bool
            */
            bool    Listen(int blog);

            /**
            * 设置socket的模式
            *
            * @param mode   模式[BLK、NBLK]
            * @return bool
            */
            bool    SetMode(network::fdmode mode);

            /**
            * 当socket中没有连接时，可以使用该方法设置句柄
            *
            * @param socket
            * @param bool
            */
            bool    SetSocket(const int socket);
            /**
            * 创建socket连接
            *
            * @param type   创建的类型[TCP、UDP]
            * @return bool
            */
            bool    Create(const network::Protocol type);

            /**
            * 将socket绑定到ip所指示的地址
            *
            * @param ip
            * @param port
            * @return bool
            */
            bool    Bind(const char* ip,unsigned short port);

            /**
            * 连接到ip所指示的地址
            *
            * @param ip
            * @param port
            * @return bool
            */
            bool    Connect(const char* ip,unsigned short port);

            /**
            * 将socket绑定到socket所指示的地址
            *
            * @param socket
            * @param port
            * @return bool
            */
            bool    Bind(SocketAddress socket,unsigned int port);

            /**
            * 设置socket的属性
            *
            * @param level
            * @param optname
            * @param optval
            * @param len
            */
            bool    SetOpt(int level, int optname,const void *optval,int len);

            /**
            * 连接到socketAddress指示的主机
            * 
            * @param    socketAddress
            * @param    port
            * @return   bool
            */
            bool    Connect(SocketAddress socketAddress, unsigned short port);

            /**
            * 关闭连接
            */
            void    Disconnect();

            /**
            * 获取对应socket对象的文件描述符句柄
            */
            int     GetNetId(void);

            /**
            * 接受新的网络连接
            *
            * @param socket     连接返回
            */
            bool    Accept(Socket* socket);

            /**
            * 接收数据
            * 
            * @param buf       接收数据缓冲区
            * @param size       接收的字节数
            * @return int 
            */
            int     Recv(char* buf, int size);

            /**
            * 发送数据
            *
            * @param    buf     需要发送的数据
            * @param    size    发送的数据大小
            */
            int     Send(const char* buf,int size);

            /**
            * 获取socket对应的SocketAddress 对象
            */
            SocketAddress   getAddress(void);

            /**
            * 设置socket类型，用于标识连接的类型
            *
            * @param type
            */
            inline void SetType(unsigned short type){
                this->m_type = type;
            }

            /**
            * 获取网络连接的类型
            */
            inline unsigned short GetType(void){
                return this->m_type;
            }

            /**
            * 获取网络连接的端口好
            */
            inline unsigned short GetPort(void){
                return this->m_port;
            }

            int isConnect(void);

        private:
            bool isopen(void);
            void addcounter(void);

        private:
            int                 m_socket;
            unsigned short      m_type;
            unsigned short      m_port;
            SocketAddress       m_socketAddress;

            static map<int,int> s_counter;
        };

        typedef Socket Network;
        typedef int    NetId;
    }

}
#endif
