#include <iostream>
#include <map>

#ifdef _cplusplus
    extern "C"{
#endif
    #include <string.h>
    #include <unistd.h>
    #include <stdlib.h>
#ifdef _cplusplus
    }
#endif

#include "network.h"
#include "./../util/log.h"

using namespace std;
using namespace engine::network;
using namespace util;

Network test_network(void){
    Network network(Protocol::TCP);

    map<int,Network> ts;
    ts[1] = network;
    ts[2] = network;
    ts[3] = network;
    ts[4] = network;

    return network;
}


int main(int argc,const char* argv[]){
    Log::Print(__func__,"log test");
    char address[] = "127.0.0.1";

    NetworkAddress addres(address,4);
    NetworkAddress networkAddres = addres;
    string str = networkAddres.getHostAddress();
    cout<<networkAddres.getHostName()<<endl;
    cout<<networkAddres.getHostAddress()<<endl;

    int op = 1;
   
    Network acce;

    Network network(test_network());
    network.SetOpt(SOL_SOCKET,SO_REUSEADDR,&op,sizeof(op));
    network.Bind(networkAddres,8999);
    network.Listen(5);
    network.Accept(&acce);
    char buf[] = "shockerjue\n";
    acce.Send(buf,strlen(buf));

    return  0;
}
