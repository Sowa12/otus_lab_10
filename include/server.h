#include <boost/asio.hpp>
#include <map>
#include <vector>
#include <string>
#include <parser.h>
#include <memory>

namespace asio = boost::asio;

class Server;

class Client
{
public:
    Client(asio::ip::tcp::socket&& socket, Server* server);
    void async_read();
private:
    asio::ip::tcp::socket sock;
    CommandParser parser;
    Server* srv;
    char data[4096];
};

class Server
{
public:
    Server(const uint16_t port, const uint32_t bulk_size);
    uint32_t getBulkSize() {return  bulk_size; }
    void push(const std::string& str);
    int run();
private:
    asio::io_context io;

    uint16_t port;
    uint32_t bulk_size;

    asio::ip::tcp::acceptor acceptor;
    asio::ip::tcp::socket socket;
    asio::steady_timer timer;
    
    std::vector<std::unique_ptr<Client>> clients;
    CommandParser parser;

    std::vector<std::string> vstr;
    
    void start_accept();
    void start_timer();
};