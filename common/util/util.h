#ifndef H_UTIL_H_
#define H_UTIL_H_

#ifdef _cplusplus
    extern "C"{
#endif
	#include <time.h>
	#include <stdlib.h>
	#include <unistd.h>
#ifdef _cplusplus
    }
#endif

#include <string>
#include <iostream>
#include "log.h"
#include "datetime.h"

using namespace std;

namespace util {

	/**
	* 生成秘钥的字符池
	*/
	const char temp_key[] = {
			'A','B','C','D','E','F','G','H','I',
			'J','K','M','N','O','P','Q','R','S',
			'T','U','V','W','X','Y','Z','L','1',
			'2','3','4','5','6','7','8','9','0',
			'@','&'
		};

	class Util{
	public:
		/**
		* 生成登录秘钥key
		*/
		static string genkey(void);

		/**
		* string数字字符串转成数字
		*
		* @param str
		*/
		static int str2int(const string str);

		/**
		* int转换成string字符串
		*
		* @param  value
		*/ 
		static string int2str(const int value);


		/**
		* 数字字符串转换成数字
		*
		* @param chrs
		*/
		static int chars2int(const char* chrs);
	};
}

#endif