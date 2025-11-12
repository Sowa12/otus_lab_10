#pragma once
#include <command.h>
#include <memory>

/// @brief Интерфейс для вывода командых пакетов
class IPrinter {
    public:
    virtual void print(const CommandsWithTime& container) = 0;
    virtual ~IPrinter() = default;
};
/// @brief Класс вывода командых пакетов в консоль
class ConsolePrinter : public IPrinter {
    public:
    void print(const CommandsWithTime& container) override;
};
/// @brief Класс вывода командых пакетов в файл
class FilePrinter : public IPrinter {
    public:
    void print(const CommandsWithTime& container) override;
};

using uPrinter = std::unique_ptr<IPrinter>;
using printer_container = std::vector<uPrinter>;

