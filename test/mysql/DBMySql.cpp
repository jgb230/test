#include "DBMySql.h"


DBMySql::DBMySql()
{
    m_mysql = NULL;
}

DBMySql::DBMySql(const std::string& host, const std::string& user, const std::string& passwd, const std::string& db, short port)
{
    m_host = host;
    m_user = user;
    m_passwd = passwd;
    m_db = db;
    m_port = port;

    m_mysql = NULL;

    initialize();
}

DBMySql::~DBMySql()
{
	mysql_close(m_mysql);
    m_mysql = NULL;
}

int DBMySql::initialize()
{
    m_mysql = mysql_init(NULL);

    if(m_mysql == NULL)
    {
        std::cout<< "mysql_init Failed, Error["<< mysql_error(m_mysql)<<"]";
        return -1;
    }

	if (mysql_set_character_set(m_mysql, "utf8"))
	{
        std::cout<< "mysql_set_character_set Failed, Error["<< mysql_error(m_mysql)<<"]";
	}

    return connect();
}

int DBMySql::initialize(const std::string& host, const std::string& user, const std::string& passwd, const std::string& db, short port)
{
    m_host = host;
    m_user = user;
    m_passwd = passwd;
    m_db = db;
    m_port = port;

    m_mysql = mysql_init(NULL);

    if(m_mysql == NULL)
    {
        std::cout<< "mysql_init Failed, Error["<< mysql_error(m_mysql)<<"]";
        return -1;
    }
    
    if (mysql_set_character_set(m_mysql, "utf8"))
    {
        std::cout<< "mysql_set_character_set Failed, Error["<< mysql_error(m_mysql)<<"]";
    }

    return connect();
}

int DBMySql::connect()
{
    std::cout<< "Begin Connect: host="<<m_host<<", port="<<m_port<<", user="<<m_user<<", passwd="<<m_passwd<<", db="
    		<<m_db;
    if (mysql_real_connect(m_mysql,
                           m_host.c_str(),
                           m_user.c_str(),
                           m_passwd.c_str(),
                           m_db.c_str(),
                           m_port, NULL, 0) == NULL)
    {
        std::cout<< "mysql_real_connect Failed, Error["<< mysql_error(m_mysql)<<"]";
        return -1;
	}
    std::cout<< "MySql Connect Success";
    return 0;
}

void DBMySql::destroy()
{
	mysql_close(m_mysql);
    m_mysql = NULL;
    return;
}

std::string DBMySql::error_msg(const std::string& func)
{
    std::ostringstream os;
    os << func;
    os << " errno = ";
    os << mysql_errno(m_mysql);
    os << ", error = ";
    os << mysql_error(m_mysql);

    std::string str(os.str());

    return str;
}

MYSQL_RES* DBMySql::Query(const std::string& str)
{
    std::cout<< "Query str["<< str<<"]";
    int ret = execute(str);

    if(ret == -1)
    {
        return NULL;
    }

    return mysql_store_result(m_mysql);
}

int DBMySql::Insert(const std::string& str)
{
    std::cout<< "Insert str["<< str<<"]";
    return execute(str);
}

int DBMySql::Update(const std::string& str)
{   
    std::cout<< "Update str["<< str<<"]";
    return execute(str);
}

int DBMySql::execute(const std::string& str)
{
    if(m_mysql == NULL)
    {
        std::cout<< "m_mysql == NULL, Init Again";
        initialize();

        if(m_mysql == NULL)
        {
            std::cout<< "m_mysql == NULL, Return -1";
            return -1;
        }
    }

    std::cout<< "Execute sql["<< str<<"]";

    int ret = mysql_query(m_mysql, str.c_str());

    if(ret != 0)
    {
        std::cout<< "Error["<< error_msg(__func__)<<"]";

        int err = get_errno();

        if(err == 2006 || err == 2013) // MySQL server has gone away
        {
            ret = connect();
        
            if(ret != 0)
            {
                std::cout<< "Error["<<error_msg(__func__)<<"], Return Ret["<< ret<<"]";
                return ret;
            }

            ret = mysql_query(m_mysql, str.c_str());

            if(ret != 0)
            {
                std::cout<< "Error["<<error_msg(__func__)<<"], Return Ret["<< ret<<"]";
            }
        }
	}

    return ret;
}

int DBMySql::get_errno()
{
    return mysql_errno(m_mysql);
}

int DBMySql::num_fields(MYSQL_RES* res)
{
    int num = mysql_num_fields(res);
    
    std::cout<< "Query Num["<<num<<"]";
    return num; 
}

int DBMySql::affected_rows()
{
    int num = mysql_affected_rows(m_mysql);

    std::cout<< "Affected Num["<<num<<"]";
    return num;
}

std::string DBMySql::get_error()
{
    return mysql_error(m_mysql);
}
