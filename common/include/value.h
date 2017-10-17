/******************************************************************
* 代表了引擎中存储数据的工厂，其中转换不会报错，仅仅是返回默认值
* 
* author : shockerjue
********************************************************************/

#ifndef H_VALUE_H_
#define H_VALUE_H_

#ifdef _cplusplus
    extern "C"{
#endif
	#include <string.h>
#ifdef _cplusplus
    }
#endif

#include <map>
#include <string>

using namespace std;

namespace engine {
	
	class Value {
	public:
		Value(void);
		~Value(void);

		Value(const string);
		Value(const Value&);

		Value& operator=(const Value&);
		Value& operator=(const string);

		bool   operator==(const string);
		bool   operator==(const Value&);

		/**
		* 转换到int
		*/
		int 	toInt(void);

		/**
		* 转换到char类型的值，仅仅是获取value中的首字母
		*/
		char 	toChar(void);

		/**
		* 转换到bool值
		*/
		bool 	toBool(void);

		/**
		* 转换到float类型的值
		*/
		float 	toFloat(void);

		/**
		* 返回对象存储的值
		*/
		string 	toString(void);

		/**
		* 设置Value中存储的值
		*/
		void SetValue(string value);

		bool 	isEmpty(void);

	private:
		string 	m_value;	//	存储的数据
	};
}

#endif