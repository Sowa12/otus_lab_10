#include <async.h>
#include <sstream>
#include <iostream>

namespace async
{
    Bulk* connect(const size_t bulk_size)
    {
        return new Bulk(bulk_size);
    }

    void receive(Bulk* context, const char* buffer, size_t )
    {
        std::string command(buffer);
        std::string line;

        std::stringstream sstr(command);

        while(std::getline(sstr, line))
        {
            context->parseCommand(line);
        }
    }

    void disconnect(Bulk* context)
    {
        delete context;
    }
};