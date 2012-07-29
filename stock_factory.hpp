#ifndef STOCK_FACTORY_HPP_INCLUDED
#define STOCK_FACTORY_HPP_INCLUDED

#include <string>
#include "IStock.hpp"

namespace my_rest
{

class stock_factory
{

public:
    static IStock *createInstance(std::string type, std::string root_path);
};
}

#endif
