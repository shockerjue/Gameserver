#ifndef H_SHARERAM_TEST_H_
#define H_SHARERAM_TEST_H_

#include "./../../common/util/util.h"
#include "./../../common/include/shareram.h"

using namespace util;
using namespace engine::shareram;

struct Share {
	int rid;
};

void onSave(ShareRam* share);
void onLoad(ShareRam* share);
void shareram_test(void);

#endif