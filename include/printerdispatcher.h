#pragma once

#include <printer.h>
#include <mutex>
#include <atomic>
#include <command.h>
#include <queue>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <iostream>

class PrinterDispatcher
{
public:
    static PrinterDispatcher& create();
    void push(const CommandsWithTime&);
    ~PrinterDispatcher();
private:
    PrinterDispatcher();

    void runLogger();
    void runOutput();

    std::mutex mutex_logger;
    std::mutex mutex_output;

    std::queue<CommandsWithTime> qLogger;
    std::queue<CommandsWithTime> qOutput;

    std::condition_variable condLogger;
    std::condition_variable condOutput;
    std::atomic<bool> stop;

    std::thread thread_file_1;
    std::thread thread_file_2;
    std::thread thread_logger;
};


