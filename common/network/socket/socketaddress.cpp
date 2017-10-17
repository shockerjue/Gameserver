#include "socketaddress.h"

namespace engine {

    namespace network {
        
        SocketAddress::SocketAddress(){
            this->m_ip = "127.0.0.1";
            this->m_address = "127.0.0.1";
            this->m_name = "localhost";
        }

        SocketAddress::SocketAddress(const SocketAddress& socketAddress){
            this->m_ip = socketAddress.m_ip;
            this->m_address = socketAddress.m_address;
            this->m_name = socketAddress.m_name;

            return ;
        }

        SocketAddress::SocketAddress(const char* ip,int m_len){
            if(nullptr == ip){
                return ;
            }

            this->m_ip = string(ip);
            this->m_address = string(ip);

            struct in_addr addr;
            struct hostent *host;

            inet_pton(AF_INET, ip, &addr);
            host = gethostbyaddr((const char*)&addr, sizeof(addr), AF_INET);
            if (host == NULL) {
                return ;
            }

            this->m_name = string(host->h_name);
        }

        SocketAddress::SocketAddress(const char* address){
            if(nullptr == address){
                return ;
            }

            struct hostent* host = gethostbyname(address);

            this->m_name = string(host->h_name);
            this->m_ip = string(inet_ntoa(*((struct in_addr*)host->h_addr_list[0])));
            this->m_address = string(inet_ntoa(*((struct in_addr*)host->h_addr_list[0])));
        }

        SocketAddress SocketAddress::getLocalSocketAddress(){
            SocketAddress socketAddress;

            return socketAddress;
        }

        SocketAddress& SocketAddress::operator=(const SocketAddress& socketAddress){
            if(this == &socketAddress){
                return *this;
            }

            this->m_ip = socketAddress.m_ip;
            this->m_address = socketAddress.m_address;
            this->m_name = socketAddress.m_name;

            return *this;
        }

        string SocketAddress::getHostName(){
            return this->m_name;
        }

        string SocketAddress::getHostAddress(){
            return this->m_ip;
        }

        SocketAddress::~SocketAddress(){

        }
    }

}