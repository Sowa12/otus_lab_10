#pragma once
#include <command.h>
#include <memory>
#include <printerdispatcher.h>
#include <iostream>

/// @brief Состояния разбора команд
enum class ParserState : uint8_t {
    Empty,
    ParseStaticBlock,
    ParseDynamicBlock,
    EndParse,
    EndParseAndDynamicBlock
};
/// @brief Интерфейс парсера команд
class ICommandParser {
public:
    ICommandParser(const size_t size) 
    : _staticSize(size)
    , _parseState(ParserState::Empty) {}

    virtual void parse(const std::string&) = 0;
    virtual bool isBlockEnd() = 0;

    virtual CommandsWithTime getCommands() = 0;
    virtual CommandsWithTime getLastCommands() = 0;

    ParserState getParseState() {return _parseState;}
    virtual ~ICommandParser() = default;
protected:
    size_t _staticSize;
    ParserState _parseState;
};

/// @brief Парсер командных пакетов
class CommandParser : public ICommandParser {
    public:
    CommandParser(const size_t);

    void parse(const std::string&) final;
    bool isBlockEnd() final;

    CommandsWithTime getCommands() final;
    CommandsWithTime getLastCommands() final;
    
    static size_t getId() { static size_t staticID = 0; return ++staticID;};

    void tryClear();

    ~CommandParser() = default;
private:
    CommandsWithTime _container;
    CommandsWithTime _lastContainer;

    size_t _countOpen;
    size_t id;

    void push(const std::string& command);
};


using uCommandParser = std::unique_ptr<ICommandParser>;