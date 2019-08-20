#ifndef __DBMYSQL_H__
#define __DBMYSQL_H__

#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
#include <sstream>   

#include <mysql/mysql.h>

class DBMySql
{
public:
    DBMySql();
	DBMySql(const std::string& host, const std::string& user, const std::string& passwd, const std::string& db, short port = 3306);
	~DBMySql();

	int initialize();
    int initialize(const std::string& host, const std::string& user, const std::string& passwd, const std::string& db, short port = 3306);

	//Ping
	void ping() { mysql_ping(m_mysql); }

public:
	int Insert(const std::string& str);
    MYSQL_RES* Query(const std::string& str);
    int Update(const std::string& str);

public:
    int execute(const std::string& str);
    int connect();
    void destroy();
    
    std::string error_msg(const std::string& func);
    int get_errno();
    std::string get_error();

    MYSQL_RES* query(const std::string& str);
    
    int num_fields(MYSQL_RES* res);
    int affected_rows();
        
private:
    std::string m_host;
    std::string m_user;
    std::string m_passwd;
    std::string m_db;    
    short 		m_port;


private:
	MYSQL* m_mysql;

};

#endif // __DBMYSQL_H__
