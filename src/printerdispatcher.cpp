#include <printerdispatcher.h>
#include <chrono>

PrinterDispatcher::PrinterDispatcher()
{
    thread_file_1 = std::thread(&PrinterDispatcher::runOutput, this);
    thread_file_2 = std::thread(&PrinterDispatcher::runOutput, this);
    thread_logger = std::thread(&PrinterDispatcher::runLogger, this);

    pthread_setname_np(thread_file_1.native_handle(), "file1");
    pthread_setname_np(thread_file_2.native_handle(), "file2");
    pthread_setname_np(thread_logger.native_handle(), "logger");

    stop.store(false);
};

void PrinterDispatcher::runLogger()
{
    auto logger = std::make_unique<ConsolePrinter>();

    while(!stop.load())
    {
        std::unique_lock lock(mutex_logger);

        condLogger.wait(lock, [this]()
        {
            return !qLogger.empty() || stop.load();
        });

        while (!qLogger.empty())
        {
            logger->print(qLogger.front());
            qLogger.pop();
        }
    }
}

void PrinterDispatcher::runOutput()
{
    auto logger = std::make_unique<FilePrinter>();
    while(!stop.load())
    {
        std::unique_lock lock(mutex_output);

        condOutput.wait(lock, [this]()
        {
            return !qOutput.empty() || stop.load();
        });

        while (!qOutput.empty())
        {
            logger->print(qOutput.front());
            qOutput.pop();
        }
    }
}

void PrinterDispatcher::push(const CommandsWithTime& command)
{

    mutex_logger.lock();
    qLogger.push(command);
    mutex_logger.unlock();

    mutex_output.lock();
    qOutput.push(command);
    mutex_output.unlock();

    condLogger.notify_all();
    condOutput.notify_one();
}

PrinterDispatcher::~PrinterDispatcher()
{
    stop.store(true);

    condLogger.notify_all();
    condOutput.notify_all();

    thread_file_1.join();
    thread_file_2.join();
    thread_logger.join();
}

PrinterDispatcher &PrinterDispatcher::create()
{
    static PrinterDispatcher d;
    return d;
}
