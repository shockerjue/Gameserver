### redis
https://github.com/redis/hiredis
>使用redis需要编译生成的可(.so,.a)复制到lib下对应的平台目录.

项目编译的时候，还要家上-lhiredis编译链接。-L是用于添加搜索路径

```
	$(CXX) -o1 -o main $(objs) $(CXXFLAGS) $(LDFLAGS) -lmysqlclient -L $(TDARTY_LIB) -lhiredis
```

### protobuf
https://github.com/google/protobuf
>使用protobuf需要编译生成的库文件(.so,.a)复制到protobuf/lib目录下复制到对应lib目录下对应的平台目录.

>项目编译的时候，还要链接protobuf静态库
```
$(CXX) -o1 -o main $(objs) $(CXXFLAGS) $(LDFLAGS)  -L $(LIB) -lprotobuf
```

对于OS X系统，同样的需要将对应的库拷贝到相应的库目录中，OS X中库文件是以.dylib结尾.

```
$(TDARTY_LIB) = $(ROOT)/3tdarty/lib/linux
$(LIB) = $(ROOT)/3tdarty/protobuf/lib/linux
```
