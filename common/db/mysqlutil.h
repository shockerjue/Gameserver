/**************************************************************
*	mysql数据库操作工具，主要是进行数据的查询与插入或者更新
*
*	author : shockerjue
****************************************************************/
#ifndef H_MYSQLUTIL_H_
#define H_MYSQLUTIL_H_

#ifdef _cplusplus
    extern "C"{
#endif
#ifdef __linux
	#include <mysql/mysql.h> 

#elif __APPLE__
	#include <mysql.h>

#endif
#ifdef _cplusplus
    }
#endif

#include "dbmodule.h"
#include "./../include/value.h"

namespace engine {

	namespace db {
		
		/**
		* 不使用单例的原因是，一个服务可以连接多个数据库
		*/
		class MysqlUtil : public DBModule {
		public:
			MysqlUtil(void);
			~MysqlUtil(void);

			/**
			* 连接到mysql服务器
			*
			* @return bool
			*/
			virtual bool InitConnection(void);

			/**
			* 通过数据库信息连接到数据库
			*
			* @param host		数据库服务地址
			* @param port		数据库服务端口
			* @param dbname		数据库名字
			* @param user		数据库用户
			* @param pass 		数据库密码
			* @return bool 		true:连接成功
			*/
			virtual bool InitConnection(const string,const int,
				const string,const string,const string );

			/**
			* 执行数据库SQL语句，用于更新或者是执行插入操作
			*
			* @param sql
			* @return int
			*/
			int 	Exec(const string);

			/**
			* 从数据库中查询数据
			* 
			* @param sql
			* return vector<map<string,Value> > 
			*/
			vector<map<string,Value> >	Selection(const string);

		private:
			MysqlUtil(const MysqlUtil&){}
			MysqlUtil& operator=(const MysqlUtil&){ return *this; }

		private:
			MYSQL 			m_mysql;
		};
	}

}

#endif