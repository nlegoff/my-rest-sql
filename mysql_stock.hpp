#ifndef MYSQL_STOCK_HPP_INCLUDED
#define MYSQL_STOCK_HPP_INCLUDED

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/lexical_cast.hpp>
#include "mysql_connection.h"
#include "mysql_driver.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include "reply.hpp"
#include "IStock.hpp"

namespace my_rest
{

class mysql_stock : public IStock
{

public:
    mysql_stock(std::string path);

    void create_database(http::server::reply &rep, std::string db_name)
    {
        try
        {
            stmt->execute("CREATE DATABASE " + db_name);

            delete stmt;
            delete dbConn;

            rep = http::server::reply::stock_reply(http::server::reply::created);
            rep.content = db_name + " has been created";
            return;
        }
        catch (sql::SQLException e)
        {

            if (e.getErrorCode() == 1050)
            {
                rep = http::server::reply::stock_reply(http::server::reply::bad_request);
                rep.content = e.what();
                return;
            }
            else
            {
                rep = http::server::reply::stock_reply(http::server::reply::internal_server_error);
                rep.content = "something went wrong : " + boost::lexical_cast<std::string>(e.what());
                return;
            }
        }
    }

    void delete_database(http::server::reply &rep, std::string db_name)
    {
        try
        {
            stmt->execute("DROP DATABASE " + db_name);

            delete stmt;
            delete dbConn;

            rep = http::server::reply::stock_reply(http::server::reply::created);
            rep.content = db_name + " database has been removed";
            return;
        }
        catch (sql::SQLException e)
        {

            if (e.getErrorCode() == 1049)
            {
                rep = http::server::reply::stock_reply(http::server::reply::not_found);
                rep.content = e.what();
                return;
            }
            else
            {
                rep = http::server::reply::stock_reply(http::server::reply::internal_server_error);
                rep.content = "something went wrong : " + boost::lexical_cast<std::string>(e.what());
                return;
            }
        }
    }

    void create_table(http::server::reply &rep, std::string db_name, std::string table_name)
    {
        try
        {
            stmt->execute("USE " + db_name);
            stmt->execute("CREATE TABLE " + table_name + " (id MEDIUMINT NOT NULL AUTO_INCREMENT, value VARCHAR(100), PRIMARY KEY (id))");

            delete stmt;
            delete dbConn;

            rep = http::server::reply::stock_reply(http::server::reply::created);
            rep.content = table_name + " has been created";
            return;
        }
        catch (sql::SQLException e)
        {
            if (e.getErrorCode() == 1049)
            {
                rep = http::server::reply::stock_reply(http::server::reply::not_found);
                rep.content = e.what();
                return;
            }
            else if (e.getErrorCode() == 1050)
            {
                rep = http::server::reply::stock_reply(http::server::reply::bad_request);
                rep.content = e.what();
                return;
            }
            else
            {
                rep = http::server::reply::stock_reply(http::server::reply::internal_server_error);
                rep.content = "something went wrong : " + boost::lexical_cast<std::string>(e.what()) + " " + boost::lexical_cast<std::string>(e.getErrorCode());
                return;
            }
        }
    }

    void delete_table(http::server::reply &rep, std::string db_name, std::string table_name)
    {
        try
        {
            stmt->execute("USE " + db_name);
            stmt->execute("DROP TABLE " + table_name);

            delete stmt;
            delete dbConn;

            rep = http::server::reply::stock_reply(http::server::reply::created);
            rep.content = table_name + " has been removed";
            return;
        }
        catch (sql::SQLException e)
        {
            if (e.getErrorCode() == 1049)
            {
                rep = http::server::reply::stock_reply(http::server::reply::not_found);
                rep.content = e.what();
                return;
            }
            else
            {
                rep = http::server::reply::stock_reply(http::server::reply::internal_server_error);
                rep.content = "something went wrong : " + boost::lexical_cast<std::string>(e.what());
                return;
            }
        }
    }

    void create_row(http::server::reply &rep, std::string db_name, std::string table_name, std::string value)
    {
        try
        {
            stmt->execute("USE " + db_name);
            stmt->execute("INSERT INTO " + table_name + " (value) VALUES ('" + value + "')");

            delete stmt;
            delete dbConn;

            rep = http::server::reply::stock_reply(http::server::reply::created);
            rep.content = "row has been created";
            return;
        }
        catch (sql::SQLException e)
        {
            if (e.getErrorCode() == 1049)
            {
                rep = http::server::reply::stock_reply(http::server::reply::not_found);
                rep.content = e.what();
                return;
            }
            else
            {
                rep = http::server::reply::stock_reply(http::server::reply::internal_server_error);
                rep.content = "something went wrong : " + boost::lexical_cast<std::string>(e.what());
                return;
            }
        }
    }

