#ifndef FLAT_FILE_HPP_INCLUDED
#define FLAT_FILE_HPP_INCLUDED

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

class filesystem_stock : public IStock
{

public:
    filesystem_stock(std::string path);

    void create_database(http::server::reply &rep, std::string db_name)
    {
        boost::filesystem::path base_path(root_path);
        boost::filesystem::path db_path(db_name);

        boost::filesystem::path dir(base_path/db_path);

        if (boost::filesystem::exists(dir))
        {
            rep = http::server::reply::stock_reply(http::server::reply::bad_request);
            rep.content = db_name + " database already exists";
            return;
        }

        if (boost::filesystem::create_directory(dir))
        {
            rep = http::server::reply::stock_reply(http::server::reply::created);
            rep.content = db_name + " database has been created";
            return;
        }

        rep = http::server::reply::stock_reply(http::server::reply::internal_server_error);
        rep.content = db_name + " could not be created for obscur reasons";
    }

    void delete_database(http::server::reply &rep, std::string db_name)
    {
        boost::filesystem::path base_path(root_path);
        boost::filesystem::path db_path(db_name);

        boost::filesystem::path dir(base_path/db_path);

        if ( ! boost::filesystem::exists(dir))
        {
            rep = http::server::reply::stock_reply(http::server::reply::not_found);
            rep.content = db_name + " database could not be found";
            return;
        }

        if (boost::filesystem::remove_all(dir))
        {
            rep = http::server::reply::stock_reply(http::server::reply::created);
            rep.content = db_name + " has been removed";
            return;
        }

        rep = http::server::reply::stock_reply(http::server::reply::internal_server_error);
        rep.content = db_name + " could not be removed for obscur reasons";
    }

    void create_table(http::server::reply &rep, std::string db_name, std::string table_name)
    {
        std::string pathfile = root_path+"/"+db_name+"/"+table_name;


        if (boost::filesystem::exists(pathfile))
        {
            rep = http::server::reply::stock_reply(http::server::reply::bad_request);
            rep.content = table_name + " already exists";
            return;
        }

        ///Create the file
        std::fstream myfile(pathfile.c_str(), std::ios::out);

        if(boost::filesystem::exists(pathfile))
        {
            rep = http::server::reply::stock_reply(http::server::reply::created);
            rep.content = table_name + " table has been created";
        }
        else
        {
            rep = http::server::reply::stock_reply(http::server::reply::internal_server_error);
            rep.content = table_name + " table could not be created for obscur reasons";
        }
    }

    void delete_table(http::server::reply &rep, std::string db_name, std::string table_name)
    {
        std::string pathfile = root_path+"/"+db_name+"/"+table_name;


        if (! boost::filesystem::exists(pathfile))
        {
            rep = http::server::reply::stock_reply(http::server::reply::not_found);
            rep.content = table_name + " could not be found";
            return;
        }

        if(boost::filesystem::remove(pathfile))
        {
            rep = http::server::reply::stock_reply(http::server::reply::created);
            rep.content = table_name + " table has been removed";
        }
        else
        {
            rep = http::server::reply::stock_reply(http::server::reply::internal_server_error);
            rep.content = table_name + " could not be created for obscur reasons";
        }
    }

    void create_row(http::server::reply &rep, std::string db_name, std::string table_name, std::string value)
    {
        std::string line;
        std::string row;
        std::vector<std::string> strs;
        std::string pathfile = root_path+"/"+db_name+"/"+table_name;
        std::ifstream myfile(pathfile.c_str());

        if(! boost::filesystem::exists(pathfile))
        {
            rep = http::server::reply::stock_reply(http::server::reply::not_found);
            rep.content = table_name + " does not exist";
        }

        if(myfile.is_open())
        {
            if (myfile.peek() == std::ifstream::traits_type::eof())
            {
                row = "1 " + value;
            }
            else
            {
                while( getline(myfile, line) )
                {
                    bool is_empty = true;
                    for (int i = 0; i < boost::numeric_cast<int>(line.size()); i++)
                    {
                        char ch = line[i];
                        is_empty = is_empty && isspace(ch);
                    }

                    if (! is_empty)
                    {
                        boost::split(strs, line, boost::is_any_of(" "));

                        try
                        {
                            int id = boost::lexical_cast<int>(strs[0]) + 1;
                            row = boost::lexical_cast<std::string>(id) + " " + value;
                        }
                        catch( boost::bad_lexical_cast const& )
                        {
                            rep = http::server::reply::stock_reply(http::server::reply::internal_server_error);
                            rep.content = "row could not be created for obscur reasons";
                            return;
                        }
                    }
                }
            }

            myfile.close();

            if (! row.empty())
            {
                std::ofstream fout;
                fout.open(pathfile.c_str(), std::ios::app);
                fout << row << std::endl;
                fout.close( );
            }

            rep = http::server::reply::stock_reply(http::server::reply::created);
            return;

        }
        else
        {
            rep = http::server::reply::stock_reply(http::server::reply::internal_server_error);
            rep.content = " row could not be inserted for obscur reasons";
        }
    }

