#pragma once

#include <parser.h>
#include <map>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>

class ParserManager
{
public:
    static ParserManager& create();
    void subscribe(int, size_t);
    void unsubscribe(int);
    void push(int, command_type);
    ~ParserManager();
private:
    ParserManager();
    std::map<int, uCommandParser> parses;
    std::vector<std::pair<int, command_type>> commands;
    std::atomic<bool> stop;
    std::mutex mutex_parser;
    std::condition_variable condVar;
    std::thread work_thread;

    void work();
};