    void delete_row(http::server::reply &rep, std::string db_name, std::string table_name, int id)
    {
        try
        {
            stmt->execute("USE " + db_name);
            stmt->execute("DELETE FROM " + table_name + " WHERE id = "+ boost::lexical_cast<std::string>(id));

            delete stmt;
            delete dbConn;

            rep = http::server::reply::stock_reply(http::server::reply::created);
            rep.content = "row has been deleted";
            return;
        }
        catch (sql::SQLException e)
        {
            if (e.getErrorCode() == 1049)
            {
                rep = http::server::reply::stock_reply(http::server::reply::not_found);
                rep.content = e.what();
                return;
            }
            else if (e.getErrorCode() == 1050)
            {
                rep = http::server::reply::stock_reply(http::server::reply::bad_request);
                rep.content = e.what();
                return;
            }
            else
            {
                rep = http::server::reply::stock_reply(http::server::reply::internal_server_error);
                rep.content = "something went wrong : " + boost::lexical_cast<std::string>(e.what());
                return;
            }
        }
    }

    void list_databases(http::server::reply &rep)
    {
        try
        {
            sql::ResultSet  *res;

            res = stmt->executeQuery("show databases");

            rep = http::server::reply::stock_reply(http::server::reply::ok);
            while (res->next())
            {
                rep.content += res->getString(1) + "\n";
            }

            delete res;
            delete stmt;
            delete dbConn;
        }
        catch (sql::SQLException e)
        {
            rep = http::server::reply::stock_reply(http::server::reply::internal_server_error);
            rep.content = "something went wrong : " + boost::lexical_cast<std::string>(e.what());
            return;
        }
    }

    void list_tables(http::server::reply &rep, std::string db_name)
    {
        try
        {
            sql::ResultSet  *res;

            stmt->execute("USE " + db_name );

            res = stmt->executeQuery("show tables");

            rep = http::server::reply::stock_reply(http::server::reply::ok);
            while (res->next())
            {
                rep.content += res->getString(1) + "\n";
            }

            delete res;
            delete stmt;
            delete dbConn;
        }
        catch (sql::SQLException e)
        {
            if (e.getErrorCode() == 1049)
            {
                rep = http::server::reply::stock_reply(http::server::reply::not_found);
                rep.content = e.what();
                return;
            }
            else
            {
                rep = http::server::reply::stock_reply(http::server::reply::internal_server_error);
                rep.content = "something went wrong : " + boost::lexical_cast<std::string>(e.what()) + " " + boost::lexical_cast<std::string>(e.getErrorCode());
                return;
            }
        }
    }

    void list_rows(http::server::reply &rep, std::string db_name, std::string table_name)
    {
        try
        {
            sql::ResultSet  *res;

            stmt->execute("USE " + db_name);
            res = stmt->executeQuery("SELECT id, value FROM " + table_name);

            rep = http::server::reply::stock_reply(http::server::reply::ok);
            while (res->next())
            {
                rep.content += res->getString(1) + " " + res->getString(2) + "\n";
            }

            delete res;
            delete stmt;
            delete dbConn;
        }
        catch (sql::SQLException e)
        {
            if (e.getErrorCode() == 1049 || e.getErrorCode() == 1146)
            {
                rep = http::server::reply::stock_reply(http::server::reply::not_found);
                rep.content = e.what();
                return;
            }
            else
            {
                rep = http::server::reply::stock_reply(http::server::reply::internal_server_error);
                rep.content = "something went wrong : " + boost::lexical_cast<std::string>(e.what()) + " " + boost::lexical_cast<std::string>(e.getErrorCode());
                return;
            }
        }
    }

    void list_row(http::server::reply &rep, std::string db_name, std::string table_name, int id)
    {
        try
        {
            sql::ResultSet  *res;

            stmt->execute("USE " + db_name);
            res = stmt->executeQuery("SELECT value FROM " + table_name + " WHERE id = " + boost::lexical_cast<std::string>(id));

            bool found = false;

            rep = http::server::reply::stock_reply(http::server::reply::ok);
            while (res->next())
            {
                rep.content += res->getString(1) + "\n";
                found = true;
            }

            if (! found)
            {
                rep = http::server::reply::stock_reply(http::server::reply::not_found);
                rep.content = "row could not be found";
                return;
            }

            delete res;
            delete stmt;
            delete dbConn;
        }
        catch (sql::SQLException e)
        {
            if (e.getErrorCode() == 1049 || e.getErrorCode() == 1146)
            {
                rep = http::server::reply::stock_reply(http::server::reply::not_found);
                rep.content = e.what();
                return;
            }
            else
            {
                rep = http::server::reply::stock_reply(http::server::reply::internal_server_error);
                rep.content = "something went wrong : " + boost::lexical_cast<std::string>(e.what()) + " " + boost::lexical_cast<std::string>(e.getErrorCode());
                return;
            }
        }
    }

private:
    sql::Driver     *driver;
    sql::Connection *dbConn;
    sql::Statement  *stmt;
    std::string root_path;
};
}

#endif
