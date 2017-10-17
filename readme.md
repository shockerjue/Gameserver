## 服务器框架

![structrue](/doc/diagram/structrue.png)

#### Port
1.  gateway		gateway server			[Gateway]		8989
2.  login		login server			[Login]			8900
3.  dataaccess	data access server		[DB]			8093
4.  dataaccess	data access server 		[DB]			8090
5.  gameworld	gameworld server		[Gameworld]		8091
6.  chatserver	chat server				[Chatserver]	8094
7.  gameserver	game server				[IO]			8092
8.  gameserver 	game server1 			[IO1]			9000
9.  gameserver 	game server2 			[IO2]			9001
10. logserver	log  server 			[LogServer]		9002

#### 目录
* Game
    * 3tdarty [第三方库]
        * hiredis [redis库]
        * json [-json库]
        * lib
        * protobuf [protobuf库]
    * chatserver [聊天服务]
    * common [引擎框架目录]
    * dataaccess [数据访问服务]
    * doc
    * gameserver [游戏服务器]
    * gameworld [游戏世界服务器]
    * gateway [网关服务器]
    * login [登录服务器]
    * logserver [日志服务器]
    * proto [协议定义与导出目录]
        * export 
        * message
    * tool
        * protobuf [protobuf协议导出攻击]
        * test [游戏关卡测试程序]	

#### 服务器功能介绍
* dataaccessi:游戏数据服务器，所有的对数据操作都是通过这个服务器操作，其中dataaccess1是用于用户的基本操作，dataaccess2是用于gameserver和chatserver数据回写。
* chatserver:游戏中的通信服务器，所有的通信都经过这个服务器，不管是玩家对玩家、还是工会、或者是世界公告都是经过这个服务器
* gameworld:具体的游戏逻辑服务器，里面承载了游戏世界，这个服务器可以创建多个，根据不同的功能或者游戏世界划分，如可以一个副本一个服。
* login:登录服务器，这里主要是验证登录玩家是否存在，同时生成对应的登录秘钥。同时找出负载最小的服务器信息返回给玩家，并将登录信息通知给对应的gameserver。
* gateway:一个网关服务，主要是对平台进行验证，并将登录信息返回给玩家。
* gameserver:游戏服务器，所有玩家的操作均会通过这个服务器。
* logserver:日志处理服务，接受其他服务器发送过来的日志数据，然后进行处理。

登录gameserver仅仅是需要从gateway服务器获取的key来登录。


common目录下面是引擎源码，里面包含了网络管理（socket）、事件管理、IPC管理(socket)、缓冲管理（内存/缓村）、数据库管理（mysql/redis）、共享内存管理等。
各个服务之间的通信使用protobuf实现。

#### Complier
 ```shell
 #> make 
 ```


#### Install 
```shell
 make install
```

#### Run
```shell
#> sh start.sh 
```

#### Stop
```shell
#> sh stop.sh
```

#### Develop
##### 1 定义协议
>>在/proto/message定义好需要使用的协议，之后使用/tool/protobuf导出协议并拷贝到对应的目录下。

##### 2 定义协议号
>> 在对应的协议号目录下定义对应的协议号。服务器之间的协议号定义在/common/msgdefine/msgcode.h中定义。
>> 与用户通信[Gateway和Gameserver服务]的协议号定义在对应服务器结构下的msghandle/msgcode.h中定义。

##### 3 实现对应协议的处理方法
>> 在对应的服务中实现对应协议的处理方法

##### 4 注册协议处理方法
>> 在需要注册处理方法的服务器下的eventhandle/event_manage中注册对应协议号的处理方法。
```
RegistMsgHandle::registerMsgHandle
```

##### 5 编译和启动服务器
>> 编译和启动服务器，启动的时候需要传递配置文件。

##### 6 如果需要添加服务器
>>当现有服务器类型不够时，需要添加服务器，需要先在/common/include/type.h的ConnectType中添加服务器类型，因为在连接服务器时需要设置类型，同时当和这个服务器通信时，需要使用该类型进行通信。
```
...->ConnectHostBySign(host_ip,host_port,ConnectType::当前服务器类型,ConnectType::连接的服务器类型);
```



#### Test
>> 在/test目录实现了部分测试；线程、共享内存、缓存测试以及服务器的登录测试。
* 服务器登录测试
```shell 
    ./bin/main server_ip server_port username
```
里面包含了游戏登录的过程、添加好友、聊天通信