    void delete_row(http::server::reply &rep, std::string db_name, std::string table_name, int id)
    {
        std::string line;
        std::string row;
        std::vector<std::string> strs;
        std::string pathfile = root_path+"/"+db_name+"/"+table_name;
        std::string bkp_pathfile = pathfile + ".bkp";
        std::ifstream myfile(pathfile.c_str());
        std::ofstream bkpfile(bkp_pathfile.c_str());

        if(! boost::filesystem::exists(pathfile))
        {
            rep = http::server::reply::stock_reply(http::server::reply::not_found);
            rep.content = table_name + " could not be found";
        }

        if(myfile.is_open())
        {

            while( getline(myfile, line) )
            {
                bool is_empty = true;
                for (int i = 0; i < boost::numeric_cast<int>(line.size()); i++)
                {
                    char ch = line[i];
                    is_empty = is_empty && isspace(ch);
                }

                if (! is_empty)
                {
                    boost::split(strs, line, boost::is_any_of(" "));

                    try
                    {
                        int id_row = boost::lexical_cast<int>(strs[0]);

                        if(id_row != id)
                        {
                            bkpfile << line << "\n";
                        }
                    }
                    catch( boost::bad_lexical_cast const& )
                    {
                        rep = http::server::reply::stock_reply(http::server::reply::internal_server_error);
                        rep.content = "row could not be removed for obscur reasons";
                        return;
                    }
                }
            }

            bkpfile.close();
            myfile.close();

            boost::filesystem::remove(pathfile);
            boost::filesystem::rename(bkp_pathfile, pathfile);

            rep = http::server::reply::stock_reply(http::server::reply::ok);
            rep.content = "row has been removed";
            return;

        }
        else
        {
            rep = http::server::reply::stock_reply(http::server::reply::internal_server_error);
            rep.content = " row could not be removed for obscur reasons";
        }
    }

    void list_databases(http::server::reply &rep)
    {

        boost::filesystem::path dir(root_path);

        if (! boost::filesystem::exists(dir))
        {
            rep = http::server::reply::stock_reply(http::server::reply::not_implemented);
            rep.content = "server document root does not exists";
            return;
        }

        boost::filesystem::directory_iterator end;

        rep = http::server::reply::stock_reply(http::server::reply::ok);

        for ( boost::filesystem::directory_iterator pos(dir); pos != end; ++pos )
        {
            if ( is_directory(pos->status()) )
            {
                rep.content += pos->path().filename().string() + "\n";
            }
        }
    }

    void list_tables(http::server::reply &rep, std::string db_name)
    {
        boost::filesystem::path base_path(root_path);
        boost::filesystem::path db_path(db_name);
        boost::filesystem::path dir(base_path/db_path);

        if (!boost::filesystem::exists(dir))
        {
            rep = http::server::reply::stock_reply(http::server::reply::not_found);
            rep.content = db_name + " database could not be found";
            return;
        }

        boost::filesystem::directory_iterator end;
        rep = http::server::reply::stock_reply(http::server::reply::ok);

        for (boost::filesystem::directory_iterator pos(dir); pos != end; ++pos)
        {
            if ( is_regular_file( pos->status() ) )
            {
                rep.content += pos->path().filename().string() + "\n";
            }
        }
    }

    void list_rows(http::server::reply &rep, std::string db_name, std::string table_name)
    {
        std::string pathfile = root_path+"/"+db_name+"/"+table_name;
        std::ifstream is(pathfile.c_str());
        char buf[512];

        if (!is)
        {
            rep = http::server::reply::stock_reply(http::server::reply::not_found);
            rep.content = table_name + " could not be found";
            return;
        }

        rep = http::server::reply::stock_reply(http::server::reply::ok);

        while (is.read(buf, sizeof(buf)).gcount() > 0)
        {
            rep.content.append(buf, is.gcount());
        }
    }

    void list_row(http::server::reply &rep, std::string db_name, std::string table_name, int id)
    {
        std::string pathfile = root_path+"/"+db_name+"/"+table_name;
        std::ifstream is( pathfile.c_str());
        int key;
        std::string value;

        if (!is)
        {
            rep = http::server::reply::stock_reply(http::server::reply::not_found);
            rep.content = table_name + " could not be found";
            return;
        }

        while (is >> key >> value)
        {
            if (id == key)
            {
                rep = http::server::reply::stock_reply(http::server::reply::ok);
                rep.content = value;
                return;
            }
        }

        rep = http::server::reply::stock_reply(http::server::reply::not_found);
        rep.content = "row could not be found";
    }

private:
    std::string root_path;
};
}

#endif
