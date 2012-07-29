#include "mysql_connection.h"
#include "mysql_driver.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include "mysql_stock.hpp"
#include "config.hpp"

namespace my_rest
{

mysql_stock::mysql_stock(std::string path) : root_path(path)
{
    try
    {

        driver = sql::mysql::get_driver_instance();
    }
    catch (sql::SQLException e)
    {

    }

    try
    {
        dbConn = driver->connect(MYSQL_SERVER_ADRESS, MYSQL_SERVER_USER, MYSQL_SERVER_PASS);
    }
    catch (sql::SQLException e)
    {

    }

    stmt = dbConn->createStatement();

}
}
