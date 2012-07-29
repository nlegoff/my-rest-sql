#ifndef ISTOCK_HPP_INCLUDED
#define ISTOCK_HPP_INCLUDED

#include <string>
#include "reply.hpp"

using std::string;

namespace my_rest
{
class IStock
{
public:

    virtual void list_databases(http::server::reply &rep) = 0;
    virtual void list_tables(http::server::reply &rep, string db_name) = 0;
    virtual void list_rows(http::server::reply &rep, string db_name, string table_name) = 0;
    virtual void list_row(http::server::reply &rep, string db_name, string table_name, int id) = 0;
    virtual void create_database(http::server::reply &rep, string db_name) = 0;
    virtual void create_table(http::server::reply &rep, string db_name, string table_name) = 0;
    virtual void create_row(http::server::reply &rep, string db_name, string table_name, string value) = 0;
    virtual void delete_database(http::server::reply &rep, string db_name) = 0;
    virtual void delete_table(http::server::reply &rep, string db_name, string table_name) = 0;
    virtual void delete_row(http::server::reply &rep, string db_name, string table_name, int id) = 0;
};
}
#endif
