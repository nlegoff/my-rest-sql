#include "request_handler.hpp"
#include <fstream>
#include <typeinfo>
#include <sstream>
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "mime_types.hpp"
#include "reply.hpp"
#include "request.hpp"
#include "stock_factory.hpp"
#include "config.hpp"

//link libraries
/// -lboost_filesystem
/// -lboost_system
/// -I/usr/local/include
/// -I/usr/local/include/cppconn
/// -lmysqlcppconn
//search directories linker
/// /usr/local/lib

namespace http
{
namespace server
{

request_handler::request_handler(const std::string& doc_root)
    : doc_root_(doc_root)
{
}

void request_handler::handle_request(const request& req, reply& rep)
{
    std::string request_path;
    std::vector<std::string> strs;

    if (!url_decode(req.uri, request_path))
    {
        rep = reply::stock_reply(reply::bad_request);
        return;
    }

    // Request path must be absolute and not contain "..".
    if (request_path.empty() || request_path[0] != '/' || request_path.find("..") != std::string::npos)
    {
        rep = reply::stock_reply(reply::bad_request);
        return;
    }

    boost::split(strs, req.uri, boost::is_any_of("/"));

    my_rest::IStock *stock_db  = NULL;

    stock_db = my_rest::stock_factory::createInstance(STOCKAGE, doc_root_);

    if(NULL == stock_db)
    {
        rep = http::server::reply::stock_reply(http::server::reply::internal_server_error);
        return;
    }

    switch (strs.size())
    {
    case 2 :
        if (strs[1].empty())
        {
            if (req.method == "GET")
            {
                stock_db->list_databases(rep);
            }
            else
            {
                rep = reply::stock_reply(reply::not_implemented);
            }
        }
        else
        {
            if (req.method == "POST")
            {
                stock_db->create_database(rep, strs[1]);
            }
            else if (req.method == "GET")
            {
                stock_db->list_tables(rep, strs[1]);
            }
            else if (req.method == "DELETE")
            {
                stock_db->delete_database(rep, strs[1]);
            }
            else
            {
                rep = reply::stock_reply(reply::not_implemented);
            }
        }
        break;

    case 3 :
        if (strs[2].empty())
        {
            if (req.method == "POST")
            {
                stock_db->create_database(rep, strs[1]);
            }
            else if (req.method == "GET")
            {
                stock_db->list_tables(rep, strs[1]);
            }
            else if (req.method == "DELETE")
            {
                stock_db->delete_database(rep, strs[1]);
            }
            else
            {
                rep = reply::stock_reply(reply::not_implemented);
                return;
            }
        }
        else
        {
            if (req.method == "POST")
            {
                stock_db->create_table(rep, strs[1], strs[2]);
            }
            else if (req.method == "GET")
            {
                stock_db->list_rows(rep, strs[1], strs[2]);
            }
            else if (req.method == "DELETE")
            {
                stock_db->delete_table(rep, strs[1], strs[2]);
            }
            else
            {
                rep = reply::stock_reply(reply::not_implemented);
            }
        }
        break;

    case 4 :
        if (strs[3].empty())
        {
            if (req.method == "POST")
            {
                stock_db->create_table(rep, strs[1], strs[2]);
            }
            else if (req.method == "GET")
            {
                stock_db->list_rows(rep, strs[1], strs[2]);
            }
            else if (req.method == "DELETE")
            {
                stock_db->delete_table(rep, strs[1], strs[2]);
            }
            else
            {
                rep = reply::stock_reply(reply::not_implemented);
            }
        }
        else
        {
            if (req.method == "POST")
            {
                rep.content = strs[3];
                std::vector<std::string> uri;
                boost::split(uri, strs[3], boost::is_any_of("?"));

                if(2 == boost::lexical_cast<int>(uri.size()))
                {
                    std::vector<std::string> params_str;
                    boost::split(params_str, uri[1], boost::is_any_of("&"));
                    rep.content += " " + uri[1];
                    for (std::size_t i = 0; i <  uri.size(); ++i)
                    {
                        std::vector<std::string> params;
                        boost::split(params, uri[1], boost::is_any_of("="));
                        stock_db->create_row(rep, strs[1], strs[2], params[1]);
                        break;
                    }
                }
            }
            else if (req.method == "GET")
            {
                stock_db->list_row(rep, strs[1], strs[2], boost::lexical_cast<int>(strs[3]));
            }
            else if (req.method == "DELETE")
            {
                stock_db->delete_row(rep, strs[1], strs[2], boost::lexical_cast<int>(strs[3]));
            }
            else
            {
                rep = reply::stock_reply(reply::not_implemented);
            }
        }
        break;

    default :
        rep = reply::stock_reply(reply::bad_request);
    }

    rep.headers.resize(2);
    rep.headers[0].name = "Content-Length";
    rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
    rep.headers[1].name = "Content-Type";
    rep.headers[1].value = mime_types::extension_to_type("text/html");
}

bool request_handler::url_decode(const std::string& in, std::string& out)
{
    out.clear();
    out.reserve(in.size());
    for (std::size_t i = 0; i < in.size(); ++i)
    {
        if (in[i] == '%')
        {
            if (i + 3 <= in.size())
            {
                int value = 0;
                std::istringstream is(in.substr(i + 1, 2));
                if (is >> std::hex >> value)
                {
                    out += static_cast<char>(value);
                    i += 2;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else if (in[i] == '+')
        {
            out += ' ';
        }
        else
        {
            out += in[i];
        }
    }
    return true;
}

} // namespace server
} // namespace http
