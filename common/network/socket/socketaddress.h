/**********************************************************************
* 网络连接地址对象
* Dev : shockerjue
************************************************************************/

#ifndef H_SOCKETADDRESS_H_
#define H_SOCKETADDRESS_H_

#include <iostream>

#ifdef _cplusplus
    extern "C"{
#endif
    #include <stdio.h>
    #include <string>
    #include <netdb.h>
    #include <arpa/inet.h>
#ifdef _cplusplus
    }
#endif

using namespace std;

namespace engine {

    namespace network {
        
        class SocketAddress {
        public:
            SocketAddress();

            /**
            * 使用主机ip设置SocketAddress
            * @param    ip      主机ip
            * @param    m_len   地址额长度[4:IPv4  6:IPv6]
            */
            SocketAddress(const char* ip, int m_len);

            /**
            * 使用主机地址初始化SocketAddress
            * @param    address     主机地址
            */
            SocketAddress(const char* address);

            SocketAddress(const SocketAddress& socketAddress);

            ~SocketAddress();

            /**
            * 获取本地地址的socket
            */
            SocketAddress   getLocalSocketAddress();
            
            SocketAddress &operator=(const SocketAddress& socketAddress);

            /**
            * 获取主机名
            */
            string          getHostName();
            /**
            * 获取主机IP地址
            */
            string          getHostAddress();
        private:
            string      m_ip;
            string      m_address;
            string      m_name;
        };

        typedef SocketAddress NetworkAddress;
    }

}
#endif
