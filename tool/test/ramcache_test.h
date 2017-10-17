#ifndef H_RAMCACHE_TEST_H_
#define H_RAMCACHE_TEST_H_

#include "./../../common/util/util.h"
#include "./../../common/include/ramcache.h"

using namespace util;
using namespace engine::cache;

void write_cache(RamCache* ramcache);
void read_cache(RamCache* ramcache);
void ramcache_test(void);

#endif