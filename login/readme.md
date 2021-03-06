### 登录服务器
>这个服务器主要是进行玩家信息验证，对玩家信息进行异步认证。将验证结果异步的返回给gateway服务器。

#### 流程
1. 接收请求.接收来自gateway的验证请求[就是玩家的登录请求]。
2. 发送请求到DB服务器 .发送请求到DB，检查玩家是否是有效的玩家。
3. 接收DB的响应.如果玩家信息是有效的，则会收到DB的验证返回。
4. 通知gameserver服务器.如果玩家信息是验证通过的，则通知gameserver服务器有玩家即将登陆。通知的内容是{rid,key}
5. 将信息返回gateway服务器.这里是将登录的验证信息返回给gateway，这里面如果玩家信息验证通过，里面包含了gameserve的登录信息。
6. 接收来自gameserver的信息.这里是保证gameserver平均分配的一个点。当gameserveri有玩家登录以后，会主动的通知login服务器，以便login知道gameserver中连接的信息。方便当有新玩家的时候分配资源最多的服务器。

* 优化
    因为玩家客户端会出现一些不定的错误，这是客户端会在次请求登录验证。因为在请求验证的时候会去读取数据库和动态的生成key，
    为了当玩家频繁登录时减少数据库的读写，这里将登录验证通过后的信息缓存起来，当再次登录时只需要读取缓存数据响应玩家即可。

>该服务器主要功能是借助DB验证玩家真实性以及获取登录秘钥key。其中使用ramcache来缓存验证通过后的信息。

![login](/doc/diagram/login.png)
