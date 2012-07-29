#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include "filesystem_stock.hpp"


namespace my_rest
{

filesystem_stock::filesystem_stock(std::string path) : root_path(path)
{
}
}
