#include <async.h>
#include <server.h>

int main()
{
    Server server(9000, 3);
    return server.run();
}