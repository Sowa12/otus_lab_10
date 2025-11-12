#pragma once

#include <parsermanager.h>
#include <cstdint>
/// @brief Класс реализующий основую логику работы
class Bulk {
public:
    Bulk(size_t);
    Bulk(const Bulk&) = delete;
    Bulk(Bulk&&) = delete;

    void parseCommand(const command_type&);

    Bulk& operator=(const Bulk&) = delete;
    Bulk& operator=(Bulk&&) = delete;

    ~Bulk();
};