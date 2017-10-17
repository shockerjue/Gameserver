### 框架的核心部分
    包含了引擎中共同部分：网络连接、数据库、缓存、共享内存、配置、线程。

#### 结构
* db 
* ddos
* engine
* include
* ipc
* lib
* msgdefine
* network
* release
* src
* test
* util

##### db
>包含了对mysql数据库和redis数据库的读写操作
```
MysqlUtil是操作mysql数据库的工具类
RedisUtil是操作redis数据库的工具类
```

##### ddos
>通过创建两个线程处理DDOS预防以及接收数据超时。
```
DDos 处理DDOS
Timeout 接受数据超时处理
```

##### engine
>引擎的处理，里面包含了网络管理、事件管理以及框架的入口。

##### include
>里面定义了缓存的管理结构、文件缓存结构、内存缓存结构、共享内存结构、线程结构、配置文件读取。

##### ipc
> 定义了服务器之间通信的协议结构，这里是通过protobuf自动生成的。

##### lib
>引擎生成以后的库目录

##### msgdefine
>服务器之间通信的协议号定义

##### network
>引擎网络对象、地址对象实现部分

##### src
>对include目录定义的结构实现

##### test 

##### util 
>定义了一些时间转换、字符与数字的转换、日志。
