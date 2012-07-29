#ifndef CONFIG_HPP_INCLUDED
#define CONFIG_HPP_INCLUDED


///Define backend stockage system
//#define STOCKAGE "mysql"
#define STOCKAGE "filesystem"

///Define http server properties
#define HTTP_SERVER_ADRESS "127.0.0.1"
#define HTTP_SERVER_PORT "8787"
#define HTTP_SERVER_DOCUMENT_ROOT "/home/nicolas/workspace/my-rest-sql/tests/server1/document_root"

///Define mysql server properties
#define MYSQL_SERVER_ADRESS "localhost"
#define MYSQL_SERVER_USER "root"
#define MYSQL_SERVER_PASS "Nicolas78"

#endif // CONFIG_HPP_INCLUDED
