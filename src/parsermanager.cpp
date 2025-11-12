#include <parsermanager.h>

ParserManager::ParserManager()
{
    work_thread = std::thread(&ParserManager::work, this);
    pthread_setname_np(work_thread.native_handle(), "parser");

    stop.store(false);
}

ParserManager &ParserManager::create()
{
    static ParserManager manager;
    return manager;
}

void ParserManager::subscribe(int id, size_t bulk_size)
{
    std::lock_guard<std::mutex> lock(mutex_parser);
    parses.insert(std::make_pair(id, std::make_unique<CommandParser>(bulk_size)));
}

void ParserManager::unsubscribe(int id)
{
    std::lock_guard<std::mutex> lock(mutex_parser);
    if (auto i = parses.find(id); i != parses.end())
    {
        parses.erase(i);
    }
}

void ParserManager::push(int id, command_type command)
{
    std::lock_guard<std::mutex> lock(mutex_parser);
    commands.push_back({id, command});
    condVar.notify_all();
}

ParserManager::~ParserManager()
{
    stop.store(true);
    condVar.notify_all();
    work_thread.join();
}

void ParserManager::work()
{
    while (!stop.load())
    {
        std::unique_lock lock(mutex_parser);

        condVar.wait(lock, [this]()
                     { return !commands.empty() || stop.load(); });

        for (auto &item : commands)
        {
            if (auto it = parses.find(item.first); it != parses.end())
            {
                it->second->parse(item.second);
            }
        }
        commands.clear();
    }
}
