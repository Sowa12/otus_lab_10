#include <server.h>

Server::Server(const uint16_t port, const uint32_t bulk_size)
    : port(port)
    , bulk_size(bulk_size)
    , acceptor(io, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
    , socket(io)
    , timer(io)
    , parser(bulk_size)
{
    
}

void Server::push(const std::string &str)
{
    vstr.push_back(str);
}

int Server::run()
{
    start_accept();
    start_timer();
    return io.run();
}

void Server::start_accept()
{
    acceptor.async_accept(socket,
        [this](const boost::system::error_code ec)
        {
            if (!ec)
            {
                auto client = std::make_unique<Client>(std::move(socket), this);
                client->async_read();
                clients.push_back(std::move(client));
            }

            start_accept();
        }
    );
}

void Server::start_timer()
{
    timer.expires_after(std::chrono::seconds(5));
    timer.async_wait([this](const boost::system::error_code& ec)
    {
        if (!ec)
        {
            for (auto& command : vstr)
            {
                parser.parse(command);

                if (parser.isBlockEnd())
                {
                    PrinterDispatcher::create().push(parser.getLastCommands());
                }
            }

            if (!parser.getCommands().commands.empty())
            {
                PrinterDispatcher::create().push(parser.getCommands());
            }

            parser.tryClear();


            vstr.clear();
        }

        start_timer();
    });
}

Client::Client(asio::ip::tcp::socket &&socket, Server *server)
    : sock(std::move(socket))
    , parser(server->getBulkSize())
    , srv(server)
{
    async_read();
}

void Client::async_read()
{
    sock.async_read_some(boost::asio::buffer(data, sizeof(data)),
        [this](boost::system::error_code ec, std::size_t size)
        {
            if (!ec)
            {
                std::string str(data, size);

                std::stringstream sstr(str);
                std::string line;

                bool success = true;

                while(std::getline(sstr, line))
                {
                    parser.parse(line);

                    if (parser.getParseState() == ParserState::ParseStaticBlock
                        || parser.getParseState() == ParserState::EndParseAndDynamicBlock) 
                        success = true;

                    else if (parser.getParseState() == ParserState::ParseDynamicBlock) success = false;

                    if (success)
                    {
                        srv->push(line);
                    } else{
                        if (parser.isBlockEnd())
                        {
                            PrinterDispatcher::create().push(parser.getLastCommands());
                        }
                    }
                }
               
                async_read();
            }
        }
    );
}
