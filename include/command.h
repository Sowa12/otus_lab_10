#pragma once

#include <string>
#include <ctime>
#include <vector>

using command_type = std::string;
using time_type = std::time_t;
using command_container = std::vector<command_type>;

const std::string DYNAMIC_START = "{";
const std::string DYNAMIC_END = "}";
const std::string BULK_OUT = "bulk:";
/// @brief Структура данных с временем поступления первой команды
/// и со списком команд блока
struct CommandsWithTime {
    command_container commands;
    time_type startTime;
    int id;
};