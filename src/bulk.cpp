#include <bulk.h>


Bulk::Bulk(size_t size)
{
    ParserManager::create().subscribe(reinterpret_cast<std::uintptr_t>(this), size);
}

void Bulk::parseCommand(const command_type &input)
{
    ParserManager::create().push(reinterpret_cast<std::uintptr_t>(this), input);
}

Bulk::~Bulk()
{
    ParserManager::create().unsubscribe(reinterpret_cast<std::uintptr_t>(this));
}
