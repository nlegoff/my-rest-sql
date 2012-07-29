#include <string>
#include "stock_factory.hpp"
#include "filesystem_stock.hpp"
#include "mysql_stock.hpp"
#include "IStock.hpp"

namespace my_rest
{

IStock* stock_factory::createInstance (std::string type, std::string root_path)
{
    if (type == "mysql") return new mysql_stock(root_path);
    else return new filesystem_stock(root_path);
}
}
