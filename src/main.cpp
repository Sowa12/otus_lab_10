#include <async.h>
#include <server.h>

int main(int argc, char** argv)
{
    uint16_t port;
    uint32_t bulk_size;

    if (argc < 3)
    {
        std::cout << "example bulk_server <port> <bulk_size>\nport - uint16_t\nbulk_size - uint32_t" << std::endl;
        return 0;
    }

    try {
        port = std::stoul(argv[1]);
        bulk_size = std::stoul(argv[2]);

    } catch(...)
    {
        std::cout << "example bulk_server <port> <bulk_size>\nport - uint16_t\nbulk_size - uint32_t" << std::endl;
        return 0;
    }

    Server server(port, bulk_size);
    return server.run();
}