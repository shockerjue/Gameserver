#include "ramcache_test.h"

void write_cache(RamCache* ramcache){
	if(nullptr == ramcache){
		return ;
	}

	int size = 8;

	char data[] = "RamCache";
	ramcache->addCache("key",data,size);

	Log::Print(__func__,"RamCache");
}

void read_cache(RamCache* ramcache){
	if(nullptr == ramcache){
		return ;
	}

	char data[128] = {0};
	int ret = ramcache->getCache("key",data);

	Log::Print(__func__, data);
}

void ramcache_test(void){
	RamCache ramcache;

	write_cache(&ramcache);
	read_cache(&ramcache);
}